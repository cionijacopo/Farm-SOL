/*
    Created by Jacopo Cioni
    FarmProject - SOL
    File sorgente contenente il funzionamento del collector.
*/

#include"../../includes/farm_collector.h"
#include"../../includes/connection.h"
#include"../../includes/linkedlist.h"
#include"../../includes/isregular.h"
#include<sys/socket.h>
#include<sys/un.h>
#include<string.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>

#define _POSIX_C_SOURCE 200112L

// Indirizzo
socklen_t address_length;

int serverSocket(int *fd_socket, struct sockaddr_un *socket_add) {
    // Ottengo il file descriptor del socket
    (*fd_socket) = socket(AF_UNIX, SOCK_STREAM, 0);
    // Controllo che sia tutto ok
    if((*fd_socket) == -1) {
        perror("SOCKET");
        cleanup();
        return -1;
    }
    // Faccio il binding 
    if((bind((*fd_socket), (struct sockaddr *)socket_add, sizeof(struct sockaddr_un))) != 0) {
        perror("BIND");
        close((*fd_socket));
        cleanup();
        return -1;
    }
    if((listen((*fd_socket), MAX_BACK_LOG)) != 0) {
        perror("LISTEN");
        close((*fd_socket));
        cleanup();
        return -1;
    }
    return 0;
}

static int handler(int fd_client, int *termina, FinalNode_t *final_list, char string[], long int *ris) {
    //Gestione del client, da completare
    char collector_buffer[MAX_PATH_LENGTH] = {'\0'};
    int n;
    if((n = readn(fd_client, collector_buffer, MAX_PATH_LENGTH)) == -1) {
        fprintf(stderr, "FATAL ERROR: readn collector.\n");
        close(fd_client);
        return -1;
    }

    // Controllo se sto terminando
    if(strncmp(collector_buffer, "END", 3) == 0) {
        close(fd_client);
        (*termina) = 1;
        return 1;
    } else if(strncmp(collector_buffer, "SIGNAL", 6) == 0) {
        close(fd_client);
        printFinalList(final_list);
        return 0;
    } else {
        // Aggiungo alla lista finale
        // Faccio prima la tokenizer 
        char *token = strtok(collector_buffer, "-");
        *ris = strtol(token, NULL, 10);
        token = strtok(NULL, "-");
        strncpy(string, token, strlen(token)+1);
        close(fd_client);
        return 0;
    }
}

int farm_collector() {
    int fd_socket;
    int termina = 0;
    char string[MAX_PATH_LENGTH];
    long int ris;
    FinalNode_t *final_list = NULL;
    // Pulizia
    cleanup();
    //Inizializzazione dell'indirizzo
    struct sockaddr_un socket_address;
    socket_address.sun_family = AF_UNIX;
    strncpy(socket_address.sun_path, SOCKETNAME, strlen(SOCKETNAME)+1);
    // Inizializzazione del Socket
    if(serverSocket(&fd_socket, &socket_address) != 0) {
        // Errore inizializzazione del server
        fprintf(stderr, "FATAL ERROR: inizializzazione del server.\n");
        return -1;
    }

    // ciclo del server. Qui verranno accettate nuove connessioni in entrata dai vari client
    while(termina == 0) {
        int fd_client = accept(fd_socket, (struct sockaddr *)&socket_address, &address_length);
        if(fd_client == -1) {
            if(errno == EINTR) continue;
            else {
                fprintf(stderr, "FATAL ERROR: accept client.\n");
                // Pulisco
                cleanup();
                // Interrompo forzatamente il ciclo
                return -1;
            }
        } else {
            int temp = handler(fd_client, &termina, final_list, string, &ris); 
            if(temp == -1) {
                return -1;
            } 
            if(temp == 0){
                // printf("Val: %ld - String: %s\n", ris, string);
                //printf("---\nAggiungo: %s\n---\n", string);
                final_list = orderInsert(final_list, string, ris);
            }
        }
    }

    // Terminazione
    //fprintf(stdout, "Terminazione collector.\n");
    printFinalList(final_list); 
    if(deleteFinalList(final_list) == NULL) {
        cleanup();
        return 0;
    } else {
        cleanup();
        return -1;
    } 
}

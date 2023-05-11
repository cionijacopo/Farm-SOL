/*
    Created by Jacopo Cioni
    FarmProject - SOL
*/

#include"../../includes/farm_collector.h"
#include"../../includes/connection.h"
#include"../../includes/linkedlist.h"
#include<sys/socket.h>
#include<string.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>

#define _POSIX_C_SOURCE 200112L

static int handler(int fd_client, int *termina, FinalNode_t *final_list) {
    //Gestione del client, da completare
    char *collector_buffer = NULL;
    int length;
    int n;
    // Leggo prima la lunghezza del messaggio
    if((n = readn(fd_client, &length, sizeof(int))) == -1) {
        fprintf(stderr, "FATAL ERROR: readn collector.\n");
        close(fd_client);
        return -1;
    }
    if((n = readn(fd_client, collector_buffer, length)) == -1) {
        fprintf(stderr, "FATAL ERROR: readn collector.\n");
        close(fd_client);
        return -1;
    }

    collector_buffer[length] = '\0';

    // Controllo se sto terminando
    if(strncmp(collector_buffer, "END", length) == 0) {
        close(fd_client);
        (*termina) = 1;
        return 0;
    } else if(strncmp(collector_buffer, "SIGNAL", length) == 0) {
        close(fd_client);
        printFinalList(final_list);
        return 0;
    } else {
        // Aggiungo alla lista finale
        // Faccio prima la tokenizer 
        char *nome;
        long int ris;
        char *token = strtok(collector_buffer, "-");
        ris = strtol(token, NULL, 10);
        token = strtok(NULL, "-");
        nome = token;
        // Aggiungo il nodo
        final_list = insertFinalNode(final_list, nome, ris);
        if(final_list == NULL) {
            fprintf(stderr, "Errore inserimento nodo finale.\n");
            close(fd_client);
            return -1;
        }
        close(fd_client);
        return 0;
    }
}

int farm_collector() {
    int fd_socket;
    int termina = 0;
    FinalNode_t *final_list = NULL;
    // Pulizia
    cleanup();
    //Inizializzazione dell'indirizzo
    initServerAddress();
    // Inizializzazione del Socket
    if(serverSocket(&fd_socket) != 0) {
        // Errore inizializzazione del server
        fprintf(stderr, "FATAL ERROR: inizializzazione del server.\n");
        return -1;
    }

    // ciclo del server. Qui verranno accettate nuove connessioni in entrata dai vari client
    while(termina == 0) {
        int fd_client = accettaClient(fd_socket);
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
            if(handler(fd_client, &termina, final_list) == -1) {
                return -1;
            }
        }
    }

    // Terminazione
    //fprintf(stdout, "Terminazione collector.\n");
    printFinalList(final_list);
    if(deleteFinalList(final_list) == NULL) {
        cleanup();
        return -1;
    } else {
        cleanup();
        return 0;
    } 
}
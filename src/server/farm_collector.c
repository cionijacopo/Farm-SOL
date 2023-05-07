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

static int handler(int fd_client, int *termina) {
    //Gestione del client, da completare
    char *collector_buffer = NULL;
    int length;
    int n;
    // Leggo prima la lunghezza del messaggio
    if((n = readn(fd_client, &length, sizeof(int))) == -1) {
        fprintf(stderr, "FATAL ERROR: readn collector.\n");
        return -1;
    }
    if((n = readn(fd_client, collector_buffer, length)) == -1) {
        fprintf(stderr, "FATAL ERROR: readn collector.\n");
        return -1;
    }

    collector_buffer[length] = '\0';

    // Controllo se sto terminando
    if(strncmp(collector_buffer, "END", length) == 0) {
        close(fd_client);
        (*termina) = 1;
        return 0;
    } else {
        close(fd_client);
        // Qui devo gestire il client, per ora mi limito a stampare
        fprintf(stdout, "%s\n", collector_buffer);

        return 0;
    }
}

int farm_collector() {
    int fd_socket;
    int termina = 0;
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
            if(handler(fd_client, &termina) == -1) {
                return -1;
            }
        }
    }

    // Terminazione
    fprintf(stdout, "Terminazione collector.\n");
    cleanup();
    return 0;
}
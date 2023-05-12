/*
    Created by Jacopo Cioni
    FarmProject - SOL
    File sorgente contenente il funzionamento del processo master_worker.
*/

#include"../../includes/farm_master_worker.h"
#include"../../includes/utils.h"
#include"../../includes/threadutils.h"
#include"../../includes/task_queue.h"
#include"../../includes/connection.h"
#include"../../includes/pool.h"
#include"../../includes/isregular.h"
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>

extern volatile __sig_atomic_t force;

int farm_master(int num_thread, int q_length, int r_time, char *dir_name, int argc, char **argv, int optind) {
    // Prima di tutto devo creare la coda ed inizializzarla
    TaskQueue_t *safe_queue = initQueue(q_length, r_time);
    if(safe_queue == NULL) {
        fprintf(stderr, "FATAL ERROR: inizializzazione coda.\n");
        // deleteList(lista);
        return -1;
    }
    
    // Poi creo la pool di threads
    pthread_t pool[num_thread];
    for(int i = 0; i < num_thread; i++) {
        createThread(&pool[i], NULL, workerFun, (void *) safe_queue);
    }

    int i = optind;
    while(i<argc) {
        if((strlen(argv[i])+1) > MAX_ARGV_LENGTH) {
            fprintf(stderr, "Errore: argomento formato da troppi caratteri. Ignorato.\n");
        } else {
            // printf("Test.\n");
            if (is_regular_file(argv[i]) == 0) {
                if(force == 1) {
                    printf("Ricevuto SEGNALE.\n");
                    break;
                }
                int temp = pushPool(safe_queue, argv[i]);
                if(temp == -1) {
                    fprintf(stderr, "FATAL ERROR: inserimento elementi nella pool.\n");
                    return -1;
                }
                if(temp == 1) {
                    break;
                }
            } else {
                fprintf(stderr, "Errore: File non regolare/stat fallita. Ignorato.\n");
            }
        }
        i++;
    }
    if(force != 1) {
        if (dir_name != NULL) {
            // Controllo che dir sia una cartella regolare
            if(is_regular_folder(dir_name) == 0) {
                naviga_cartella(dir_name, safe_queue);
            } else {
                fprintf(stderr, "Errore: cartella non regolare. Ignorato.\n");
            }
        }
    }
    
    // Avviso della terminazione
    LOCK(&(safe_queue->qlock));
    safe_queue->uscita++;
    BCAST(&(safe_queue->vuoto));
    UNLOCK(&(safe_queue->qlock));

    // Attendo la terminazione dei threads
    for(int i = 0; i < num_thread; i++) {
        JOIN(pool[i]);
    }

    // File descriptor del processo master
    int fd_master = clientSocket();
    // Ho finito, comunico al collector la terminazione
    int n;
    if((n = writen(fd_master, "END", 3)) == -1) {
        fprintf(stderr, "Errore writen master.\n");
        return -1;
    }
    // Chiudo la connessione
    close(fd_master);
    // Libero tutta la memoria
    free(safe_queue->coda);
    pthread_mutex_destroy(&(safe_queue->qlock));
    pthread_cond_destroy(&(safe_queue->pieno));
    pthread_cond_destroy(&(safe_queue->vuoto));
    free(safe_queue);
    return 0;
}

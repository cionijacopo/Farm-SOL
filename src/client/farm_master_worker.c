/*
    Created by Jacopo Cioni
    FarmProject - SOL
*/

#include"../../includes/farm_master_worker.h"
#include"../../includes/linkedlist.h"
#include"../../includes/utils.h"
#include"../../includes/threadutils.h"
#include"../../includes/task_queue.h"
#include"../../includes/connection.h"
#include"../../includes/pool.h"
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

extern volatile __sig_atomic_t force;

int farm_master(int num_thread, int q_length, int r_time, Node_t *lista) {
    // Prima di tutto devo creare la coda ed inizializzarla
    TaskQueue_t *safe_queue = initQueue(q_length, r_time);
    if(safe_queue == NULL) {
        fprintf(stderr, "FATAL ERROR: inizializzazione coda.\n");
        deleteList(lista);
        return -1;
    }

    // TEST: --> OK
    // printf("TEST.\n");
    // printList(lista);
    
    // Poi creo la pool di threads
    pthread_t pool[num_thread];
    for(int i = 0; i < num_thread; i++) {
        createThread(&pool[i], NULL, workerFun, (void *) safe_queue);
    }


    // Comincio ad aggiungiere i files
    while(lista!= NULL) {
        // Aggiungo l'elemento
        if(force == 1) {
            printf("Ricevuto SEGNALE.\n");
            break;
        }
        int temp = pushPool(safe_queue, lista->info);
        if(temp == -1) {
            fprintf(stderr, "FATAL ERROR: inserimento elementi nella pool.\n");
            return -1;
        }
        if(temp == 1) {
            break;
        }
        // printTaskQueue(safe_queue);
        // Scorro la lista di file regolari
        lista = lista->next;
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

    int fd_master = clientSocket();
    // int length = 3;
    int n;
    /*
    if((n = writen(fd_master, &length, sizeof(int))) == -1) {
        fprintf(stderr, "Errore writen master.\n");
        return -1;
    }
    if((n = writen(fd_master, "END", length*sizeof(char))) == -1) {
        fprintf(stderr, "Errore writen master.\n");
        return -1;
    }
    */
    if((n = writen(fd_master, "END", 3)) == -1) {
        fprintf(stderr, "Errore writen master.\n");
        return -1;
    }
    // Chiudo la connessione
    close(fd_master);
    // Libero tutta la memoria
    deleteList(lista);
    free(safe_queue->coda);
    pthread_mutex_destroy(&(safe_queue->qlock));
    pthread_cond_destroy(&(safe_queue->pieno));
    pthread_cond_destroy(&(safe_queue->vuoto));
    free(safe_queue);

    return 0;
}


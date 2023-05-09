/*
    Created by Jacopo Cioni
    FarmProject - SOL
*/

#include"../../includes/task_queue.h"
#include"../../includes/utils.h"
#include"../../includes/threadutils.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<errno.h>
#include<pthread.h>

TaskQueue_t *initQueue(int max_length, int r_time) {
    TaskQueue_t *safe_queue = (TaskQueue_t *)s_malloc(sizeof(TaskQueue_t));
    if(!safe_queue) {
        return NULL;
    }
    safe_queue->coda = s_malloc(max_length*sizeof(char *));
    if(!safe_queue->coda) {
        return NULL;
    }
    // Inizializzo i nodi
    for(int i = 0; i < max_length; i++) {
        safe_queue->coda[i] = NULL;
    }
    safe_queue->pos = 0;
    safe_queue->max_length = max_length;
    safe_queue->r_time = r_time;
    safe_queue->uscita = -1;
    if(pthread_mutex_init(&safe_queue->qlock, NULL) != 0) {
        perror("mutex init");
        return NULL;
    }
    if(pthread_cond_init(&safe_queue->pieno, NULL) != 0 || pthread_cond_init(&safe_queue->vuoto, NULL) != 0) {
        perror("cond init");
        if(&safe_queue->qlock) {
            pthread_mutex_destroy(&safe_queue->qlock);
        }
        return NULL;
    }
    return safe_queue;
}

int pushPool(TaskQueue_t *lista, char *info) {
    if((lista == NULL) || (info == NULL)){
        errno = EINVAL;
        perror("pushPool");
        return -1;
    }
    // Prendo la lock, devo giocare con il problema produttore-consumatore
    // Dopo aver preso la lock devo mettermi in attesa fino a quando la coda è piena.
    LOCK(&lista->qlock);

    // Devo gestire i segnali, nel caso di una interrupt devo uscire


    // Controllo la condizione di attesa e verifico che la coda non sia piena
    while(lista->pos >= lista->max_length) {
        WAIT(&lista->pieno, &lista->qlock);
    }

    // Una volta svegliato viene ripresa la lock. aggiungo l'elemento alla coda
    // Se è la prima volta che trovo questa posizione, alloco lo spazio
    if(lista->coda[lista->pos] == NULL) {
        lista->coda[lista->pos] = s_malloc(strlen(info)+1 * sizeof(char));
    }
    // Copio infio
    strncpy(lista->coda[lista->pos], info, strlen(info)+1);
    lista->pos++;

    // Invio un segnale sulla condizione
    SIGNAL(&lista->vuoto);
    //Rilascio la lock
    UNLOCK(&lista->qlock);
    return 0;
}

char *popPool(TaskQueue_t *lista) {
    if(lista == NULL) {
        errno = EINVAL;
        perror("popPool");
        return NULL;
    }
    // Puntatore al dato estratto
    char *info;
    // Rimuovo l'elemento
    info = lista->coda[lista->pos];
    lista->coda[lista->pos] = NULL;
    lista->pos--;

    return info;
}






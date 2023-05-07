#if !defined(TASK_QUEUE_H_)
#define TASK_QUEUE_H_

#include<pthread.h>

typedef struct task {
    // Informazione
    char *info; 
    // Puntatore al prossimo nodo
    struct task *next;
} Task_t;

typedef struct taskqueue {
    // Coda concorrente
    Task_t *coda;
    // Lunghezza MAX coda
    int max_length;
    // Posizione
    int pos;
    // Lock della coda
    pthread_mutex_t qlock;
    // Conditions per la gestione del produttore/consumatore
    pthread_cond_t pieno;
    pthread_cond_t vuoto;
    // Tempo di attesa tra due richieste successive
    int r_time;
    // Possibile variabile di uscia
    
} TaskQueue_t;

#endif

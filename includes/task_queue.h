#if !defined(TASK_QUEUE_H_)
#define TASK_QUEUE_H_

#include<pthread.h>

typedef struct taskqueue {
    // Coda concorrente - riferimento alla testa
    char **coda;
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
    // Possibile variabile di uscita: -1 -> ok ; 0 -> esco
    int uscita;
} TaskQueue_t;

/**
 * @function initQueue
 * @brief inizializza la coda dei task
 * @param safe_queue coda da inizializzare
 * @return 0 in caso di successo, -1 altrimenti
*/
TaskQueue_t *initQueue(int max_length, int r_time);

/**
 * @function pushPool
 * @brief permette l'inserimento di un nodo nella coda del pool
 * @param lista pool dove inserire un nodo
 * @param info informazione da inserire nella lista
 * @return 0 in caso di succeso, -1 in caso di errore (errno settato)
*/
int pushPool(TaskQueue_t *lista, char *info);

/**
 * @function popPool
 * @brief estrae un dato dalla coda
 * @param lista coda da cui estrarre l'elemento
 * @return puntatore al dato estratto (errno settato)
*/
char *popPool(TaskQueue_t *lista);

#endif

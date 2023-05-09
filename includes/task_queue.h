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

/**
 * @function initQueue
 * @brief inizializza la coda dei task
 * @param safe_queue coda da inizializzare
 * @return 0 in caso di successo, -1 altrimenti
*/
int initQueue(TaskQueue_t *safe_queue, int max_length, int r_time);

/**
 * @function newNodePool
 * @brief permette la creazione di un nodo della coda
 * @param info informazione da inserire nel nodo
 * @return nodo creato
*/
Task_t *newNodePool(char *info);

/**
 * @function postInsertPool
 * @brief permette l'inserimento di un nodo nella coda del pool
 * @param lista lista dove inserire un nodo
 * @param info informazione da inserire nella lista
 * @return puntatore alla lista con nuovo nodo
*/
Task_t *postInsertPool(Task_t *lista, char *info);

/**
 * @function removePoolTask
 * @brief permette la rimozione di un nodo dalla coda del pool
 * @param lista lista da dove viene rimosso il nodo
 * @return valore rimosso
*/
char *removePoolTask(Task_t **lista);

/**
 * @function printPoolList
 * @brief permette la stampa della coda della pool
 * @param lista da stampare
*/
void printPoolList(Task_t *lista);

#endif

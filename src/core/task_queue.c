/*
    Created by Jacopo Cioni
    FarmProject - SOL
*/

#include"../../includes/task_queue.h"
#include"../../includes/utils.h"
#include<string.h>

int initQueue(TaskQueue_t *safe_queue, int max_length, int r_time) {
    // Definisco la lunghezza massima
    safe_queue->max_length = max_length;
    // Definisco il tempo di ritardo
    safe_queue->r_time = r_time;
    // Definisco la posizione iniziale
    safe_queue->pos = 0;
    // Definisco mutex e conditions
    if(pthread_mutex_init(&safe_queue->qlock, NULL) != 0 || pthread_cond_init(&safe_queue->pieno, NULL) != 0 || pthread_cond_init(&safe_queue->vuoto, NULL) != 0) {
        perror("mutex/conditions init");
        return -1;
    }
    return 0;
} 

Task_t *newNodePool(char *info) {
    Task_t *ptr = (Task_t *)s_malloc(sizeof(Task_t));
    ptr->info = (char *)s_malloc(strlen(info)+1);
    strncpy(ptr->info, info, strlen(info)+1);
    ptr->next = NULL;
    return ptr;
}

Task_t *postInsertPool(Task_t *lista, char *info) {
    if(lista == NULL) {
        return newNodePool(info);
    } else {
        lista->next = postInsertPool(lista->next, info);
    }
    return lista;
}

char *removePoolTask(Task_t **lista) {
    char *value;
    Task_t **last = lista;
    if(*lista == NULL) {
        return NULL;
    }
    while((*lista)->next != NULL) {
        last = lista;
        lista = &(*lista)->next;
    }
    value = (*lista)->info;
    (*last)->next = NULL;
    free(*lista);
    *lista = NULL;
    return value;
}

void printPoolList(Task_t *lista) {
    if(lista != NULL) {
        fprintf(stdout, "%s\n", (char *)lista->info);
        printPoolList(lista->next);
    } else {
        fprintf(stdout, "NULL\n");
    }
}

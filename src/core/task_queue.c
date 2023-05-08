/*
    Created by Jacopo Cioni
    FarmProject - SOL
*/

#include"../../includes/task_queue.h"

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
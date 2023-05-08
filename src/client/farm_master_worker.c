/*
    Created by Jacopo Cioni
    FarmProject - SOL
*/

#include"../../includes/farm_master_worker.h"
#include"../../includes/linkedlist.h"
#include"../../includes/utils.h"
#include"../../includes/threadutils.h"
#include"../../includes/task_queue.h"
#include<stdio.h>
#include<pthread.h>

int farm_master(int num_thread, int q_length, int r_time, Node_t *lista) {
    // Prima di tutto devo creare la coda ed inizializzarla
    TaskQueue_t *safe_queue = (TaskQueue_t *)s_malloc(sizeof(TaskQueue_t));
    if(initQueue(safe_queue, q_length, r_time) != 0) {
        fprintf(stderr, "FATAL ERROR: inizializzazione coda.\n");
        return -1;
    }
    
    // Poi creo la pool di threads
    pthread_t pool[num_thread];
    for(int i = 0; i < num_thread; i++) {
        createThread(&pool[i], NULL, workerFun, (void *) safe_queue);
    }

    // Comincio ad aggiungiere i files
}
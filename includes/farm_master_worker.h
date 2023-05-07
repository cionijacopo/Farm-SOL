/*
    Created by Jacopo Cioni
    FarmProject - SOL
*/
#if !defined(FARM_MASTER_WORKER_H_)
#define FARM_MASTER_WORKER_H_

#include"./linkedlist.h"

/**
 * @function farm_master_worker
 * @brief master_worker del progetto
 * @return 0 in caso di successo, -1 altrimenti
*/
int farm_master(int num_thread, int q_length, int r_time, Node_t *lista);

#endif

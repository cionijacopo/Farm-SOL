/*
    Created by Jacopo Cioni
    farmProject - SOL
    Header contenente il metodo dello worker. 
*/
#if !defined(POOL_H_)
#define POOL_H_

/**
 * @function workerFun
 * @brief funzione che svolge il ruolo dello worker, si occupa di consumare l'elemento dalla coda e calcolare il risultato
 * @param args è la coda concorrente della pool
*/
void *workerFun(void *args);

#endif

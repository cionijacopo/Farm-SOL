/*
    Created by Jacopo Cioni
    farmProject - SOL
    Header contenente i metodi per la connessione e scrittura/lettura sui socket
*/

#if !defined(CONNECTION_H_)
#define CONNECTION_H_

#include<stdio.h>

#define MAX_BACK_LOG 32
#define SOCKETNAME "./farm.sck"
 
/**
 * @function cleanup
 * @brief pulizia della SOCKETNAME
*/
void cleanup();

/**
 * @function clientSocket
 * @brief funzione che restituisce il file descriptor del client connesso ala socket
 * @return fd_client in caso di successo, -1 in caso di errore
*/
int clientSocket();

/**
 * @function readn
 * @brief funzione di lettura di un buffer di dimensione size, evita letture parziali
 * @param fd file descriptor 
 * @param buf buffer
 * @param size dimensione del buffer
 * @return 0 se durante la lettura leggo EOF, -1 in caso di errore, size se termina correttamente
*/
int readn(long fd, void *buf, size_t size);

/**
 * @function writen
 * @brief funzione di scrittura di un buffer di dimensione size, evita scritture parziali
 * @param fd file descriptor 
 * @param buf buffer
 * @param size dimensione del buffer
 * @return 0 se durante la scrittura la write ritorna 0, -1 in caso di errore, 1 se termina correttamente
*/
int writen(long fd, void *buf, size_t size);

#endif

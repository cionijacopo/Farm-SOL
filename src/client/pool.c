/*
    Created by Jacopo Cioni
    FarmProject - SOL
*/

#include"../../includes/pool.h"
#include"../../includes/task_queue.h"
#include"../../includes/threadutils.h"
#include"../../includes/connection.h"
#include"../../includes/utils.h"
#include<string.h>
#include<unistd.h>

static long workerResult(char *info) {
    FILE *file;
    long int temp = 0;
    long int ris = 0;
    // Valori compresi 0 - 255
    unsigned char buf[8];

    //File binario in apertura
    file = fopen(info, "rb");
    if(file == NULL) {
        fprintf(stderr, "Errore nell'apertura del file.\n");
    }
    //File binario in lettura
    while(fread(buf, sizeof(buf), 1, file) == 1) {
        ris = ris + temp * *(buf);
        temp++;
    }
    // Restituisco il risultato
    fclose(file);
    return ris;
}

// Consumatore della coda concorrente
void *workerFun(void *args) {
    TaskQueue_t *safe_queue = (TaskQueue_t *)args;
    int fd_client;
    long int ris;
    char *info;

    // TEST: --> OK
    // printf("Test di prova\n"); 
    // printTaskQueue(safe_queue); 

    // Ciclo infinito
    for(;;) {
        LOCK(&(safe_queue->qlock));
        // Controllo la condizione di attesa, il consumatore deve verificare che la coda non sia vuota
        // Nel caso si mette in attesa
        /*
        if((safe_queue->pos) % safe_queue->max_length == safe_queue->tail && safe_queue->coda[safe_queue->pos] == NULL){
            printf("[CONSUMATORE] --> Coda Vuota, non posso estrarre.\n");
        } else {
            printf("[CONSUMATORE] --> Coda con elementi, posso estrarre.\n"); 
        }
        */
        while((safe_queue->pos) % safe_queue->max_length == safe_queue->tail && safe_queue->coda[safe_queue->pos] == NULL) {
            // Controllo che non sia attivo il flag di terminazione
            if(safe_queue->uscita != -1) {
                UNLOCK(&(safe_queue->qlock));
                return (void *)0;
            }
            WAIT(&(safe_queue->vuoto), &(safe_queue->qlock));
        }

        // Controllo se sono nella fase di terminazione
        if(((safe_queue->pos % safe_queue->max_length) == safe_queue->tail && safe_queue->coda[safe_queue->pos] == NULL) && safe_queue->uscita != -1) {
            UNLOCK(&(safe_queue->qlock));
            return (void *)0;
        }
        // TEST: --> OK
        // printf("Test di prova\n"); 
        // printTaskQueue(safe_queue);  
        
        // La wait riprende la lock quando la condizione è verificata
        info = popPool(safe_queue);
        //printf("\n---\nDato estratto: %s \n---\n", info);
        
        // Rilascio la lock
        UNLOCK(&(safe_queue->qlock));

        // Lo worker procede con il calcolo del risultato
        ris = workerResult(info); 
        // printf("Risultato: %ld \n", ris); 
        // Cerco il numero di cifre in modo da allocare esattamente lo spazio necessario
        int cifre = 0;
        long int temp = ris;
        do {
            temp = temp/10;
            ++cifre;
        }while(temp != 0);
        // Carattere di fine stringa
        cifre++;
        // Spazio per il buffer
        char *buf_msg = (char *)s_malloc((cifre + strlen(info) +1 +1) * sizeof(char));
        //Stackoverflow
        snprintf(buf_msg, cifre+1, "%ld", ris);
        strncat(buf_msg, "-", 2);
        strncat(buf_msg, info, strlen(info));
        // Calcolo la lunghezza della stringa
        // int length = strlen(buf_msg); 

        fd_client = clientSocket();
        if(fd_client == -1){
            fprintf(stderr, "Errore connessione del thread worker.\n");
            break;
        }
        int n;
        // printf("FD CLIENT: %d\n-----\n", fd_client); 
        if((n = writen(fd_client, buf_msg, strlen(buf_msg))) == -1) {
            fprintf(stderr, "Errore writen.\n");
            break;
        }

        //Chiudo il file descriptor e libero memoria
        close(fd_client);
        free(info);
        free(buf_msg);

        // Attesa fino alla prossima esecuzione
        sleep(safe_queue->r_time * 0.001);
    }
    return (void *)0;
} 
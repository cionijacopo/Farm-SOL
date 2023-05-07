#if !defined(UTILS_H_)
#define UTILS_H_

#include<stdio.h>

// Lunghezza massima della stringa passata come parametro
#define MAX_ARGV_LENGTH 512
// Lunghezza massima del path di una cartella
// #define MAX_PATH_LENGHT 256
// Valore di default per il numero di threads
#define DEFAULT_NUM_THREADS 4
// Valore di default per la lunghezza della coda
#define DEFAULT_QUEUE_LENGTH 8
// Valore di default per il tempo di ritardo
#define DEFAULT_DELAY_TIME 0

/**
 * @function s_malloc
 * @brief versione safe della malloc
 * @param size dimensione da allocare
 * @return puntatore alla memoria allocata
*/
void *s_malloc(size_t size);

/**
 * @function isNumber
 * @brief La funzione isNumber prende come parametro una stringa e controlla se è un numero.
 * @param s è la stringa da controllare
 * @param n è la stringa convertita in numero
 * @return 0: successo, 1: non è un numero, 2: errore di overflow/underflow
*/
int isNumber(const char* s, long *n);

#endif

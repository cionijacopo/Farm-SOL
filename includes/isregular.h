#if !defined(ISREGULAR_H_)
#define ISREGULAR_H_

#include"./linkedlist.h"

#define MAX_PATH_LENGTH 4096

/**
 * @function is_regular_file
 * @brief La funzione controlla che il parametro sia un file.
 * @param file file da controllare
 * @return 0: successo, 1: non è un file, -1: Errore Stat
*/
int is_regular_file(char *file);

/**
 * @function is_regular_folder
 * @brief La funzione controlla che il parametro sia una cartella.
 * @param folder_name cartella da controllare
 * @return 0: successo, 1: non è una cartella, -1: Errore Stat
*/
int is_regular_folder(char *folder_name);

/**
* @function naviga_cartella
* @brief funzione ricorsiva che naviga la cartella passata come parametro ed aggiunge i file ad una lista
* @param folder cartella che viene navigata
* @param lista dove viene inserito il file
*/
void naviga_cartella(char *folder_name, Node_t *lista);

#endif

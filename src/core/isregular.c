/*
    Created by Jacopo Cioni
    FarmProject - SOL
    File sorgente che contiene i controlli sui file/cartelle e la navigazione della cartella.
*/

#include"../../includes/isregular.h"
#include"../../includes/utils.h"
#include"../../includes/task_queue.h"

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>



int is_regular_file(char *file) {
    struct stat pathbuf;
    if(stat(file, &pathbuf) != 0) {
        // Errore stat
        return -1;
    }
    // Se ritorna zero non è un file
    if(S_ISREG(pathbuf.st_mode) == 0) {
        return 1;
    } else {
        return 0;
    }
}

int is_regular_folder(char *folder_name) {
    struct stat pathbuf_f;
    if(stat(folder_name, &pathbuf_f) != 0) {
        // Errore stat
        perror("stat");
        return -1;
    }
    // Se ritorna zero non è un file
    if(S_ISDIR(pathbuf_f.st_mode) == 0) {
        return 1;
    } else {
        return 0;
    }
}

// stackoverflow code
void naviga_cartella(char *folder_name, TaskQueue_t *safe_queue) {
    DIR *cartella;
    if((cartella = opendir(folder_name)) == NULL) {
        fprintf(stderr, "FATAL ERROR: navigazione della directory.\n");
        exit(EXIT_FAILURE);
    } else {
        char sub_cartella[MAX_PATH_LENGTH];
        errno = 0;
        struct dirent *file;
        
        // Comincio ad iterare
        while((file = readdir(cartella)) != NULL && (errno == 0)) {
            if(strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
                continue;
            } else {
                strncpy(sub_cartella, folder_name, strlen(folder_name)+1);
                strncat(sub_cartella, "/", 2);
                strncat(sub_cartella, file->d_name, strlen(file->d_name)+1);
                if(is_regular_folder(sub_cartella) == 0) {
                    naviga_cartella(sub_cartella, safe_queue);
                } else {
                    if(is_regular_file(sub_cartella) == 0) {
                        // Aggiungiere alla coda
                        if(pushPool(safe_queue, sub_cartella) == -1) {
                            fprintf(stderr, "FATAL ERROR: inserimento elementi cartella nella pool.\n");
                        }
                    }
                }
            }
        }
        if(errno != 0) {
            perror("Readdir");
        } else {
            closedir(cartella);
        }
    }
}

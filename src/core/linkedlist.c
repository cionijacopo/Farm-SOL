/*
    Created by Jacopo Cioni
    FarmProject - SOL
    File sorgente contenente il funzionamento della lista finale di elementi.
*/

#include"../../includes/linkedlist.h"
#include"../../includes/utils.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FinalNode_t *newFinalNode(char info[], long int ris) {
    // Alloco lo spazio per il nodo
    FinalNode_t *ptr = (FinalNode_t *)s_malloc(sizeof(FinalNode_t));
    // Alloco lo spazio per la stringa
    ptr->info = (char *)s_malloc(strlen(info)+1);
    // Copio il valore
    strncpy(ptr->info, info, strlen(info)+1);
    ptr->ris = ris;
    ptr->next = NULL;
    return ptr;
}

FinalNode_t *orderInsert(FinalNode_t *final_list, char info[], long int ris) {
    if(final_list == NULL || final_list->ris >= ris) {
        FinalNode_t *temp = newFinalNode(info, ris);
        temp->next = final_list;
        final_list = temp;
        return final_list;
    } else {
        final_list->next = orderInsert(final_list->next, info, ris);
    }
    return final_list;
}

FinalNode_t *deleteFinalList(FinalNode_t *final_list) {
    if(final_list != NULL) {
        deleteFinalList(final_list->next);
        free(final_list->info);
        free(final_list);
    } else {
        return NULL;
    }
    return NULL;
}

void printFinalList(FinalNode_t *final_list) {
    if(final_list != NULL) {
        fprintf(stdout, "%ld %s\n", final_list->ris, final_list->info);
        printFinalList(final_list->next);
    }
}

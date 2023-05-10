/*
    Created by Jacopo Cioni
    FarmProject - SOL
*/

#include"../../includes/linkedlist.h"
#include"../../includes/utils.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

Node_t *newNode(char *info) {
    // Alloco lo spazio per il nodo
    Node_t *ptr = (Node_t *)s_malloc(sizeof(Node_t));
    // Alloco lo spazio per la stringa
    ptr->info = (char *)s_malloc(strlen(info)+1);
    // Copio il valore
    strncpy(ptr->info, info, strlen(info)+1);
    ptr->next = NULL;
    return ptr;
}

FinalNode_t *newFinalNode(char *info, long int ris) {
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

Node_t *postInsert(Node_t *lista, char *info) {
    if(lista == NULL) {
        return newNode(info);
    } else {
        lista->next = postInsert(lista->next, info);
    }
    return lista;
}

FinalNode_t *insertFinalNode(FinalNode_t *final_list, char *info, long int ris) {
    if(final_list == NULL) {
        return newFinalNode(info, ris);
    } else {
        FinalNode_t *current = final_list;
        FinalNode_t *new = newFinalNode(info, ris);
        // Controllo se lo devo inserire direttamente in testa
        if(current->ris >= new->ris) {
            new->next = current;
            final_list = new;
            return final_list;
        }
        // Scorro fino a quando non trovo la posizione giusta
        while(current->next != NULL && current->next->ris <= new->ris) {
            current = current->next;
        }
        new->next = current->next;
        current->next = new;
        return final_list;
    }
}

Node_t *deleteList(Node_t *lista) {
    if(lista != NULL) {
        deleteList(lista->next);
        free(lista->info);
        free(lista);
    } else {
        return NULL;
    }
}

FinalNode_t *deleteFinalList(FinalNode_t *final_list) {
    if(final_list != NULL) {
        deleteFinalList(final_list->next);
        free(final_list->info);
        free(final_list);
    } else {
        return NULL;
    }
}

void printFinalList(FinalNode_t *final_list) {
    if(final_list != NULL) {
        fprintf(stdout, "%ld %s\n", final_list->ris, final_list->info);
        printFinalList(final_list->next);
    }
}

int countNodes(Node_t *lista) {
    if(lista == NULL) {
        return 0;
    } else {
        return 1 + countNodes(lista->next);
    }
}

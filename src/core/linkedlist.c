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

Node_t *postInsert(Node_t *lista, char *info) {
    if(lista == NULL) {
        return newNode(info);
    } else {
        lista->next = postInsert(lista->next, info);
    }
    return lista;
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

int countNodes(Node_t *lista) {
    if(lista == NULL) {
        return 0;
    } else {
        return 1 + countNodes(lista->next);
    }
}
/*
    Created by Jacopo Cioni
    farmProject - SOL
    Header contenente i metodi per la gestione della linkedList.
*/
#if !defined(LINKEDLIST_H_)
#define LINKEDLIST_H_
#include<stdio.h>
#include<stdlib.h>

typedef struct finalnode {
    // nome del file
    char *info;
    // Risultato
    long int ris;
    // Puntatore al nodo successivo
    struct finalnode *next;
} FinalNode_t;

/**
 * @function newFinalNode
 * @brief funzione che crea un nuovo nodo
 * @param info elemento del nodo
 * @param ris elemento del nodo
 * @return puntatore al nodo
*/
FinalNode_t *newFinalNode(char info[], long int ris);

/**
 * @function orderInsert
 * @brief funzione che fa la insert di un nuovo FinalNode in modo ordinato
 * @param final_list lista a cui aggiungere il nodo
 * @param info elemento del nodo
 * @param ris elemento del nodo
 * @return puntatore alla lista mdoficata
*/
FinalNode_t *orderInsert(FinalNode_t *FinalNode_t, char info[], long int ris);

/**
 * @function deleteFinalList
 * @brief funzione che elimina la lista finale
 * @param final_list da rimuovere
 * @return NULL se la lista è stata distrutta correttamente
*/
FinalNode_t *deleteFinalList(FinalNode_t *final_list);

/**
 * @function printFinalList
 * @brief funzione che stampa la lista
 * @param final_list lista da stampare
*/
void printFinalList(FinalNode_t *final_list);

#endif

#if !defined(LINKEDLIST_H_)
#define LINKEDLIST_H_

typedef struct node {
    // Informazione
    char *info; 
    // Puntatore al prossimo nodo
    struct node *next;
} Node_t;

typedef struct finalnode {
    // nome del file
    char *info;
    // Risultato
    long int ris;
    // Puntatore al nodo successivo
    struct finalnode *next;
} FinalNode_t;

/**
 * @function newNode
 * @brief funzione che crea un nuovo nodo
 * @param info elemento del nodo
 * @return puntatore al nodo
*/
Node_t *newNode(char *info);

/**
 * @function newFinalNode
 * @brief funzione che crea un nuovo nodo
 * @param info elemento del nodo
 * @param ris elemento del nodo
 * @return puntatore al nodo
*/
FinalNode_t *newFinalNode(char *info, long int ris);

/**
 * @function postInsert
 * @brief funzione che posiziona un nuovo elemento alla fine della lista
 * @param lista lista a cui aggiungere il nodo 
 * @param info elemento del nodo
 * @return puntatore alla lista modificata
*/
Node_t *postInsert(Node_t *lista, char *info);

/**
 * @function insertFinalNode
 * @brief funzione che fa la inser di un nuovo FinalNode in modo ordinato
 * @param final_list lista a cui aggiungere il nodo
 * @param info elemento del nodo
 * @param ris elemento del nodo
 * @return puntatore alla lista mdoficata
*/
FinalNode_t *insertFinalNode(FinalNode_t *final_list, char *info, long int ris);

/**
 * @functio deleteList
 * @brief funzione che elimina la lista
 * @param lista da rimuovere
 * @return NULL se la lista è stata distrutta correttamente
*/
Node_t *deleteList(Node_t *lista);

/**
 * @function deleteFinalList
 * @brief funzione che elimina la lista finale
 * @param final_list da rimuovere
 * @return NULL se la lista è stata distrutta correttamente
*/
FinalNode_t *deleteFinalList(FinalNode_t *final_list);

void printList(Node_t *lista);

/**
 * @function printFinalList
 * @brief funzione che stampa la lista
 * @param final_list lista da stampare
*/
void printFinalList(FinalNode_t *final_list);

/**
 * @function countNodes
 * @brief conta i nodi presenti in una lista
 * @param lista da contare
 * @return numero di nodi presenti nella lista
*/
int countNodes(Node_t *lista);

#endif

#if !defined(LINKEDLIST_H_)
#define LINKEDLIST_H_

typedef struct node {
    // Informazione
    char *info; 
    // Puntatore al prossimo nodo
    struct node *next;
} Node_t;

/**
 * @function newNode
 * @brief funzione che crea un nuovo nodo
 * @param info elemento del nodo
 * @return puntatore al nodo
*/
Node_t *newNode(char *info);

/**
 * @function postInsert
 * @brief funzione che posiziona un nuovo elemento alla fine della lista
 * @param lista lista a cui aggiungere il nodo 
 * @param info elemento del nodo
 * @return puntatore alla lista modificata
*/
Node_t *postInsert(Node_t *lista, char *info);

/**
 * @functio deleteList
 * @brief funzione che elimina la lista
 * @param lista da rimuovere
 * @return NULL se la lista è stata distrutta correttamente
*/
Node_t *deleteList(Node_t *lista);

/**
 * @function countNodes
 * @brief conta i nodi presenti in una lista
 * @param lista da contare
 * @return numero di nodi presenti nella lista
*/
int countNodes(Node_t *lista);

#endif

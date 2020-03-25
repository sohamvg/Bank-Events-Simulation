#include "LinkedList.h"
#include <stdlib.h>
#include <string.h> // for memcpy

Node* new_node(void *data, int data_size)
{
    Node *node = malloc(sizeof(Node));
    node->data = malloc(data_size);
    node->next = NULL;
    memcpy(node->data, data, data_size);

    return node;
}

LinkedList *new_list(int data_size)
{
    LinkedList *list = malloc(sizeof(LinkedList));
    list->data_size = data_size;
    list->length = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void append(LinkedList *list, void *data)
{
    Node *node = new_node(data, list->data_size);

    if (list->length == 0)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;
        list->tail = node;
    }
    
    list->length++;
}
#include "LinkedList.h"
#include <stdlib.h>
#include <string.h> // for memcpy
#include <stdio.h>

ll_Node *ll_new_node(void *data, int data_size)
{
    ll_Node *node = malloc(sizeof(ll_Node));
    node->data = malloc(data_size);
    node->next = NULL;
    memcpy(node->data, data, data_size);

    return node;
}

LinkedList *ll_new_list(int data_size)
{
    LinkedList *list = malloc(sizeof(LinkedList));
    list->data_size = data_size;
    list->length = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void ll_append(LinkedList *list, void *data)
{
    ll_Node *node = ll_new_node(data, list->data_size);

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

void *ll_get_head(LinkedList *list)
{
    if (list->head == NULL)
    {
        perror("List is empty");
        exit(EXIT_FAILURE);
    }

    return list->head->data;
}

void ll_remove_head(LinkedList *list)
{
    if (list->head == NULL)
    {
        perror("List is empty");
        exit(EXIT_FAILURE);
    }

    ll_Node *temp = list->head;
    list->head = list->head->next;
    free(temp);
    list->length--;
}

bool ll_is_empty(LinkedList *list)
{
    return list->length == 0;
}
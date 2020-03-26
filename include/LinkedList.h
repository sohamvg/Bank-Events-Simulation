#ifndef _LIST_H_
#define _LIST_H_

#include <stdbool.h>

typedef struct ll_Node
{
    void *data;
    struct ll_Node *next;
} ll_Node;

typedef struct LinkedList
{
    int length;
    int data_size;
    ll_Node *head;
    ll_Node *tail;
} LinkedList;

/* Returns a new list which can store element of data_size */
LinkedList *ll_new_list(int data_size);

/* Add new node with data to the end of the list */
void ll_append(LinkedList *list, void *data);

/* Get element at head */
void *ll_get_head(LinkedList *list);

/* Remove element at head of list */
void ll_remove_head(LinkedList *list);

/* Check whether list is empty or not */
bool ll_is_empty(LinkedList *list);

#endif
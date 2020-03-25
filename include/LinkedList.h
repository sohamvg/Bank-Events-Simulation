#ifndef _LIST_H_
#define _LIST_H_

typedef struct Node
{
    void *data;
    struct Node *next;
} Node;

typedef struct LinkedList
{
    int length;
    int data_size;
    Node *head;
    Node *tail;
} LinkedList;

/* Returns a new list which can store element of data_size */
LinkedList *new_list(int data_size);

/* Add new node with data to the end of the list */
void append(LinkedList *list, void *data);

#endif
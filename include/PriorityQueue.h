#ifndef _PRIORITYQUEUE_H_
#define _PRIORITYQUEUE_H_

#include <stdbool.h>

typedef struct Node
{
    void *data;
    int priority;
    struct Node *next;
} Node;

typedef struct PriorityQueue
{
    int length;
    int data_size;
    Node *front;
    Node *rear;
} PriorityQueue;

/* Returns a new queue which can store element of data_size */
PriorityQueue *new_queue(int data_size);

/* Add element at the end of the queue */
void enqueue(PriorityQueue *queue, void *data, int priority);

/* Remove the element at front */
void dequeue(PriorityQueue *queue);

/* Get element at fromt */
void *get_front(PriorityQueue *queue);

/* Check whether queue is empty or not */
bool is_empty(PriorityQueue *queue);

#endif
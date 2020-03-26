#ifndef _PRIORITYQUEUE_H_
#define _PRIORITYQUEUE_H_

#include <stdbool.h>

typedef struct pq_Node
{
    void *data;
    int priority;
    struct pq_Node *next;
} pq_Node;

typedef struct PriorityQueue
{
    int length;
    int data_size;
    pq_Node *front;
    pq_Node *rear;
} PriorityQueue;

/* Returns a new queue which can store element of data_size */
PriorityQueue *pq_new_queue(int data_size);

/* Add element to the queue based on priority */
void pq_enqueue(PriorityQueue *queue, void *data, int priority);

/* Remove the element at front */
void pq_dequeue(PriorityQueue *queue);

/* Get element at front */
void *pq_get_front(PriorityQueue *queue);

/* Check whether queue is empty or not */
bool pq_is_empty(PriorityQueue *queue);

#endif
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "LinkedList.h"
#include <stdbool.h>

/* Create a new queue */
LinkedList *q_new_queue(int data_size);

/* Remove element from front */
void q_dequeue(LinkedList *queue);

/* Add element at end of the queue */
void q_enqueue(LinkedList *queue, void *data);

/* Get element at front */
void *q_get_front(LinkedList *queue);

/* Check whether queue is empty or not */
bool q_is_empty(LinkedList *queue);

#endif
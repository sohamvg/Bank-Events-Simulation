#include "PriorityQueue.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

pq_Node *pq_new_node(void *data, int data_size, int priority)
{
    pq_Node *node = malloc(sizeof(pq_Node));
    node->data = malloc(data_size);
    node->priority = priority;
    node->next = NULL;
    memcpy(node->data, data, data_size);

    return node;
}

PriorityQueue *pq_new_queue(int data_size)
{
    PriorityQueue *queue = malloc(sizeof(PriorityQueue));
    queue->data_size = data_size;
    queue->length = 0;
    queue->front = NULL;
    queue->rear = NULL;

    return queue;
}

void *pq_get_front(PriorityQueue *queue)
{
    if (queue->front == NULL)
    {
        perror("Queue is empty");
        exit(EXIT_FAILURE);
    }

    return queue->front->data;
}

void pq_enqueue(PriorityQueue *queue, void *data, int priority)
{
    pq_Node *temp = pq_new_node(data, queue->data_size, priority);

    if (queue->length == 0)
    {
        queue->front = temp;
    }
    else if (queue->front->priority < priority)
    {
        temp->next = queue->front;
        queue->front = temp;
    }
    else
    {
        pq_Node *curr = queue->front;

        while (curr->next != NULL && curr->next->priority >= priority)
        {
            curr = curr->next;
        }

        temp->next = curr->next;
        curr->next = temp;
    }

    queue->length++;
}

void pq_dequeue(PriorityQueue *queue)
{
    if (queue->front == NULL)
    {
        perror("Queue is empty");
        exit(EXIT_FAILURE);
    }

    pq_Node *temp = queue->front;
    queue->front = queue->front->next;
    free(temp);
    queue->length--;
}

bool pq_is_empty(PriorityQueue *queue)
{
    return queue->length == 0;
}
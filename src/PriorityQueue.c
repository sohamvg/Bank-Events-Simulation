#include "PriorityQueue.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

Node* new_qnode(void *data, int data_size, int priority)
{
    Node *node = malloc(sizeof(Node));
    node->data = malloc(data_size);
    node->priority = priority;
    node->next = NULL;
    memcpy(node->data, data, data_size);

    return node;
}

PriorityQueue *new_queue(int data_size)
{
    PriorityQueue *queue = malloc(sizeof(PriorityQueue));
    queue->data_size = data_size;
    queue->length = 0;
    queue->front = NULL;
    queue->rear = NULL;

    return queue;
}

void *get_front(PriorityQueue *queue)
{
    return queue->front->data;
}

void enqueue(PriorityQueue *queue, void *data, int priority)
{
    Node *temp = new_qnode(data, queue->data_size, priority);

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
        Node *curr = queue->front;

        while (curr->next != NULL && curr->next->priority >= priority)
        {
            curr = curr->next;
        }
           
        temp->next = curr->next;
        curr->next = temp;
    }
    
    queue->length++;
}

void dequeue(PriorityQueue *queue)
{
    Node *temp = queue->front;
    queue->front = queue->front->next;
    free(temp);
    queue->length--;
}

bool is_empty(PriorityQueue *queue)
{
    return queue->length == 0;
}
#include "Queue.h"

LinkedList *q_new_queue(int data_size)
{
    return ll_new_list(data_size);
}

void q_dequeue(LinkedList *queue)
{
    ll_remove_head(queue);
}

void q_enqueue(LinkedList *queue, void *data)
{
    ll_append(queue, data);
}

void *q_get_front(LinkedList *queue)
{
    return ll_get_head(queue);
}

bool q_is_empty(LinkedList *queue)
{
    return ll_is_empty(queue);
}
#include "Teller.h"
#include <stdlib.h>

Teller *new_teller(float idle_time)
{
    Teller *teller = malloc(sizeof(Teller));
    teller->idle_time = idle_time;

    return teller;
}

LinkedList *new_teller_queue()
{
    return q_new_queue(sizeof(Customer));
}

void enqueue_customer(LinkedList *teller_queue, Customer *customer)
{
    q_enqueue(teller_queue, (void *)customer);
}

Customer *get_front_customer(LinkedList *teller_queue)
{
    return (Customer *)q_get_front(teller_queue);
}

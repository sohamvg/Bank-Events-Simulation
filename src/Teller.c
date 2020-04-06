#include "Teller.h"
#include <stdlib.h>

Teller *new_teller(float init_idle_time)
{
    Teller *teller = malloc(sizeof(Teller));
    teller->init_idle_time = init_idle_time;
    teller->total_idle_time = init_idle_time; // Add initial idle time to total idle time
    teller->total_service_time = 0.0;

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

#include "Event.h"
#include <stdlib.h>

Event *new_customer_arrive_event(Customer *customer)
{
    Event *event = malloc(sizeof(Event));
    event->type = CUSTOMER_ARRIVE;
    event->customer = customer;
    event->time = customer->arrival_time;

    return event;
}

Event *new_customer_depart_event(Customer *customer, float depart_time)
{
    Event *event = malloc(sizeof(Event));
    event->type = CUSTOMER_DEPART;
    event->customer = customer;
    event->time = depart_time;

    return event;
}

Event *new_teller_event(Teller *teller, float time)
{
    Event *event = malloc(sizeof(Event));
    event->type = TELLER_EVENT;
    event->time = time;
    event->teller = teller;

    return event;
}

PriorityQueue *new_event_queue()
{
    return pq_new_queue(sizeof(Event));
}

void enqueue_event(PriorityQueue *event_queue, Event *event)
{
    pq_enqueue(event_queue, (void *)event, -(event->time));
}
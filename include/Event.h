#ifndef _EVENT_H_
#define _EVENT_H_

#include "Customer.h"
#include "Teller.h"
#include "PriorityQueue.h"

typedef enum
{
    CUSTOMER_ARRIVE, // customer arrival
    CUSTOMER_DEPART, // customer event and represents the completion of that service
    TELLER_EVENT     // event represents a teller who has either completed serving a customer or has completed an idle time task
} event_type;

typedef int (* Action)(Teller *teller, Customer *customer);

typedef struct Event
{
    float time; // time when the event occurs
    event_type type;
    Customer *customer;
    Teller *teller;
    Action action;
} Event;

/* Creates a new customer arrive event */
Event *new_customer_arrive_event(Customer *customer);

/* Creates a new teller event */
Event *new_teller_event(Teller *teller, float time);

/* Create a new customer depart event */
Event *new_customer_depart_event(Customer *customer, float depart_time);

/* Action for event */
void action(Event *event);

/* Creates a new event queue */
PriorityQueue *new_event_queue();

/* Enqueue event to event_queue */
void enqueue_event(PriorityQueue *event_queue, Event *event);

#endif
#ifndef _TELLER_H_
#define _TELLER_H_

#include "Queue.h"
#include "Customer.h"

#define INIT_IDLE_TIME 10.0   // max idle time before start of service is 600s.
#define SERVICE_IDLE_TIME 3.0 // max idle time after serving a customer is 150s.

typedef struct Teller
{
    float init_idle_time;    // initial idle time
    float total_service_time;
    float total_idle_time;
    LinkedList *teller_queue;
} Teller;

/* Creates a new teller with idle_time */
Teller *new_teller(float init_idle_time);

/* Creates a new teller queue */
LinkedList *new_teller_queue();

/* Enqueue customer to teller_queue */
void enqueue_customer(LinkedList *teller_queue, Customer *customer);

/* Get front customer from queue */
Customer *get_front_customer(LinkedList *teller_queue);

#endif
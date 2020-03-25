#ifndef _EVENT_H_
#define _EVENT_H_

#include "Customer.h"
#include "Teller.h"

typedef enum {
    CUSTOMER_ARRIVE, // customer arrival
    CUSTOMER_DEPART, // customer event and represents the completion of that service
    SERVICE_COMPLETE // teller event representing completion of a service and to look for the next customer (or to idle)
} event_type;

typedef struct Event
{
    float time;
    event_type event;
    Customer *customer;
    Teller *teller;
} Event;

void action(Event *event);

#endif
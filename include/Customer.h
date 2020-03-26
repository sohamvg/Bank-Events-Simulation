#ifndef _CUSTOMER_H_
#define _CUSTOMER_H_

typedef struct Customer
{
    float arrival_time;
    float service_time;
    float depart_time;
} Customer;

/* Creates a new customer with arrival_time */
Customer *new_customer(float arrival_time);

#endif
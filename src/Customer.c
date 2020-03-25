#include "Customer.h"
#include <stdlib.h>

Customer *new_customer(float arrival_time)
{
    Customer *new_customer = malloc(sizeof(Customer));
    new_customer->arrival_time = arrival_time;
    new_customer->service_time = 0;

    return new_customer;
}
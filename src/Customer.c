#include "Customer.h"
#include <stdlib.h>

Customer *new_customer(float arrival_time)
{
    Customer *customer = malloc(sizeof(Customer));
    customer->arrival_time = arrival_time;
    customer->service_time = 0;

    return customer;
}
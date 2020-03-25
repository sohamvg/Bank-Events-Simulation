#include "PriorityQueue.h"
#include "Customer.h"
#include "Event.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char **argv)
{
    if (argc != 5) {
        printf("Enter all arguments\n");
        exit(EXIT_SUCCESS);
    }

    int number_of_customers = atoi(argv[1]);
    int number_of_tellers = atoi(argv[2]);
    float simulation_time = atof(argv[3]);
    float avg_service_time = atof(argv[4]);

    printf("Number of customers: %d\n", number_of_customers);
    printf("Number of tellers: %d\n", number_of_tellers);
    printf("Simulation time: %f\n", simulation_time);
    printf("Average service time: %f\n", avg_service_time);

    /* Creating customers */
    for (int i = 0; i < number_of_customers; i++)
    {
        float arrival_time = simulation_time * rand()/(float)(RAND_MAX);
        Customer *customer = new_customer(arrival_time);
    }

    /* Creating tellers */
    for (int i = 0; i < number_of_tellers; i++)
    {
        float idle_time = MAX_IDLE_TIME * rand()/(float)(RAND_MAX);
        Teller *teller = new_teller(idle_time);
    }
    

    // PriorityQueue *l = new_queue(sizeof(int));

    // int a = 4;
    // int b = 53;
    // int c = 6;
    // void *p = &a;

    // printf("length: %d\n", l->length);

    // enqueue(l, &a, a);
    // enqueue(l, &b, b);
    // enqueue(l, &c, c);

    // printf("length: %d\n", l->length);

    // while (!is_empty(l))
    // {
    //     printf("val: %d\n", *(int *)get_front(l));
    //     dequeue(l);
    // }

    return 0;
}
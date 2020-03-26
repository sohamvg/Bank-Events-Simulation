#include "PriorityQueue.h"
#include "Queue.h"
#include "Customer.h"
#include "Teller.h"
#include "Event.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char **argv)
{
    if (argc != 5)
    {
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

    float simulation_clock = 0.0; // Current time of simulation

    PriorityQueue *event_queue = new_event_queue(); // Event queue
    LinkedList *teller_queue = new_teller_queue();  // Teller queue

    /* Creating customers */
    for (int i = 0; i < number_of_customers; i++)
    {
        float arrival_time = simulation_time * rand() / (float)(RAND_MAX);
        Customer *customer = new_customer(arrival_time);
        Event *arrival_event = new_customer_arrive_event(customer);
        enqueue_event(event_queue, arrival_event);

        printf("customer arrive time: %f\n", arrival_time);
    }

    /* Creating tellers */
    for (int i = 0; i < number_of_tellers; i++)
    {
        float idle_time = INIT_IDLE_TIME * rand() / (float)(RAND_MAX);
        Teller *teller = new_teller(idle_time);
        Event *teller_idle_event = new_teller_event(teller, idle_time);
        enqueue_event(event_queue, teller_idle_event);
    }

    /* Running events */
    while (!pq_is_empty(event_queue))
    {
        printf("---------------events----------------\n");
        Event *event = (Event *)pq_get_front(event_queue);

        simulation_clock = event->time;
        printf("time: %f\n", simulation_clock);

        if (simulation_clock > simulation_time)
        {
            printf("Simulation time over \n");
            break;
        }

        if (event->type == CUSTOMER_ARRIVE)
        {
            printf("Customer Arrive event\n");

            if (event->customer == NULL)
            {
                perror("Customer arrive event has no customer");
                exit(EXIT_FAILURE);
            }
            enqueue_customer(teller_queue, event->customer);

            printf("Current teller queue length: %d\n", teller_queue->length);
        }
        else if (event->type == CUSTOMER_DEPART)
        {
            /* code */
        }
        else if (event->type == TELLER_EVENT)
        {
            printf("Teller event\n");

            if (event->teller == NULL)
            {
                perror("Teller event has no teller");
                exit(EXIT_FAILURE);
            }
            // TODO: gather stats of teller

            if (q_is_empty(teller_queue))
            {
                float idle_time = SERVICE_IDLE_TIME * rand() / (float)(RAND_MAX);
                Event *teller_idle_event = new_teller_event(event->teller, simulation_clock + idle_time);
                enqueue_event(event_queue, teller_idle_event);

                printf("Empty queue\n");
                printf("idle time: %f\t", idle_time);
                printf("new time: %f\n", simulation_clock + idle_time);
            }
            else
            {
                Customer *customer = get_front_customer(teller_queue);
                q_dequeue(teller_queue);

                float service_time = 2 * avg_service_time * rand() / (float)(RAND_MAX);
                customer->service_time = service_time;

                Event *customer_depart_event = new_customer_depart_event(customer, simulation_clock + service_time);
                Event *teller_service_complete_event = new_teller_event(event->teller, simulation_clock + service_time);

                enqueue_event(event_queue, teller_service_complete_event);
                enqueue_event(event_queue, customer_depart_event);

                printf("Not empty queue\n");
                printf("service time: %f\t", service_time);
                printf("new time: %f\n", simulation_clock + service_time);
            }
        }
        else
        {
            perror("No matching event type");
            exit(EXIT_FAILURE);
        }

        pq_dequeue(event_queue);
        printf("-------------------------------\n");
    }

    return 0;
}
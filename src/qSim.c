#include "PriorityQueue.h"
#include "Queue.h"
#include "Customer.h"
#include "Teller.h"
#include "Event.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

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

    Teller *tellers[number_of_tellers];       // array of all tellers
    Customer *customers[number_of_customers]; // array of all customers

    /* Creating customers */
    for (int i = 0; i < number_of_customers; i++)
    {
        float arrival_time = simulation_time * rand() / (float)(RAND_MAX);
        Customer *customer = new_customer(arrival_time);
        customers[i] = customer;
        printf("customer arrive time: %f\n", arrival_time);
    }

    /* Creating tellers */
    for (int i = 0; i < number_of_tellers; i++)
    {
        float idle_time = INIT_IDLE_TIME * rand() / (float)(RAND_MAX);
        Teller *teller = new_teller(idle_time);
        tellers[i] = teller;
        printf("teller idle time: %f\n", tellers[i]->idle_time);
    }

    for (int sim_count = 0; sim_count < 2; sim_count++)
    {
        PriorityQueue *event_queue = new_event_queue(); // Event queue // TODO: free queues
        LinkedList *teller_queue = new_teller_queue();  // Teller queue

        /* Creating customer arrival events */
        for (int i = 0; i < number_of_customers; i++)
        {
            Event *arrival_event = new_customer_arrive_event(customers[i]);
            enqueue_event(event_queue, arrival_event);
        }

        /* Creating teller events */
        for (int i = 0; i < number_of_tellers; i++)
        {
            tellers[i]->total_service_time = 0.0;
            Event *teller_idle_event = new_teller_event(tellers[i], tellers[i]->idle_time);
            enqueue_event(event_queue, teller_idle_event);
        }

        /* Variables for stats */
        float simulation_clock = 0.0;          // Current time of simulation
        int total_customer_served = 0;         // total customers served can be different from total customers arriving
        float total_time_served = 0.0;         // service time
        float total_time_spent = 0.0;          // depart time - arriva time
        float time_spent[number_of_customers]; // for standard deviation
        float max_wait_time = 0.0;

        /* Running events */
        while (!pq_is_empty(event_queue))
        {
            printf("---------------events----------------\n");
            Event *event = (Event *)pq_get_front(event_queue);
            pq_dequeue(event_queue);

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
                if (event->customer == NULL)
                {
                    perror("Customer depart event has no customer");
                    exit(EXIT_FAILURE);
                }

                time_spent[total_customer_served] = event->customer->depart_time - event->customer->arrival_time;
                total_time_spent = total_time_spent + time_spent[total_customer_served];
                total_time_served = total_time_served + event->customer->service_time;

                float wait_time = time_spent[total_customer_served] - event->customer->service_time;
                if (wait_time > max_wait_time)
                {
                    max_wait_time = wait_time;
                }
                total_customer_served++;

                printf("customer left the bank\n");
                free(event);
            }
            else if (event->type == TELLER_EVENT)
            {
                printf("Teller event\n");

                if (event->teller == NULL)
                {
                    perror("Teller event has no teller");
                    exit(EXIT_FAILURE);
                }

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
                    float depart_time = service_time + simulation_clock;
                    customer->service_time = service_time;
                    customer->depart_time = depart_time;

                    event->teller->total_service_time += service_time;

                    Event *customer_depart_event = new_customer_depart_event(customer, depart_time);
                    Event *teller_service_complete_event = new_teller_event(event->teller, depart_time);

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

            printf("-------------------------------\n");
        }

        float avg_time_spent = total_time_spent / total_customer_served;
        float sq_diff_time_spent = 0.0;

        for (int i = 0; i < total_customer_served; i++)
        {
            sq_diff_time_spent = sq_diff_time_spent + ((time_spent[i] - avg_time_spent) * (time_spent[i] - avg_time_spent));
        }

        float sd_time_spent = sqrtf(sq_diff_time_spent / total_customer_served);

        printf("------------Statistics-------------\n");
        printf("total customers served: %d\n", total_customer_served);
        printf("total time required to serve all customers: %f\n", total_time_served);
        printf("Number of tellers: %d\n", number_of_tellers);
        printf("Type of queuing: common\n");
        printf("Average time a customer spent in the bank: %f\n", avg_time_spent);
        printf("Standard deviation: %f\n", sd_time_spent);
        printf("Maximum wait time from the time a customer arrives to the time he/she is seen by a teller: %f\n", max_wait_time);
        printf("Total amount of teller service time and total amount of teller idle time:\n");
        for (int i = 0; i < number_of_tellers; i++)
        {
            printf("teller %d\t", i + 1);
            printf("service time: %f\t\t", tellers[i]->total_service_time);
            printf("idle time: %f\n", simulation_time - tellers[i]->total_service_time);
        }

        /* Free events */
        while (!pq_is_empty(event_queue))
        {
            pq_dequeue(event_queue);    
        }
        free(event_queue);

        while (!q_is_empty(teller_queue))
        {
            q_dequeue(teller_queue);
        }
        free(teller_queue);
    }

    /* Free customers and tellers */
    for (int i = 0; i < number_of_customers; i++)
    {
        free(customers[i]);
    }
    for (int i = 0; i < number_of_tellers; i++)
    {
        free(tellers[i]);
    }
    

    return 0;
}
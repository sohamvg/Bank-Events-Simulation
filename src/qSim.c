#include "PriorityQueue.h"
#include "Queue.h"
#include "Customer.h"
#include "Teller.h"
#include "Event.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Get shortest teller queue */
LinkedList *get_shortest_teller_queue(Teller *tellers[], int number_of_tellers, int number_of_customers)
{
    int min_length = number_of_customers + 1;
    int count = 0;

    for (int i = 0; i < number_of_tellers; i++)
    {
        if (tellers[i]->teller_queue->length == min_length)
        {
            count++;
        }

        if (tellers[i]->teller_queue->length < min_length)
        {
            min_length = tellers[i]->teller_queue->length;
            count = 1;
        }
    }

    int n = (rand() % count) + 1; // random number between 1 to count
    int j = 1;

    LinkedList *teller_queue;

    for (int i = 0; i < number_of_tellers; i++)
    {
        if (tellers[i]->teller_queue->length == min_length)
        {
            if (j == n)
            {
                teller_queue = tellers[i]->teller_queue;
            }
            j++;
        }
    }
    return teller_queue;
};

/* teller serving the customer */
void serve_customer(Teller *teller, Customer *customer, PriorityQueue *event_queue, float avg_service_time, float simulation_clock, FILE *file)
{
    float service_time = 2 * avg_service_time * rand() / (float)(RAND_MAX);
    float depart_time = service_time + simulation_clock;
    customer->service_time = service_time;
    customer->depart_time = depart_time;

    teller->total_service_time = teller->total_service_time + service_time;

    Event *customer_depart_event = new_customer_depart_event(customer, depart_time);
    Event *teller_service_complete_event = new_teller_event(teller, depart_time);

    enqueue_event(event_queue, teller_service_complete_event);
    enqueue_event(event_queue, customer_depart_event);

    fprintf(file, "Not empty queue\n");
    fprintf(file, "service time: %f\t", service_time);
    fprintf(file, "new time: %f\n", simulation_clock + service_time);
}

/* Teller idle event */
void set_to_idle(Teller *teller, PriorityQueue *event_queue, float simulation_clock, FILE *file)
{
    float idle_time = SERVICE_IDLE_TIME * rand() / (float)(RAND_MAX);
    teller->total_idle_time = teller->total_idle_time + idle_time;

    Event *teller_idle_event = new_teller_event(teller, simulation_clock + idle_time);
    enqueue_event(event_queue, teller_idle_event);

    fprintf(file, "Empty queue\n");
    fprintf(file, "idle time: %f\t", idle_time);
    fprintf(file, "new time: %f\n", simulation_clock + idle_time);
}

/* Run simulation
    mode = 1 for simulation single teller queue (plotting graph)
    mode = 2 for simulating both single and then multiple teller queue
 */
float run_simulation(int number_of_customers, int number_of_tellers, float simulation_time, float avg_service_time, int mode)
{
    FILE *file = fopen("output/event_logs.txt", "w");
    if (file == NULL)
    {
        printf("Could not open log file. Continuing...\n");
    }

    Teller *tellers[number_of_tellers];       // array of all tellers
    Customer *customers[number_of_customers]; // array of all customers

    /* Creating customers */
    for (int i = 0; i < number_of_customers; i++)
    {
        float arrival_time = simulation_time * rand() / (float)(RAND_MAX);
        Customer *customer = new_customer(arrival_time);
        customers[i] = customer;
        fprintf(file, "customer arrive time: %f\n", arrival_time);
    }

    /* Creating tellers */
    for (int i = 0; i < number_of_tellers; i++)
    {
        float idle_time = INIT_IDLE_TIME * rand() / (float)(RAND_MAX);
        Teller *teller = new_teller(idle_time);
        tellers[i] = teller;
        fprintf(file, "teller idle time: %f\n", tellers[i]->init_idle_time);
    }

    float avg_time_spent_1; // average amount of time a customer spent in bank with single teller queue

    /* Running simulations */
    for (int sim_count = 0; sim_count < mode; sim_count++)
    {
        PriorityQueue *event_queue = new_event_queue(); // Event queue

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
            Event *teller_idle_event = new_teller_event(tellers[i], tellers[i]->init_idle_time);
            enqueue_event(event_queue, teller_idle_event);
        }

        int event_counter = 0;

        /* Variables for stats */
        float simulation_clock = 0.0;          // Current time of simulation
        int total_customer_served = 0;         // total customers served can be different from total customers arriving
        float total_time_served = 0.0;         // service time
        float total_time_spent = 0.0;          // depart time - arrival time
        float time_spent[number_of_customers]; // for standard deviation
        float max_wait_time = 0.0;

        if (sim_count == 0) // Single teller queue
        {
            LinkedList *teller_queue = new_teller_queue(); // Teller queue

            /* Running events */
            while (!pq_is_empty(event_queue))
            {
                event_counter++;

                fprintf(file, "---------------event %d ----------------\n", event_counter);
                fprintf(file, "event queue len: %d\n", event_queue->length);
                Event *event = (Event *)pq_get_front(event_queue);
                pq_dequeue(event_queue);

                simulation_clock = event->time;
                fprintf(file, "time: %f\n", simulation_clock);

                if (event->type == CUSTOMER_ARRIVE)
                {
                    fprintf(file, "Customer Arrive event\n");

                    if (event->customer == NULL)
                    {
                        perror("Customer arrive event has no customer");
                        exit(EXIT_FAILURE);
                    }

                    enqueue_customer(teller_queue, event->customer);

                    fprintf(file, "Current teller queue length: %d\n", teller_queue->length);
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

                    fprintf(file, "customer left the bank\n");
                    free(event);
                }
                else if (event->type == TELLER_EVENT)
                {
                    fprintf(file, "Teller event\n");

                    if (event->teller == NULL)
                    {
                        perror("Teller event has no teller");
                        exit(EXIT_FAILURE);
                    }

                    if (q_is_empty(teller_queue))
                    {
                        if (simulation_clock < simulation_time)
                        {
                            set_to_idle(event->teller, event_queue, simulation_clock, file);
                        }
                    }
                    else
                    {
                        Customer *customer = get_front_customer(teller_queue);
                        q_dequeue(teller_queue);

                        serve_customer(event->teller, customer, event_queue, avg_service_time, simulation_clock, file);
                    }
                }
                else
                {
                    perror("No matching event type");
                    exit(EXIT_FAILURE);
                }

                fprintf(file, "-------------------------------\n");
            }

            /* Free teller queue */
            while (!q_is_empty(teller_queue))
            {
                q_dequeue(teller_queue);
            }
            free(teller_queue);
        }
        else // Multiple teller queues
        {
            /* Creating multiple teller queues */
            for (int i = 0; i < number_of_tellers; i++)
            {
                tellers[i]->teller_queue = new_teller_queue();
            }

            /* Running events */
            while (!pq_is_empty(event_queue))
            {
                event_counter++;

                fprintf(file, "---------------event %d ----------------\n", event_counter);
                fprintf(file, "event queue len: %d\n", event_queue->length);
                Event *event = (Event *)pq_get_front(event_queue);
                pq_dequeue(event_queue);

                simulation_clock = event->time;
                fprintf(file, "time: %f\n", simulation_clock);

                if (event->type == CUSTOMER_ARRIVE)
                {
                    fprintf(file, "Customer Arrive event\n");

                    if (event->customer == NULL)
                    {
                        perror("Customer arrive event has no customer");
                        exit(EXIT_FAILURE);
                    }

                    // select shortest teller queue
                    LinkedList *teller_queue = get_shortest_teller_queue(tellers, number_of_tellers, number_of_customers);

                    enqueue_customer(teller_queue, event->customer);

                    fprintf(file, "Current teller queue length: %d\n", teller_queue->length);
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

                    fprintf(file, "customer left the bank\n");
                    free(event);
                }
                else if (event->type == TELLER_EVENT)
                {
                    fprintf(file, "Teller event\n");

                    if (event->teller == NULL)
                    {
                        perror("Teller event has no teller");
                        exit(EXIT_FAILURE);
                    }

                    if (q_is_empty(event->teller->teller_queue))
                    {
                        int non_empty_queue_count = 0;

                        for (int i = 0; i < number_of_tellers; i++)
                        {
                            if (tellers[i]->teller_queue->length != 0)
                            {
                                non_empty_queue_count++;
                            }
                        }

                        if (non_empty_queue_count == 0)
                        {
                            if (simulation_clock < simulation_time)
                            {
                                set_to_idle(event->teller, event_queue, simulation_clock, file);
                            }
                        }
                        else
                        {
                            Customer *customer;
                            int n = (rand() % non_empty_queue_count) + 1; // randum number between 1 and non_empty_queue_count
                            int j = 1;

                            for (int i = 0; i < number_of_tellers; i++)
                            {
                                if (tellers[i]->teller_queue->length != 0)
                                {
                                    if (j == n)
                                    {
                                        customer = get_front_customer(tellers[i]->teller_queue);
                                        q_dequeue(tellers[i]->teller_queue);
                                    }
                                    j++;
                                }
                            }

                            serve_customer(event->teller, customer, event_queue, avg_service_time, simulation_clock, file);
                        }
                    }
                    else
                    {
                        Customer *customer = get_front_customer(event->teller->teller_queue);
                        q_dequeue(event->teller->teller_queue);

                        serve_customer(event->teller, customer, event_queue, avg_service_time, simulation_clock, file);
                    }
                }
                else
                {
                    perror("No matching event type");
                    exit(EXIT_FAILURE);
                }

                fprintf(file, "-------------------------------\n");
            }

            /* Free teller queues */
            for (int i = 0; i < number_of_tellers; i++)
            {
                while (!q_is_empty(tellers[i]->teller_queue))
                {
                    q_dequeue(tellers[i]->teller_queue);
                }
                free(tellers[i]->teller_queue);
            }
        }

        /* Stats */
        float avg_time_spent = total_time_spent / total_customer_served;
        float sq_diff_time_spent = 0.0;

        for (int i = 0; i < total_customer_served; i++)
        {
            sq_diff_time_spent = sq_diff_time_spent + ((time_spent[i] - avg_time_spent) * (time_spent[i] - avg_time_spent));
        }

        float sd_time_spent = sqrtf(sq_diff_time_spent / total_customer_served);
        avg_time_spent_1 = avg_time_spent;

        if (mode == 2)
        {
            printf("------------Statistics-------------\n");

            printf("Number of customers: %d\n", number_of_customers);
            printf("Number of tellers: %d\n", number_of_tellers);
            printf("Simulation time: %f\n", simulation_time);
            printf("Average service time: %f\n", avg_service_time);

            printf("total customers served: %d\n", total_customer_served);
            printf("total time required to serve all customers: %f\n", total_time_served);
            printf("Number of tellers: %d\n", number_of_tellers);

            if (sim_count == 0)
            {
                printf("Type of queuing: common\n");
            }
            else
            {
                printf("Type of queuing: multiple teller queues\n");
            }

            printf("Average time a customer spent in the bank: %f\n", avg_time_spent);
            printf("Standard deviation: %f\n", sd_time_spent);
            printf("Maximum wait time from the time a customer arrives to the time he/she is seen by a teller: %f\n", max_wait_time);
            printf("Total amount of teller service time and total amount of teller idle time:\n");
            for (int i = 0; i < number_of_tellers; i++)
            {
                printf("teller %d\t", i + 1);
                printf("service time: %f\t\t", tellers[i]->total_service_time);
                printf("idle time: %f\n", tellers[i]->total_idle_time);
            }

            printf("-------------- Simulation finished -------------\n");
            fprintf(file, "-------------- Simulation finished -------------\n");
        }
    }

    /* Free tellers (customers are freed with teller queues and events are freed with event queue) */
    for (int i = 0; i < number_of_tellers; i++)
    {
        free(tellers[i]);
    }

    fclose(file);

    return avg_time_spent_1;
}

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

    run_simulation(number_of_customers, number_of_tellers, simulation_time, avg_service_time, 2);

    FILE *fgp;
    fgp = fopen("output/plot.txt", "w");
    if (fgp == NULL)
    {
        perror("unable to open file");
        exit(1);
    }

    for (int teller_count = 1; teller_count < 6; teller_count++)
    {
        float avg_time = run_simulation(number_of_customers, teller_count, simulation_time, avg_service_time, 1);
        fprintf(fgp, "%d %f %0.2f\n", teller_count, avg_time, avg_time);
    }

    fclose(fgp);

    /* Plot graph */
    FILE *pipe = popen("gnuplot -persist", "w");
    fprintf(pipe, "set terminal png\n");
    fprintf(pipe, "set output '%s'\n", "output/plot.png");
    fprintf(pipe, "set title 'average time a customer spent in bank versus number of tellers'\n");
    fprintf(pipe, "set xlabel 'tellers'\n");
    fprintf(pipe, "set ylabel 'avg time (seconds)'\n");
    fprintf(pipe, "set style fill solid 1.0\n");
    fprintf(pipe, "%s\n", "set style data histograms");
    fprintf(pipe, "%s\n", "plot 'output/plot.txt' using 1:2 with linespoints notitle, '' using 1:2:3 with labels notitle");
    fclose(pipe);

    return 0;
}
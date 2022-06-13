#include "threads.h"
#include "queue.h"
#include "queues_wrapper.h"
#include "cpu_usage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CPU_USAGE "/proc/stat"
#define BUFFER_SIZE 5000

// czytanie pliku i zapisywanie do bufora jako raw data
void *thread_reader(void *arg)
{
    Queue *q = *(Queue **)arg;
    char usage[BUFFER_SIZE + 1];

    while (true)
    {
        FILE *fd = fopen(CPU_USAGE, "r");

        if (fd != NULL)
        {
            size_t len = fread(usage, sizeof(char), BUFFER_SIZE, fd);
            usage[len] = '\0';
            fclose(fd);
        }

        queue_lock(q);
        if (queue_is_full(q))
        {
            queue_wait_for_consumer(q);
        }

        queue_enqueue(q, &(char *){usage});

        queue_call_consumer(q);
        queue_unlock(q);

        sleep(1);
    }

    return NULL;
}

// odbiór danych i ich analiza
void *thread_analyzer(void *arg)
{
    Queues_Wrapper wrapper = *(Queues_Wrapper *)arg;
    Queue *q = wrapper.queue_in;
    Queue *q_out = wrapper.queue_out;
    while (true)
    {
        char *usage1 = "";
        char *usage2 = "";

        queue_lock(q);

        if (queue_is_empty(q))
        {
            queue_wait_for_producer(q);
        }

        queue_dequeue(q, &usage1);

        CPU_usage *previous = CPU_usage_create(usage1);

        if (queue_is_empty(q))
        {
            queue_wait_for_producer(q);
        }

        queue_dequeue(q, &usage2);

        CPU_usage *current = CPU_usage_create(usage2);

        queue_call_producer(q);
        queue_unlock(q);

        float *results = CPU_usage_calculate(previous, current);

        CPU_usage_destroy(previous);
        CPU_usage_destroy(current);

        queue_lock(q_out);
        if (queue_is_full(q_out))
        {
            queue_wait_for_consumer(q_out);
        }

        queue_enqueue(q_out, &(float *){results});

        queue_call_consumer(q_out);
        queue_unlock(q_out);
    }

    return NULL;
}

void *thread_printer(void *arg)
{
    Queue *q = *(Queue **)arg;

    while (true)
    {
        float *results;
        queue_lock(q);

        if (queue_is_empty(q))
        {
            queue_wait_for_producer(q);
        }

        queue_dequeue(q, &results);

        queue_call_producer(q);
        queue_unlock(q);

        printf("%f\n", results[0]);
    }

    return NULL;
}

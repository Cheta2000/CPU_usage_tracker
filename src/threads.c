#include "threads.h"
#include "queue.h"
#include "queues_wrapper.h"
#include "cpu_usage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>

#define CPU_USAGE "/proc/stat"
#define DEBUG "debug.txt"
#define BUFFER_SIZE 5000

volatile sig_atomic_t done = 0;

void finish_threads()
{
    done = 1;
}

// reading file and inserting to queue as raw data
void *thread_reader(void *arg)
{
    Queues_Wrapper wrapper = *(Queues_Wrapper *)arg;

    Queue *q_out = wrapper.queue_data_out;
    Queue *q_logger = wrapper.queue_logger;
    Queue *q_watchdog = wrapper.queue_watchdog;

    char info[] = "THREAD READER: sent data\n";

    while (!done)
    {
        // first read file
        char *usage = malloc(BUFFER_SIZE + 1);
        FILE *fd = fopen(CPU_USAGE, "r");
        if (fd != NULL)
        {
            const size_t len = fread(usage, sizeof(char), BUFFER_SIZE, fd);
            usage[len] = '\0';

            fclose(fd);
        }

        // wait for space in queue
        queue_lock(q_out);
        if (queue_is_full(q_out))
        {
            queue_wait_for_consumer(q_out);
        }

        // element insertion
        if (!done)
        {
            queue_enqueue(q_out, &(char *){usage});
            queue_call_consumer(q_out);
        }
        // if ending, do not insert element
        else
        {
            free(usage);
        }

        queue_unlock(q_out);

        queue_lock(q_logger);
        if (queue_is_full(q_logger))
        {
            queue_wait_for_consumer(q_logger);
        }

        if (!done)
        {
            queue_enqueue(q_logger, &(char *){info});
            queue_call_consumer(q_logger);
        }

        queue_unlock(q_logger);

        queue_lock(q_watchdog);
        if (!queue_is_full(q_watchdog) && !done)
        {
            queue_enqueue(q_watchdog, &(uint8_t){1});
        }

        queue_unlock(q_watchdog);
    }

    printf("READER EXIT\n");
    // after thread finish we must send signal to possible waiting threads
    queue_lock(q_out);
    queue_call_consumer(q_out);
    queue_unlock(q_out);

    queue_lock(q_logger);
    queue_call_consumer(q_logger);
    queue_unlock(q_logger);

    return NULL;
}

// data receiving from queue and calculations, then sending result to another queue
void *thread_analyzer(void *arg)
{
    Queues_Wrapper wrapper = *(Queues_Wrapper *)arg;

    Queue *q_in = wrapper.queue_data_in;
    Queue *q_out = wrapper.queue_data_out;
    Queue *q_logger = wrapper.queue_logger;
    Queue *q_watchdog = wrapper.queue_watchdog;

    char info[] = "THREAD ANALYZER: calculated CPU usage\n";

    char *current_data = "";

    while (!done)
    {
        queue_lock(q_in);
        if (queue_is_empty(q_in))
        {
            queue_wait_for_producer(q_in);
        }

        if (!done)
        {
            queue_dequeue(q_in, &current_data);

            queue_call_producer(q_in);
            queue_unlock(q_in);

            CPU_data *current = CPU_data_create(current_data);
            CPU_usage *results = CPU_usage_calculate(current);

            CPU_data_destroy(current);
            free(current_data);

            if (results != NULL)
            {
                queue_lock(q_out);
                if (queue_is_full(q_out))
                {
                    queue_wait_for_consumer(q_out);
                }

                if (!done)
                {
                    queue_enqueue(q_out, &(CPU_usage *){results});
                    queue_call_consumer(q_out);
                }
                else
                {
                    free(results);
                }

                queue_unlock(q_out);
            }
        }
        else
        {
            queue_unlock(q_in);
        }

        queue_lock(q_logger);

        if (queue_is_full(q_logger))
        {
            queue_wait_for_consumer(q_logger);
        }

        if (!done)
        {
            queue_enqueue(q_logger, &(char *){info});
            queue_call_consumer(q_logger);
        }

        queue_unlock(q_logger);

        queue_lock(q_watchdog);
        if (!queue_is_full(q_watchdog) && !done)
        {
            queue_enqueue(q_watchdog, &(uint8_t){1});
        }

        queue_unlock(q_watchdog);
    }

    printf("ANALYZER EXIT\n");

    queue_lock(q_in);

    // clearing memory
    while (!queue_is_empty(q_in))
    {
        queue_dequeue(q_in, &current_data);
        free(current_data);
    }

    queue_call_producer(q_in);
    queue_unlock(q_in);

    queue_lock(q_out);
    queue_call_consumer(q_out);
    queue_unlock(q_out);

    queue_lock(q_logger);
    queue_call_consumer(q_logger);
    queue_unlock(q_logger);

    return NULL;
}

// printing received data
void *thread_printer(void *arg)
{
    Queues_Wrapper wrapper = *(Queues_Wrapper *)arg;

    Queue *q_in = wrapper.queue_data_in;
    Queue *q_logger = wrapper.queue_logger;
    Queue *q_watchdog = wrapper.queue_watchdog;

    char info[] = "THREAD PRINTER: printed CPU usage\n";

    CPU_usage *results;

    while (!done)
    {
        queue_lock(q_in);
        if (queue_is_empty(q_in))
        {
            queue_wait_for_producer(q_in);
        }

        if (!done)
        {
            queue_dequeue(q_in, &results);

            queue_call_producer(q_in);
            queue_unlock(q_in);

            CPU_usage_print(results);

            CPU_usage_destroy(results);
        }
        else
        {
            queue_unlock(q_in);
        }

        queue_lock(q_logger);
        if (queue_is_full(q_logger))
        {
            queue_wait_for_consumer(q_logger);
        }

        if (!done)
        {
            queue_enqueue(q_logger, &(char *){info});
            queue_call_consumer(q_logger);
        }

        queue_unlock(q_logger);

        queue_lock(q_watchdog);
        if (!queue_is_full(q_watchdog) && !done)
        {
            queue_enqueue(q_watchdog, &(uint8_t){1});
        }

        queue_unlock(q_watchdog);

        sleep(1);
    }

    printf("PRINTER EXIT\n");

    queue_lock(q_in);
    while (!queue_is_empty(q_in))
    {
        queue_dequeue(q_in, &results);
        free(results);
    }

    queue_call_producer(q_in);
    queue_unlock(q_in);

    queue_lock(q_logger);
    queue_call_consumer(q_logger);
    queue_unlock(q_logger);

    return NULL;
}

// debug logs to file
void *thread_logger(void *arg)
{
    Queue *q = *(Queue **)arg;

    char *log = "";

    FILE *fd = fopen(DEBUG, "w");
    fclose(fd);

    fd = fopen(DEBUG, "a");

    while (!done)
    {

        queue_lock(q);
        if (queue_is_empty(q))
        {
            queue_wait_for_producer(q);
        }

        if (!done)
        {
            queue_dequeue(q, &log);

            queue_call_producer(q);
            queue_unlock(q);

            fprintf(fd, "%s", log);
        }
        else
        {
            queue_unlock(q);
        }
    }

    printf("LOGGER EXIT\n");

    fclose(fd);

    queue_lock(q);
    queue_call_producer(q);
    queue_unlock(q);

    return NULL;
}

// flow control, after 2 second inactivity program ends
void *thread_watchdog(void *arg)
{
    Queue *q = *(Queue **)arg;

    while (!done)
    {
        sleep(2);

        queue_lock(q);
        if (queue_is_empty(q))
        {
            printf("ERROR!\n");
            finish_threads();
        }
        else
        {
            queue_remove_all(q);
        }

        queue_unlock(q);
    }

    printf("WATCHDOG EXIT\n");

    queue_lock(q);
    queue_call_producer(q);
    queue_unlock(q);

    return NULL;
}
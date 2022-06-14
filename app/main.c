#include "threads.h"
#include "cpu_usage.h"
#include "queue.h"
#include "queues_wrapper.h"
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define QUEUE_SIZE 100

extern void finish_threads();

int main(void)
{
    printf("PID: %d\n", getpid());
    signal(SIGINT, finish_threads);
    Queue *q1 = queue_create(QUEUE_SIZE, sizeof(char *));
    Queue *q2 = queue_create(QUEUE_SIZE, sizeof(CPU_usage *));
    Queue *q3 = queue_create(QUEUE_SIZE, sizeof(char *));
    Queue *q4 = queue_create(QUEUE_SIZE, sizeof(uint8_t));
    Queues_Wrapper wrapper_reader = {.queue_data_out = q1, .queue_logger = q3, .queue_watchdog = q4};
    Queues_Wrapper wrapper_analyzer = {.queue_data_in = q1, .queue_data_out = q2, .queue_logger = q3, .queue_watchdog = q4};
    Queues_Wrapper wrapper_printer = {.queue_data_in = q2, .queue_logger = q3, .queue_watchdog = q4};
    pthread_t reader, analyzer, printer, logger, watchdog;
    pthread_create(&reader, NULL, thread_reader, (void *)&wrapper_reader);
    pthread_create(&analyzer, NULL, thread_analyzer, (void *)&wrapper_analyzer);
    pthread_create(&printer, NULL, thread_printer, (void *)&wrapper_printer);
    pthread_create(&logger, NULL, thread_logger, (void *)&q3);
    pthread_create(&watchdog, NULL, thread_watchdog, (void *)&q4);
    pthread_join(reader, NULL);
    pthread_join(analyzer, NULL);
    pthread_join(printer, NULL);
    pthread_join(logger, NULL);
    pthread_join(watchdog, NULL);
    queue_destroy(q1);
    queue_destroy(q2);
    queue_destroy(q3);
    queue_destroy(q4);
}

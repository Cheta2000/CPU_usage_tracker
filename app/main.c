#include "threads.h"
#include "queue.h"
#include "queues_wrapper.h"
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>

#define QUEUE_SIZE 10

Queue *q1;
Queue *q2;

void finish()
{
    queue_destroy(q1);
    queue_destroy(q2);
    printf("\nEXITING\n");
    exit(0);
}

int main(void)
{
    signal(SIGINT, finish);
    q1 = queue_create(QUEUE_SIZE, sizeof(char *));
    q2 = queue_create(QUEUE_SIZE, sizeof(float *));
    Queues_Wrapper wrapper = {q1, q2};
    pthread_t reader, analyzer, printer;
    pthread_create(&reader, NULL, thread_reader, (void *)&q1);
    pthread_create(&analyzer, NULL, thread_analyzer, (void *)&wrapper);
    pthread_create(&printer, NULL, thread_printer, (void *)&q2);
    pthread_join(reader, NULL);
    pthread_join(analyzer, NULL);
    pthread_join(printer, NULL);
}

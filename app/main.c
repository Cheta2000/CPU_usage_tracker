#include "threads.h"
#include "queue.h"
#include "data.h"
#include <stdio.h>
#include <pthread.h>

#define QUEUE_SIZE 10

int main(void)
{
    Queue *q = queue_create(QUEUE_SIZE, sizeof(Data));
    pthread_t tid;
    pthread_create(&tid, NULL, thread_reader, (void *)&q);
    pthread_join(tid, NULL);
    queue_destroy(q);
}

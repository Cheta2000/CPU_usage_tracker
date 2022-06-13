#include <stdbool.h>
#include <stddef.h>

#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue Queue;

Queue *queue_create(const size_t max_elements, const size_t element_size);
void queue_destroy(Queue *q);

bool queue_is_empty(const Queue *q);
bool queue_is_full(const Queue *q);

int queue_enqueue(Queue *restrict q, const void *restrict element);
int queue_dequeue(Queue *restrict q, void *restrict element);

void queue_lock(Queue *q);
void queue_unlock(Queue *q);
void queue_call_producer(Queue *q);
void queue_call_consumer(Queue *q);
void queue_wait_for_producer(Queue *q);
void queue_wait_for_consumer(Queue *q);

#endif

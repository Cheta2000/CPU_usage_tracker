#include <stdbool.h>

#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue Queue;

Queue *queue_create();
void queue_destroy(Queue *q);

bool queue_is_empty(const Queue *q);
bool queue_is_full(const Queue *q);

int queue_enqueue(Queue *restrict q, const void *restrict element);
int queue_dequeue(Queue *restrict q, void *restrict element);

#endif

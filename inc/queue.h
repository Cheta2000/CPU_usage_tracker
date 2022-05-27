#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue Queue;

Queue *queue_create();
void queue_destroy(Queue *q);

int queue_enqueue(Queue *restrict q, const void *restrict element);
int queue_dequeue(Queue *restrict q, void *restrict element);

#endif

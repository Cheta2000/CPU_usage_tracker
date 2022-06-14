#include "queue.h"

// each thread needs more then one queue
typedef struct Queues_Wrapper
{
    Queue *queue_data_in;
    Queue *queue_data_out;
    Queue *queue_logger;
    Queue *queue_watchdog;
} Queues_Wrapper;

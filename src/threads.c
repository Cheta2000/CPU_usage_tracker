#include "threads.h"
#include "queue.h"
#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CPU_USAGE "/proc/stat"
#define BUFFER_SIZE 5000

// czytanie pliku i zapisywanie do bufora jako raw data
void *thread_reader(void *arg)
{
    Queue *q = *(Queue **)arg;
    FILE *fd = fopen(CPU_USAGE, "r");
    char usage[BUFFER_SIZE + 1];
    if (fd != NULL)
    {
        size_t len = fread(usage, sizeof(char), BUFFER_SIZE, fd);
        usage[len] = '\0';
        fclose(fd);
    }
    Data data = {.data_size = strlen(usage), .data_info = usage};
    queue_enqueue(q, &data);
    return NULL;
}

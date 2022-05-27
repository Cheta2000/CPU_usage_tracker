#include <stddef.h>
#ifndef DATA_H
#define DATA_H

// dane o zużyciu procesora trzymane w kolejce
typedef struct Data
{
    char *data_info;
    size_t data_size;
} Data;

#endif

#include <stddef.h>
#ifndef CPU_H
#define CPU_H

// data from /proc/stat
typedef struct CPU_core
{
    size_t user;
    size_t nice;
    size_t system;
    size_t idle;
    size_t iowait;
    size_t irq;
    size_t softirq;
    size_t steal;
} CPU_core;

typedef struct CPU_usage CPU_usage;

typedef struct CPU_data CPU_data;

CPU_data *CPU_data_create(char usage[]);
void CPU_data_destroy(CPU_data *data);

CPU_usage *CPU_usage_calculate(const CPU_data *data);
void CPU_usage_destroy(CPU_usage *usage);

void CPU_usage_print(const CPU_usage *usage);
float CPU_usage_get_core_usage(const CPU_usage *usage, const size_t index);

#endif

#include <stddef.h>
#ifndef CPU_H
#define CPU_H

// potrzebne dane z pliku /proc/stat
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

CPU_usage *CPU_usage_create(char usage[]);
int CPU_usage_destroy(CPU_usage *CPU_usage);

float *CPU_usage_calculate(CPU_usage *previous, CPU_usage *current);

#endif

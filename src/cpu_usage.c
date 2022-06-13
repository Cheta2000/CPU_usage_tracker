#include "cpu_usage.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern size_t count_CPU_cores(char usage[]);

struct CPU_usage
{
    size_t no_of_cores;
    CPU_core cores[];
};

CPU_usage *
CPU_usage_create(char *usage)
{
    char *usage_copy = malloc(strlen(usage) + 1);
    strncpy(usage_copy, usage, strlen(usage) + 1);

    size_t no_of_cores = count_CPU_cores(usage);

    CPU_usage *cpu_usage = calloc(1, sizeof(cpu_usage) + sizeof(CPU_core) * no_of_cores);
    cpu_usage->no_of_cores = no_of_cores;
    const size_t params = 8;
    char *line = strtok(usage, "\n");
    for (size_t i = 0; i < no_of_cores; i++)
    {
        char *lineCopy = malloc(strlen(line) + 1);
        strncpy(lineCopy, line, strlen(line) + 1);
        size_t core_params[params];
        char *token = strtok(line, " ");
        token = strtok(token + strlen(token) + 1, " ");
        size_t j = 0;
        while (token != NULL && j < params)
        {
            core_params[j] = atoi(token);
            token = strtok(token + strlen(token) + 1, " ");
            j++;
        }
        line = strtok(line + strlen(line) + 1, "\n");
        CPU_core cpu_core = (CPU_core){
            .user = core_params[0],
            .nice = core_params[1],
            .system = core_params[2],
            .idle = core_params[3],
            .iowait = core_params[4],
            .irq = core_params[5],
            .softirq = core_params[6],
            .steal = core_params[7]};

        memcpy(&cpu_usage->cores[i], &cpu_core, sizeof(CPU_core));
        free(lineCopy);
    }

    free(usage_copy);

    return cpu_usage;
}

int CPU_usage_destroy(CPU_usage *CPU_usage)
{
    free(CPU_usage);
    return 1;
}

// PrevIdle = previdle + previowait
// Idle = idle + iowait

// PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
// NonIdle = user + nice + system + irq + softirq + steal

// PrevTotal = PrevIdle + PrevNonIdle
// Total = Idle + NonIdle

// # differentiate: actual value minus the previous one
// totald = Total - PrevTotal
// idled = Idle - PrevIdle

// CPU_Percentage = (totald - idled)/totald
float *CPU_usage_calculate(CPU_usage *previous, CPU_usage *current)
{
    float *results = malloc(sizeof(float) * previous->no_of_cores);
    for (size_t i = 0; i < previous->no_of_cores; i++)
    {
        CPU_core previous_core = previous->cores[i];
        CPU_core current_core = current->cores[i];

        size_t prev_idle = previous_core.idle + previous_core.iowait;
        size_t idle = current_core.idle + current_core.iowait;

        size_t prev_nonidle = previous_core.user + previous_core.nice + previous_core.system + previous_core.irq + previous_core.softirq + previous_core.steal;
        size_t nonidle = current_core.user + current_core.nice + current_core.system + current_core.irq + current_core.softirq + current_core.steal;

        size_t prev_total = prev_idle + prev_nonidle;
        size_t total = idle + nonidle;

        size_t totald = total - prev_total;
        size_t idled = idle - prev_idle;

        float usage = 100 * (totald - idled) / (float)totald;
        float rounded = round(100 * usage) / 100;
        results[i] = rounded;
    }
    return results;
}
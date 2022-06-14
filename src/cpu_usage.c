#include "cpu_usage.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern size_t CPU_cores_count(char usage[]);

// info about all cores
struct CPU_data
{
    size_t no_of_cores;
    CPU_core cores[];
};

// result of cpu usage
struct CPU_usage
{
    size_t no_of_cores;
    float core_usage[];
};

// formating usage data to create struct
CPU_data *
CPU_data_create(char usage[const])
{
    if (usage == NULL)
    {
        return NULL;
    }

    char *usage_copy = malloc(strlen(usage) + 1);
    memcpy(usage_copy, usage, strlen(usage) + 1);
    const size_t no_of_cores = CPU_cores_count(usage_copy);

    CPU_data *data = calloc(1, sizeof(data) + sizeof(CPU_core) * no_of_cores);
    data->no_of_cores = no_of_cores;

    const size_t params = 8;
    char *line = strtok(usage_copy, "\n");

    for (size_t i = 0; i < no_of_cores; i++)
    {
        size_t core_params[params];

        char *line_copy = malloc(strlen(line) + 1);
        strncpy(line_copy, line, strlen(line) + 1);
        char *token = strtok(line_copy, " ");
        token = strtok(token + strlen(token) + 1, " ");

        size_t j = 0;
        while (token != NULL && j < params)
        {
            core_params[j] = atoi(token);
            j++;

            token = strtok(token + strlen(token) + 1, " ");
        }
        line = strtok(line + strlen(line) + 1, "\n");

        const CPU_core core = (CPU_core){
            .user = core_params[0],
            .nice = core_params[1],
            .system = core_params[2],
            .idle = core_params[3],
            .iowait = core_params[4],
            .irq = core_params[5],
            .softirq = core_params[6],
            .steal = core_params[7]};

        memcpy(&data->cores[i], &core, sizeof(CPU_core));

        free(line_copy);
    }

    free(usage_copy);

    return data;
}

void CPU_data_destroy(CPU_data *const data)
{
    if (data == NULL)
    {
        return;
    }

    free(data);
}

//  formula to count CPU usage
CPU_usage *CPU_usage_calculate(const CPU_data *const data)
{
    if (data == NULL)
    {
        return NULL;
    }

    float results[data->no_of_cores];

    for (size_t i = 0; i < data->no_of_cores; i++)
    {
        const CPU_core core = data->cores[i];

        const size_t total = core.user + core.nice + core.system + core.irq + core.softirq + core.steal + core.idle + core.iowait;

        const float usage = 100 * (core.idle) / (float)total;
        const float rounded = 100 - round(100 * usage) / 100;
        results[i] = rounded;
    }

    CPU_usage *usage = malloc(sizeof(CPU_usage) + sizeof(float) * data->no_of_cores);
    usage->no_of_cores = data->no_of_cores;
    memcpy(usage->core_usage, results, sizeof(float) * data->no_of_cores);

    return usage;
}

void CPU_usage_destroy(CPU_usage *const usage)
{
    if (usage == NULL)
    {
        return;
    }

    free(usage);
}

void CPU_usage_print(const CPU_usage *const usage)
{
    if (usage == NULL)
    {
        return;
    }

    printf("\nNEW USAGE DATA\nOVERALL: %.2f%%\n", usage->core_usage[0]);
    for (size_t i = 1; i < usage->no_of_cores; i++)
    {
        printf("CORE %ld: %.2f%%\n", i, usage->core_usage[i]);
    }
}

float CPU_usage_get_core_usage(const CPU_usage *const usage, const size_t index)
{
    if (usage == NULL)
    {
        return 0;
    }

    if (index < usage->no_of_cores)
    {
        return usage->core_usage[index];
    }
    else
    {
        return 0;
    }
}
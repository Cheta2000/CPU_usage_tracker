#include <stddef.h>
#include <string.h>
#include <stdio.h>

size_t CPU_cores_count(char usage[const])
{
    size_t counter = 0;
    char *line = strtok(usage, "\n");

    while (line != NULL && strstr(line, "cpu") != NULL)
    {
        counter++;
        line = strtok(NULL, "\n");
    }

    return counter;
}
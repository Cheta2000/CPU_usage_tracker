#include <stddef.h>
#include <string.h>

size_t count_CPU_cores(char usage[])
{
    size_t counter = 0;
    char *line = strtok(usage, "\n");

    while (strstr(line, "cpu") != NULL)
    {
        counter++;
        line = strtok(NULL, "\n");
    }

    return counter;
}
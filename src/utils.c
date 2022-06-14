#include <stddef.h>
#include <string.h>
#include <stdio.h>

size_t CPU_cores_count(char usage[const])
{
    if (usage == NULL)
    {
        return 0;
    }

    size_t counter = 0;
    char *line = strtok(usage, "\n");

    while (line != NULL && strstr(line, "cpu") != NULL)
    {
        counter++;
        line = strtok(NULL, "\n");
    }

    return counter;
}

#include <stdio.h>

extern void queue_main_test();
extern void data_main_test();

int main(void)
{
    printf("Running tests...\n");
    queue_main_test();
    data_main_test();
    printf("PASSED\n");
    return 0;
}

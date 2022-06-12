#include "queue.h"
#include "data.h"
#include <assert.h>
#include <string.h>

void data_main_test(void);

static void data_insert_test(void);
static void data_get_test(void);

static void data_insert_test(void)
{
    register const size_t max_elements = 10;
    register const size_t element_size = sizeof(Data);

    register char *const message = "TEST";
    const Data value_to_insert = {.data_info = message, .data_size = strlen(message)};
    Queue *q = queue_create(max_elements, element_size);

    assert(q != NULL);

    assert(queue_is_empty(q));
    assert(!queue_is_full(q));

    assert(queue_enqueue(q, &value_to_insert) == 0);

    assert(!queue_is_empty(q));
    assert(!queue_is_full(q));

    queue_destroy(q);
}

void data_get_test(void)
{
    register const size_t max_elements = 10;
    register const size_t element_size = sizeof(Data);

    register char *const message = "TEST";
    const Data value_to_insert = {.data_info = message, .data_size = strlen(message)};
    Data value = {};
    Queue *q = queue_create(max_elements, element_size);

    assert(q != NULL);

    assert(queue_is_empty(q));
    assert(!queue_is_full(q));

    assert(queue_enqueue(q, &value_to_insert) == 0);

    assert(!queue_is_empty(q));
    assert(!queue_is_full(q));

    assert(queue_dequeue(q, &value) == 0);
    assert(value.data_info == value_to_insert.data_info);
    assert(value.data_size == value.data_size);

    assert(queue_is_empty(q));
    assert(!queue_is_full(q));

    queue_destroy(q);
}

void data_main_test(void)
{
    data_insert_test();
    data_get_test();
}
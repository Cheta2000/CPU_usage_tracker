#include "queue.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void queue_main_test(void);

static void queue_create_test(void);
static void queue_empty_test(void);
static void queue_single_enqueue_test(void);
static void queue_full_enqueue_test(void);
static void queue_single_dequeue_test(void);
static void queue_full_dequeue_test(void);

static void queue_create_test(void)
{
    {
        register const size_t max_elements = 10;
        register const size_t element_size = sizeof(int);

        Queue *q = queue_create(max_elements, element_size);

        assert(q != NULL);

        queue_destroy(q);
    }
    {
        register const size_t max_elements = 0;
        register const size_t element_size = sizeof(int);

        Queue *q = queue_create(max_elements, element_size);

        assert(q == NULL);

        queue_destroy(q);
    }
    {
        register const size_t max_elements = 10;
        register const size_t element_size = 0;

        Queue *q = queue_create(max_elements, element_size);

        assert(q == NULL);

        queue_destroy(q);
    }
    {
        register const size_t max_elements = 10;
        register const size_t element_size = sizeof(char *);

        Queue *q = queue_create(max_elements, element_size);

        assert(q != NULL);

        queue_destroy(q);
    }
}

static void queue_empty_test(void)
{
    register const size_t max_elements = 10;
    register const size_t element_size = sizeof(int);

    Queue *q = queue_create(max_elements, element_size);

    assert(q != NULL);
    assert(queue_is_empty(q));
    assert(!queue_is_full(q));

    queue_destroy(q);
}

static void queue_single_enqueue_test(void)
{
    {
        register const size_t value_to_insert = 123;
        register const size_t max_elements = 10;
        register const size_t element_size = sizeof(int);

        Queue *q = queue_create(max_elements, element_size);

        assert(q != NULL);
        assert(queue_enqueue(NULL, &(int){value_to_insert}) != 0);
        assert(queue_enqueue(q, NULL) != 0);
        assert(queue_enqueue(NULL, NULL) != 0);

        assert(queue_is_empty(q));
        assert(!queue_is_full(q));

        assert(queue_enqueue(q, &(int){value_to_insert}) == 0);

        assert(!queue_is_empty(q));
        assert(!queue_is_full(q));

        queue_destroy(q);
    }
    {
        register char *const value_to_insert = "TEST";
        register const size_t max_elements = 10;
        register const size_t element_size = sizeof(char *);

        Queue *q = queue_create(max_elements, element_size);

        assert(q != NULL);

        assert(queue_is_empty(q));
        assert(!queue_is_full(q));

        assert(queue_enqueue(q, &(char *){value_to_insert}) == 0);

        assert(!queue_is_empty(q));
        assert(!queue_is_full(q));

        queue_destroy(q);
    }
}

static void queue_full_enqueue_test(void)
{
    {
        register const size_t value_to_insert = 123;
        register const size_t max_elements = 10;
        register const size_t element_size = sizeof(int);

        Queue *q = queue_create(max_elements, element_size);

        assert(q != NULL);

        assert(queue_is_empty(q));
        assert(!queue_is_full(q));

        for (size_t i = 0; i < max_elements; i++)
        {
            assert(queue_enqueue(q, &(int){i}) == 0);
            assert(!queue_is_empty(q));
            assert(queue_is_full(q) == (i == max_elements - 1));
        }

        assert(!queue_is_empty(q));
        assert(queue_is_full(q));

        assert(queue_enqueue(q, &(int){value_to_insert}) != 0);

        queue_destroy(q);
    }

    {
        register char *const value_to_insert = "TEST";
        register const size_t max_elements = 10;
        register const size_t element_size = sizeof(char *);

        Queue *q = queue_create(max_elements, element_size);

        assert(q != NULL);

        assert(queue_is_empty(q));
        assert(!queue_is_full(q));

        for (size_t i = 0; i < max_elements; i++)
        {
            assert(queue_enqueue(q, &(char *){value_to_insert}) == 0);
            assert(!queue_is_empty(q));
            assert(queue_is_full(q) == (i == max_elements - 1));
        }

        assert(!queue_is_empty(q));
        assert(queue_is_full(q));

        assert(queue_enqueue(q, value_to_insert) != 0);

        queue_destroy(q);
    }
}

static void queue_single_dequeue_test(void)
{
    {
        register const size_t value_to_insert = 123;
        register const size_t max_elements = 10;
        register const size_t element_size = sizeof(int);

        int value = 0;
        Queue *q = queue_create(max_elements, element_size);

        assert(q != NULL);

        assert(queue_is_empty(q));
        assert(!queue_is_full(q));

        assert(queue_enqueue(q, &(int){value_to_insert}) == 0);

        assert(!queue_is_empty(q));
        assert(!queue_is_full(q));

        assert(queue_dequeue(q, &value) == 0);
        assert(value_to_insert == value);

        assert(queue_is_empty(q));
        assert(!queue_is_full(q));

        queue_destroy(q);
    }
    {
        register char *const value_to_insert = "TEST";
        register const size_t max_elements = 10;
        register const size_t element_size = sizeof(char *);

        char *value = "";
        Queue *q = queue_create(max_elements, element_size);

        assert(q != NULL);

        assert(queue_is_empty(q));
        assert(!queue_is_full(q));

        assert(queue_enqueue(q, &(char *){value_to_insert}) == 0);

        assert(!queue_is_empty(q));
        assert(!queue_is_full(q));

        assert(queue_dequeue(q, &value) == 0);

        assert(strcmp(value, value_to_insert) == 0);

        assert(queue_is_empty(q));
        assert(!queue_is_full(q));

        queue_destroy(q);
    }
}

static void queue_full_dequeue_test(void)
{
    {
        register const size_t max_elements = 10;
        register const size_t element_size = sizeof(int);

        int value = 0;
        Queue *q = queue_create(max_elements, element_size);

        assert(q != NULL);

        assert(queue_is_empty(q));
        assert(!queue_is_full(q));

        for (size_t i = 0; i < max_elements; i++)
        {
            assert(queue_enqueue(q, &(int){i}) == 0);
            assert(!queue_is_empty(q));
            assert(queue_is_full(q) == (i == max_elements - 1));
        }

        assert(!queue_is_empty(q));
        assert(queue_is_full(q));

        for (size_t i = 0; i < max_elements; i++)
        {
            assert(queue_dequeue(q, &value) == 0);
            assert(value == (int)i);
            assert(queue_is_empty(q) == (i == max_elements - 1));
            assert(!queue_is_full(q));
        }

        assert(queue_is_empty(q));
        assert(!queue_is_full(q));

        assert(queue_dequeue(q, &value) != 0);

        queue_destroy(q);
    }
}

void queue_main_test(void)
{
    queue_create_test();
    queue_empty_test();
    queue_single_enqueue_test();
    queue_full_enqueue_test();
    queue_single_dequeue_test();
    queue_full_dequeue_test();
}
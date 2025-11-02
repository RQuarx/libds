#include "ds/darray.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared/xmalloc.h"

#define SUCCESS fprintf(stderr, "successful\n");
#define FAILED \
   { fprintf(stderr, "failed (%s:%d)\n", __FILE__, __LINE__); exit(1); }
#define START fprintf(stderr, "%s: ", __func__);

#define ASSERT(expr) \
    if (!(expr)) FAILED


void
test_edge_cases(void)
{
    START

    /* empty array operations */
    darray_t da = darray_new(sizeof(int));

    /* this operation should fail */
    void *res = darray_pop_back(da);
    ASSERT(res == NULL && errno == ERANGE);

    /* this operation should fail */
    res = darray_erase(da, 0);
    ASSERT(res == NULL && errno == ERANGE);

    /* boundary operations */
    int val = 42;

    /* insert at 0 when empty */
    darray_insert(da, &val, 0);
    ASSERT(darray_size(da) == 1);

    /* insert at end */
    darray_insert(da, &val, 1);
    ASSERT(darray_size(da) == 2);

    /* erase last */
    darray_erase(da, 1);
    ASSERT(darray_size(da) == 1);

    /* beyond size, should fail */
    res = darray_insert(da, &val, 3);
    ASSERT(res == NULL && errno == ERANGE);

    darray_free(da);
    SUCCESS
}


void
test_zero_size(void)
{
    START

    /* should fail */
    darray_t da = darray_new(0);
    ASSERT(da == NULL && errno == EINVAL);

    da = darray_new(sizeof(int));

    /* should fail */
    void *res = darray_reserve(da, 0);
    ASSERT(res == NULL && errno == EINVAL);

    darray_free(da);
    SUCCESS
}


void
test_resizing(void)
{
    START

    /* automatic growth */
    darray_t da  = darray_new(sizeof(int));
    size_t   cap = darray_capacity(da);

    for (int i = 0; i < 100; i++)
    {
        int val = i;
        darray_push_back(da, &val);

        if (darray_size(da) > cap)
        {
            cap = darray_capacity(da);
            ASSERT(cap >= darray_size(da));
        }
    }
    ASSERT(darray_size(da) == 100);

    /* manual resize */
    int      vals[5] = { 1, 2, 3, 4, 5 };
    darray_t da2     = darray_new(sizeof(int));

    for (int i = 0; i < 5; i++) darray_push_back(da2, &vals[i]);

    /* shrink */
    darray_resize(da2, 3);
    ASSERT(darray_size(da2) == 3 && *(int *)darray_at(da2, 0) == 1);

    /* expand */
    darray_resize(da2, 10);
    ASSERT(darray_size(da2) == 10 && *(int *)darray_at(da2, 3) == 0);

    /* reserve without resize */
    darray_reserve(da2, 50);
    ASSERT(darray_capacity(da2) >= 50 && darray_size(da2) == 10);

    darray_free(da);
    darray_free(da2);
    SUCCESS
}


void
test_error_handling(void)
{
    START

    /* allocation failure */
    darray_t da
        = darray_new_with_allocator(sizeof(int), fail_malloc, NULL, NULL);
    ASSERT(da == NULL);

    da = darray_new(sizeof(int));

    /* out-of-bounds */
    void *res = darray_at(da, 10);
    ASSERT(res == NULL && errno == ERANGE);

    darray_free(da);
    SUCCESS
}


void
test_data_types(void)
{
    START

    /* primitives */
    darray_t da = darray_new(sizeof(double));
    double   pi = 3.14;

    darray_push_back(da, &pi);
    ASSERT(*(double *)darray_at(da, 0) == 3.14);

    darray_free(da);

    /* structs */
    struct test
    {
        int  a;
        char b;
    };

    struct test t = { 42, 'x' };
    da            = darray_new(sizeof(struct test));

    darray_insert(da, &t, 0);

    struct test *retrieved = darray_at(da, 0);
    ASSERT(retrieved->a == 42 && retrieved->b == 'x');

    darray_free(da);

    SUCCESS
}


void
test_string_pointers(void)
{
    START

    darray_t da
        = darray_new_with_allocator(sizeof(char **), xmalloc, xrealloc, NULL);

    char *str1 = "Hello, World!";
    char *str2 = "New, String!";
    char *str3 = "Dynamic Array";

    darray_insert(da, (void *)&str1, 0);
    darray_push_back(da, (void *)&str2);
    darray_push_back(da, (void *)&str3);

    printf("da[0]: %s, size: %zu\n", *(char **)darray_at(da, 0),
           darray_size(da));

    darray_erase(da, 0);
    printf("da[0]: %s, size: %zu\n", *(char **)darray_at(da, 0),
           darray_size(da));

    darray_pop_front(da);
    printf("da[0]: %s, size: %zu\n", *(char **)darray_at(da, 0),
           darray_size(da));

    darray_free(da);
    SUCCESS
}


int
main(void)
{
    test_edge_cases();
    test_zero_size();
    test_resizing();
    test_error_handling();
    test_data_types();
    test_string_pointers();

    return 0;
}

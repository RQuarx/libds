#include "ds/darray.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define DARRAY_FREE(da, ptr) \
    ELSE_IF_NULL(((darray_t)da)->free_fn, free, ptr)

#define DARRAY_REALLOC(da, ...) \
    ELSE_IF_NULL(((darray_t)da)->realloc_fn, realloc, __VA_ARGS__)

#define DARRAY_VALID(da) (da->tp_size != 0)

#define DARRAY_GROWTH_FACTOR 1.5F
#define DARRAY_INITIAL_SIZE 5


struct dyn_array
{
    void *data;

    size_t tp_size;
    size_t alloc_size;
    size_t elem_amount;

    ds_realloc_fn realloc_fn;
    ds_free_fn    free_fn;
};


struct dyn_array *
darray_new_with_allocator(size_t type_size, ds_malloc_fn malloc_func,
                          ds_realloc_fn realloc_fn, ds_free_fn free_fn)
{
    if (type_size == 0)
    {
        errno = EINVAL;
        return NULL;
    }

    struct dyn_array *da = malloc_func == NULL
                             ? malloc(sizeof(struct dyn_array))
                             : malloc_func(sizeof(struct dyn_array));
    if (da == NULL) return NULL;

    da->tp_size     = type_size;
    da->alloc_size  = 0;
    da->elem_amount = 0;

    da->data       = NULL;
    da->realloc_fn = realloc_fn;
    da->free_fn    = free_fn;
    return da;
}


struct dyn_array *
darray_new(size_t type_size)
{
    return darray_new_with_allocator(type_size, NULL, NULL, NULL);
}


void
darray_free(darray_t da)
{
    DARRAY_FREE(da, da);
}


void
darray_free_full(darray_t da)
{
    if (da->data != NULL) DARRAY_FREE(da, da->data);
    darray_free(da);
}


void *
darray_reserve(darray_t da, size_t size)
{
    if (da->alloc_size >= size) return da->data;
    if (!DARRAY_VALID(da) || size == 0)
    {
        errno = EINVAL;
        return NULL;
    }

    errno          = 0;
    void *new_data = DARRAY_REALLOC(da, da->data, size * da->tp_size);
    if (new_data == NULL) return NULL;

    da->data       = new_data;
    da->alloc_size = size;
    return da->data;
}


void *
darray_resize(darray_t da, size_t size)
{
    const size_t old_size = da->elem_amount;
    void        *res      = darray_reserve(da, size);

    if (res == NULL) return NULL;

    if (size > old_size)
    {
        size_t diff  = size - old_size;
        void  *start = (char *)da->data + (old_size * da->tp_size);
        memset(start, 0, diff * da->tp_size);
    }

    da->elem_amount = size;
    return res;
}


void
darray_clear(darray_t da)
{
    if (da->data != NULL) memset(da->data, 0, da->elem_amount * da->tp_size);
    da->elem_amount = 0;
}


void *
darray_at(const darray_t da, size_t index)
{
    if (index >= da->elem_amount)
    {
        errno = ERANGE;
        return NULL;
    }

    return (void *)((char *)da->data + (index * da->tp_size));
}


void *
darray_insert(const darray_t restrict da, void *restrict data, size_t pos)
{
    if (pos > da->elem_amount)
    {
        errno = ERANGE;
        return NULL;
    }

    if (da->elem_amount >= da->alloc_size)
    {
        size_t size = da->alloc_size == 0
                        ? DARRAY_INITIAL_SIZE
                        : da->alloc_size + (da->alloc_size >> 1);
        /* simulates multiplication by 1.5 ^ */

        if (darray_reserve(da, size) == NULL) return NULL;
    }

    if (pos == da->elem_amount)
    {
        da->elem_amount++;

        void *elem = darray_at(da, da->elem_amount - 1);
        memcpy(elem, data, da->tp_size);

        return elem;
    }

    if (pos < da->elem_amount)
    {
        char  *base          = (char *)da->data;
        size_t bytes_to_move = (da->elem_amount - pos) * da->tp_size;

        void *dest = base + ((pos + 1) * da->tp_size);
        void *src  = base + (pos * da->tp_size);

        memmove(dest, src, bytes_to_move);
    }

    void *elem_ptr = darray_at(da, pos);
    memcpy(elem_ptr, data, da->tp_size);

    da->elem_amount++;
    return elem_ptr;
}


void *
darray_erase(darray_t da, size_t pos)
{
    if (pos > da->elem_amount || da->elem_amount == 0)
    {
        errno = ERANGE;
        return NULL;
    }

    if (pos == da->elem_amount - 1)
    {
        da->elem_amount--;
        return da->data;
    }

    if (pos < da->elem_amount - 1)
    {
        char  *base          = (char *)da->data;
        size_t bytes_to_move = (da->elem_amount - pos) * da->tp_size;

        void *dest = base + (pos * da->tp_size);
        void *src  = base + ((pos + 1) * da->tp_size);

        memmove(dest, src, bytes_to_move);
        da->elem_amount--;
    }

    return da->data;
}


size_t
darray_size(const darray_t da)
{
    return da->elem_amount;
}


size_t
darray_capacity(const darray_t da)
{
    return da->alloc_size;
}


size_t
darray_type_size(const darray_t da)
{
    return da->tp_size;
}


void *
darray_data(const darray_t da)
{
    return da->data;
}


void *
darray_push_back(const darray_t da, void *data)
{
    return darray_insert(da, data, darray_size(da));
}


void *
darray_pop_back(const darray_t da)
{
    return darray_erase(da, darray_size(da) - 1);
}


void *
darray_push_front(const darray_t da, void *data)
{
    return darray_insert(da, data, 0);
}


void *
darray_pop_front(const darray_t da)
{
    return darray_erase(da, 0);
}

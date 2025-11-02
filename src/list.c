#include "ds/list.h"

#include <errno.h>
#include <stdlib.h>

#define LIST_TO_HEAD(head) \
    do { while (head->prev != NULL) head = head->prev; } while (0)

#define LIST_TO_TAIL(tail) \
    do { while (tail->next != NULL) tail = tail->next; } while (0)

#define LIST_FREE(list, ptr) \
    ELSE_IF_NULL(((list_t)list)->free_fn, free, ptr)

#define LIST_MALLOC(list, size) \
    ELSE_IF_NULL(((list_t)list)->malloc_fn, malloc, size)


struct linked_list
{
    list_t prev;
    list_t next;

    void *data;

    ds_malloc_fn malloc_fn;
    ds_free_fn   free_fn;
};


list_t
list_new_with_allocator(ds_malloc_fn malloc_fn, ds_free_fn free_fn)
{
    list_t list = ELSE_IF_NULL(malloc_fn, malloc, sizeof(struct linked_list));
    if (list == NULL) return list;

    list->data = NULL;
    list->prev = NULL;
    list->next = NULL;

    list->malloc_fn = malloc_fn;
    list->free_fn   = free_fn;

    return list;
}


list_t
list_new(void)
{
    return list_new_with_allocator(NULL, NULL);
}


void
list_free_node(list_t list)
{
    list_t head = list;
    LIST_TO_HEAD(head);

    /* connect prev with next */
    if (list->prev != NULL) list->prev->next = list->next;
    LIST_FREE(head, list);
}


void
list_free(list_t list)
{
    LIST_TO_HEAD(list);

    while (list != NULL)
    {
        list_t next = list->next;
        LIST_FREE(list, list);
        list = next;
    }
}


list_t
list_append(list_t list, void *data)
{
    list_t next = list_new_with_allocator(list->malloc_fn, list->free_fn);
    if (next == NULL) return NULL;

    next->data = data;
    next->prev = list;

    LIST_TO_TAIL(list);
    list->next = next;

    return next;
}


list_t
list_set_data(restrict list_t list, void *restrict data)
{
    list->data = data;
    return list;
}


list_t
list_prepend(list_t list, void *data)
{
    list_t prev = list_new_with_allocator(list->malloc_fn, list->free_fn);
    if (prev == NULL) return NULL;

    prev->data = data;
    prev->next = list;

    LIST_TO_HEAD(list);
    list->prev = prev;

    return prev;
}


void *
list_data(list_t list)
{
    return list->data;
}


list_t
list_next(list_t list)
{
    return list->next;
}


list_t
list_prev(list_t list)
{
    return list->prev;
}


list_t
list_at(list_t list, int64_t index)
{
    list_t current = list;

    if (index < 0)
        for (int64_t i = 0; i > index; i--)
        {
            if (current->prev == NULL) goto err;
            current = current->prev;
        }
    else
        for (int64_t i = 0; i < index; i++)
        {
            if (current->next == NULL) goto err;
            current = current->next;
        }

    return current;

err:
    errno = ERANGE;
    return NULL;
}

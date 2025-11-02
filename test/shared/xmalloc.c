#include "shared/xmalloc.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


void *
xmalloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        perror("allocation failure");
        exit(errno);
    }

    return ptr;
}


void *
xrealloc(void *old, size_t new_size)
{
    void *new = realloc(old, new_size);
    if (new == NULL)
    {
        perror("allocation failure");
        exit(errno);
    }

    return new;
}


void *
fail_malloc(size_t size)
{
    (void)size;
    return NULL;
}
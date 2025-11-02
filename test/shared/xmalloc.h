#ifndef _TEST_XMALLOC_H
#define _TEST_XMALLOC_H 1
#define __need_size_t 1
#include <stddef.h>


void *xmalloc(size_t size);
void *xrealloc(void *old, size_t new_size);
void *fail_malloc(size_t);

#endif /* _TEST_XMALLOC_H */

#include "ds/list.h"

#include <stddef.h>
#include <stdio.h>

#include "shared/xmalloc.h"


int
main(void)
{
    list_t list = list_new_with_allocator(xmalloc, NULL);

    char *str1 = "Hello, World!";
    char *str2 = "New, String!";
    char *str3 = "Linked List";

    list_set_data(list, str1);
    list_append(list, str2);
    list_prepend(list, str3);

    printf("%s\n", (char *)list_data(list));
    printf("%s\n", (char *)list_data(list_next(list)));
    printf("%s\n", (char *)list_data(list_at(list, -1)));

    list_free(list);
}

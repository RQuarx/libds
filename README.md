<div align=center>

# LibDS

LibDS is a C library that provides useful data structures that follows the
design and structure of the C standard library.

</div>

---

<div align=center>

## Overview

</div>

`libds` provides a suite of commonly used data structures, such as dynamic arrays,
and linked lists. Implemented in pure C, and written as if
they're part of `libc`. The goal is _not_ to replace existing implementations,
but rather, to explore what it would like if `libc` had implemented these common
data structures.

## Data Structure List

<details>
<summary><b>Dynamic Array</b></summary>

```c
darray_t da = darray_new_with_allocator(sizeof(char **), xmalloc, xrealloc, NULL);

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
```

</details>


<details>
<summary><b>Linked List</b></summary>

```c
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
```

</details>
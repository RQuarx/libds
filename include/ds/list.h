#pragma once
#ifndef _DS_LIST_H
#define _DS_LIST_H 1
#define __need_size_t 1
#include <stddef.h>
#include <stdint.h>

#include "ds/__priv/cdefs.h"

__DS_BEGIN_DECLS


/**
 * @typedef list_t
 * @struct linked_list
 *
 * @brief A single/doubly linked list implementation.
 */
typedef struct linked_list *list_t;


/**
 * @brief Allocate a new @struct linked_list node with a custom allocator.
 *
 * @return A pointer to the allocated node, or `NULL` on failure.
 * @note `errno` is set to whatever the allocator function
 *       sets `errno` to on failure.
 *
 * @sa ::new
 * @sa ::free
 */
extern list_t list_new_with_allocator(ds_malloc_fn malloc_fn,
                                      ds_free_fn   free_fn)
    __DS_ATTR_MALLOC __DS_ATTR_NODISCARD;


/**
 * @brief Allocate a new @struct linked_list node.
 *
 * @return A pointer to the allocated node, or `NULL` on failure.
 *         Check `errno` for more information.
 *
 * @sa ::new_with_allocator
 * @sa ::free
 */
extern list_t list_new(void) __DS_ATTR_MALLOC __DS_ATTR_NODISCARD;


/**
 * @brief Frees up a @struct linked_list node allocated by ::new.
 *
 * @sa ::free_full
 * @sa ::new
 */
extern void list_free_node(list_t list) __DS_ATTR_NONNULL(1);


/**
 * @brief Fully frees all node of a @struct linked_list .
 *
 * @sa ::free
 * @sa ::new
 */
extern void list_free(list_t list) __DS_ATTR_NONNULL(1);


/**
 * @brief Inserts a new list with a specified data at the tail node of
 *        a @struct linked_list .
 *
 * @param data The data to be inserted.
 *
 * @return A pointer to the tail node, or `NULL` on failure.
 *         Check `errno` for more information.
 *
 * @sa ::set_data
 * @sa ::prepend
 */
extern list_t list_append(list_t restrict list, void *restrict data)
    __DS_ATTR_NONNULL(1);


/**
 * @brief Sets the provided node data to the specified data.
 *
 * @param data The data to be inserted.
 *
 * @return The pointer @param list.
 *
 * @sa ::append
 * @sa ::prepend
 */
extern list_t list_set_data(list_t restrict list, void *restrict data)
    __DS_ATTR_NONNULL(1);


/**
 * @brief Inserts a new list with a specified data at the head node of
 *        a @struct linked_list .
 *
 * @param data The data to be inserted.
 *
 * @return A pointer to the tail node, or `NULL` on failure.
 *         Check `errno` for more information.
 *
 * @sa ::set_data
 * @sa ::append
 */
extern list_t list_prepend(list_t restrict list, void *restrict data)
    __DS_ATTR_NONNULL(1);


/**
 * @brief Get the current node's data.
 *
 * @sa ::next
 * @sa ::prev
 * @sa ::at
 */
 extern void *list_data(list_t list) __DS_ATTR_NONNULL(1);


/**
 * @brief Get the next node of a @struct linked_list .
 *
 * @sa ::data
 * @sa ::prev
 * @sa ::at
 */
extern list_t list_next(list_t list) __DS_ATTR_NONNULL(1);


/**
 * @brief Get the previous node of a @struct linked_list .
 *
 * @sa ::data
 * @sa ::next
 * @sa ::at
 */
extern list_t list_prev(list_t list) __DS_ATTR_NONNULL(1);


/**
 * @brief Get the nth node of a @struct linked_list .
 *
 * @sa ::data
 * @sa ::prev
 * @sa ::next
 */
extern list_t list_at(list_t list, int64_t index) __DS_ATTR_NONNULL(1);


__DS_END_DECLS

#endif /* _DS_LIST_H */

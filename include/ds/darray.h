/*
 * Copyright (c) 2025 Kei <RQuarx@protonmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * This file contains the declaration of the dynamic array structure
 * `dyn_array`, alongside with the functions that manipulates it.
 */

#ifndef _DS_DARRAY_H
#define _DS_DARRAY_H 1
#define __need_size_t 1
#include <stddef.h>

#include "ds/__priv/cdefs.h"

__DS_BEGIN_DECLS


/**
 * @typedef darray_t
 * @struct dyn_array
 *
 * @brief A dynamic array implementation.
 */
typedef struct dyn_array *darray_t;


/**
 * @brief Allocate a new @struct dyn_array with a custom allocator.
 *
 * @param type_size The size of the type the struct will hold.
 *
 * @return A pointer to the allocated @struct dyn_array , or `NULL` on failure.
 * @note `errno` is set to whatever the allocator function
 *       sets `errno` to on failure.
 *
 * @sa ::new
 * @sa ::free
 */
extern darray_t darray_new_with_allocator(
    size_t type_size, ds_malloc_fn malloc_func, ds_realloc_fn realloc_fn,
    ds_free_fn free_fn) __DS_THROW __DS_ATTR_MALLOC __DS_ATTR_NODISCARD;


/**
 * @brief Allocate a new @struct dyn_array .
 *
 * @param type_size The size of the type the struct will hold.
 *
 * @return A pointer to the allocated @struct dyn_array , or `NULL` on failure.
 *         Check `errno` for more information.
 *
 * @sa ::new_with_allocator
 * @sa ::free
 */
extern darray_t
darray_new(size_t type_size) __DS_THROW __DS_ATTR_MALLOC __DS_ATTR_NODISCARD;


/**
 * @brief Frees up a @struct dyn_array allocated by ::new.
 *
 * @sa ::free_full
 * @sa ::new
 */
extern void darray_free(darray_t da) __DS_ATTR_NONNULL(1);


/**
 * @brief Frees up all data allocated by a @struct dyn_array
 *        and the @struct dyn_array itself.
 *
 * @sa ::free
 * @sa ::new
 */
extern void darray_free_full(darray_t da) __DS_ATTR_NONNULL(1);


/**
 * @brief Ensures a @struct dyn_array has at least the specified capacity.
 *
 * @param size The minimum number of elements the @struct dyn_array
 *             should be able to hold.
 *
 * @return A pointer to the internal data buffer, or `NULL` on allocation failure.
 *         Check `errno` for more information.
 *
 * @note The function do nothing if the capacity is larger than or
 *       equal to @param size
 * @note The function will fail and set `errno` to EINVAL if the type size that
 *       the @struct dyn_array hold is 0, and if @param size is 0.
 *
 * @sa ::set_allocators
 * @sa ::free
 * @sa ::resize
 */
extern void *darray_reserve(darray_t da, size_t size) __DS_ATTR_NONNULL(1);


/**
 * @brief Ensures a @struct dyn_array can contain the
 *        specified amount of elements.
 *
 * @param size The amount of element that the @struct dyn_array should be
 *             able to hold.
 *
 * @return A pointer to the internal data buffer, or `NULL` on failure.
 *         Check `errno` for more information.
 *
 * @note The function will call ::reserve if @param size is larger than
 *       the internal buffer's size.
 *
 * @sa ::reserve
 */
extern void *darray_resize(darray_t da, size_t size) __DS_ATTR_NONNULL(1);


/**
 * @brief Clears the content of the internal buffer of a @struct dyn_array .
 *
 * @warning The function does not free the internal buffer.
 *
 * @sa ::clear_full
 * @sa ::free
 */
extern void darray_clear(darray_t da) __DS_ATTR_NONNULL(1);


/**
 * @brief Gets the element at the specified index.
 *
 * @return The pointer to the element, or `NULL` on failure.
 *         Check `errno`  for more information.
 */
extern void *darray_at(darray_t da, size_t index)
    __DS_ATTR_NONNULL(1) __DS_ATTR_NODISCARD;


/**
 * @brief Inserts an element on a position inside a @struct dyn_array .
 *
 * @param data The data to be inserted.
 * @param pos  The position @param data will be inserted in.
 *
 * @return A pointer pointing to where @param data is inside the
 *         internal buffer, or `NULL` on failure.
 *         Check `errno` for more information.
 *
 * @sa ::erase
 */
extern void *darray_insert(darray_t restrict da, void *restrict data,
                           size_t pos) __DS_ATTR_NONNULL(1);


/**
 * @brief Erases an element at the specified position.
 *
 * @param pos The position/index of the element that will be erased.
 * @return A pointer pointing to the internal buffer, or `NULL` on failure.
 *         Check `errno` for more information.
 *
 * @warning The function does not free the erased data.
 *
 * @sa ::erase_free
 * @sa ::insert
 */
extern void *darray_erase(darray_t da, size_t pos) __DS_ATTR_NONNULL(1);


/**
 * @brief Get the amount of items a @struct dyn_array holds.
 */
extern size_t darray_size(darray_t da)
    __DS_ATTR_NONNULL(1) __DS_ATTR_PURE __DS_ATTR_NODISCARD;


/**
 * @brief Get the amount of items a @struct dyn_array can hold.
 */
extern size_t darray_capacity(darray_t da)
    __DS_ATTR_NONNULL(1) __DS_ATTR_PURE __DS_ATTR_NODISCARD;


/**
 * @brief Get the type size of the type that a @struct dyn_array is holding.
 */
extern size_t darray_type_size(darray_t da)
    __DS_ATTR_NONNULL(1) __DS_ATTR_PURE __DS_ATTR_NODISCARD;


/**
 * @brief Get the internal data buffer of a @struct dyn_array .
 */
extern void *darray_data(darray_t da)
    __DS_ATTR_NONNULL(1) __DS_ATTR_PURE __DS_ATTR_NODISCARD;


/**
 * @brief Inserts an element at the back a @struct dyn_array .
 *
 * @param data The data to be inserted.
 *
 * @return A pointer pointing to where @param data is inside the
 *         internal buffer, or `NULL` on failure.
 *         Check `errno` for more information.
 *
 * @sa ::insert
 * @sa ::pop_back
 */
void *darray_push_back(darray_t restrict da, void *restrict data)
    __DS_ATTR_NONNULL(1);


/**
 * @brief Erases an element at the back of a @struct dyn_array.
 *
 * @return A pointer pointing to the internal buffer, or `NULL` on failure.
 *         Check `errno` for more information.
 *
 * @warning The function does not free the erased data.
 *
 * @sa ::erase
 * @sa ::push_back
 */
void *darray_pop_back(darray_t da) __DS_ATTR_NONNULL(1);


/**
 * @brief Inserts an element at the front a @struct dyn_array .
 *
 * @param data The data to be inserted.
 *
 * @return A pointer pointing to where @param data is inside the
 *         internal buffer, or `NULL` on failure.
 *         Check `errno` for more information.
 *
 * @sa ::insert
 * @sa ::pop_front
 */
void *darray_push_front(darray_t restrict da, void *restrict data)
    __DS_ATTR_NONNULL(1);


/**
 * @brief Erases an element at the front of a @struct dyn_array.
 *
 * @return A pointer pointing to the internal buffer, or `NULL` on failure.
 *         Check `errno` for more information.
 *
 * @warning The function does not free the erased data.
 *
 * @sa ::erase
 * @sa ::push_front
 */
void *darray_pop_front(darray_t da) __DS_ATTR_NONNULL(1);


__DS_END_DECLS

#endif /* _DS_DARRAY_H */

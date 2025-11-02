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

#ifndef __DS_PRIV_CDEFS_H
#define __DS_PRIV_CDEFS_H 1
#define __need_size_t 1
#include <stddef.h>


#ifdef __cplusplus
#if __cplusplus >= 201103L
#define __DS_THROW noexcept(true)
#else
#define __DS_THROW throw()
#endif
#define __DS_BEGIN_DECLS extern "C" {
#define __DS_END_DECLS	 }
#else
#define __DS_BEGIN_DECLS
#define __DS_END_DECLS
#define __DS_THROW
#endif

#if defined(__GNUC__) || defined(__clang__)
#define __DS_ATTR_MALLOC        __attribute__((malloc))
#define __DS_ATTR_NONNULL(...)  __attribute__((nonnull(__VA_ARGS__)))
#define __DS_ATTR_NODISCARD     __attribute__((warn_unused_result))
#define __DS_ATTR_PURE          __attribute__((pure))
#else
#define __DS_ATTR_MALLOC
#define __DS_ATTR_NONNULL(...)
#define __DS_ATTR_NODISCARD
#define __DS_ATTR_PURE
#endif

#define ELSE_IF_NULL(check, other, ...) \
    ((void *)(check) == NULL) ? (other(__VA_ARGS__)) : (check(__VA_ARGS__))


/**
 * @typedef ds_malloc_fn
 *
 * @brief The allocator function signature for libds data structures.
 *
 * @param size The allocated memory size.
 * @return The allocated memory's pointer, or NULL on failure.
 */
typedef void *(*ds_malloc_fn)(size_t size);

/**
 * @typedef ds_realloc_fn
 *
 * @brief The reallocator function signature for libds data structures.
 *
 * @param old  The old memory that will be resized.
 * @param size The new allocated memory size.
 * @return The new allocated memory's pointer, or NULL on failure.
 */
typedef void *(*ds_realloc_fn)(void *old, size_t size);

/**
 * @typedef ds_free_fn
 *
 * @brief The Deallocator function signature for libds data structures.
 *
 * @param ptr The pointer to the allocated memory, can be NULL.
 */
typedef void (*ds_free_fn)(void *ptr);


#endif /* __DS_PRIV_CDEFS_H */

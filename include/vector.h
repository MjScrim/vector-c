#ifndef VECTOR_H
#define VECTOR_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * struct Vector - Dynamic array container
 * @data: Pointer to the raw memory buffer
 * @capacity: Total number of elements currently allocated
 * @size: Number of elements actually stored in the vector
 * @element_size: Size of each element in bytes
 */
typedef struct Vector {
	void *data;
	size_t capacity;
	size_t size;
	size_t element_size;
} Vector;

/**
 * enum VectorStatus - Return codes for vector operations
 * @VECTOR_SUCCESS: Operation completed successfully
 * @VECTOR_ERR_NULL_PTR: A NULL pointer was passed as an argument
 * @VECTOR_ERR_OUT_OF_BOUNDS: Index provided is outside the valid range
 * @VECTOR_ERR_ALLOC_FAILED: Memory allocation via malloc/realloc failed
 */
typedef enum {
	VECTOR_SUCCESS = 0,
	VECTOR_ERR_NULL_PTR = 1,
	VECTOR_ERR_OUT_OF_BOUNDS = -2,
	VECTOR_ERR_ALLOC_FAILED = -3
} VectorStatus;

/* =========================
   Lifecycle
   ========================= */

/* See implementation in vector.c for full documentation */
VectorStatus vector_init(struct Vector *v, size_t capacity, size_t element_size);
VectorStatus vector_free(struct Vector *v);

/* =========================
   capacity
   ========================= */
VectorStatus vector_reserve(struct Vector *v, size_t new_capacity);
VectorStatus vector_shrink_to_fit(struct Vector *v);

/* =========================
   Element acess
   ========================= */
const void *vector_at(struct Vector *v, size_t index);
VectorStatus vector_get(struct Vector *v, size_t index, void *dest);
void *vector_begin(struct Vector *v);
void *vector_end(struct Vector *v);
void *vector_find(struct Vector *v, void *target, bool (*compare)(void *, void *));
VectorStatus vector_foreach(struct Vector *v, void (*action)(void *));
VectorStatus vector_foreach_range(struct Vector *v, size_t start_index, size_t end_index, void (*action)(void *));

/* =========================
   Modifiers
   ========================= */
VectorStatus vector_push(struct Vector *v, void *value);
VectorStatus vector_insert(struct Vector *v, void *value, size_t index);
VectorStatus vector_remove(struct Vector *v, size_t index);
VectorStatus vector_remove_range(struct Vector *v, size_t start_index, size_t count);
VectorStatus vector_pop(struct Vector *v);
VectorStatus vector_set(struct Vector *v, size_t index, void *value);
VectorStatus vector_clear(struct Vector *v);

/* =========================
   Utils
   ========================= */
VectorStatus vector_print(struct Vector *v, void (*print_fn)(void *));
VectorStatus vector_clone(struct Vector *dest, struct Vector *src);
VectorStatus vector_extend(struct Vector *dest, struct Vector *src);

/* =========================
   TYPE-SAFE MACROS (Generics via C99)
   ========================= */

//========================= Element access =========================

/**
 * VECTOR_AT() - Access an element with type checking
 * @vec_ptr: Pointer to the vector
 * @type: The expected C type of the element
 * @index: Position of the element
 * * Return: The value of the element at @index.
 */
#define VECTOR_AT(vec_ptr, type, index)                                                            \
	(assert(sizeof(type) == (vec_ptr)->element_size && "VECTOR_AT: Incorrect type!"),          \
	 *(type *)vector_at((vec_ptr), (index)))

/**
 * VECTOR_GET() - Copy an element into a variable with type checking
 * @vec_ptr: Pointer to the vector
 * @type: The expected C type of the element
 * @index: Position of the element
 * @dest_ptr: Pointer to the destination variable
 * * Return: VECTOR_SUCCESS or error code.
 */
#define VECTOR_GET(vec_ptr, type, index, dest_ptr)                                                 \
	(assert(sizeof(type) == (vec_ptr)->element_size && "VECTOR_GET:  Incorrect type!"),        \
	 vector_get((vec_ptr), (index), (void *)(dest_ptr)))

/**
 * VECTOR_FIND() - Search for a value with type safety
 * @vec_ptr: Pointer to the vector
 * @type: The expected C type of the elements
 * @value: The value to search for
 * @compare_fn: Predicate function for comparison
 * * Return: Pointer of @type to the found element, or NULL.
 */
#define VECTOR_FIND(vec_ptr, type, value, compare_fn)                                              \
	(assert(sizeof(type) == (vec_ptr)->element_size && "VECTOR_FIND:  Incorrect type"),        \
	 (type *)vector_find((vec_ptr), &(type){value}, (bool (*)(void *, void *))(compare_fn)))

//========================= Modifiers =========================

/**
 * VECTOR_PUSH() - Type-safe macro to append a value
 * @vec_ptr: Pointer to the vector
 * @type: The explicit C type of the value (e.g., int, float, struct MyStruct)
 * @value: The actual value to push
 *
 * This macro performs a compile-time size check using assert() and
 * creates a temporary literal of @type to pass its address to vector_push().
 */
#define VECTOR_PUSH(vec_ptr, type, value)                                                          \
	(assert(sizeof(type) == (vec_ptr)->element_size && "VECTOR_PUSH: Size type incorrect!"),   \
	 vector_push((vec_ptr), &(type){value}))

/**
 * VECTOR_INSERT() - Insert a value at index using type safety
 * @vec_ptr: Pointer to the vector
 * @type: The C type of the value
 * @value: The value to insert
 * @index: The target position
 * * Return: VECTOR_SUCCESS or error code.
 */
#define VECTOR_INSERT(vec_ptr, type, value, index)                                                 \
	(assert(sizeof(type) == (vec_ptr)->element_size && "VECTOR_INSERT:  Incorrect type!"),     \
	 vector_insert((vec_ptr), &(type){value}, (index)))

/**
 * VECTOR_SET() - Overwrite an element at index with type safety
 * @vec_ptr: Pointer to the vector
 * @type: The C type of the value
 * @value: The new value
 * @index: Position to overwrite
 * * Return: VECTOR_SUCCESS or error code.
 */
#define VECTOR_SET(vec_ptr, type, value, index)                                                    \
	(assert(sizeof(type) == (vec_ptr)->element_size && "VECTOR_SET:  Incorrect type!"),        \
	 vector_set((vec_ptr), (index), &(type){value}))

#endif

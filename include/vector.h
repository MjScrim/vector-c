#ifndef VECTOR_H
#define VECTOR_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Vector {
	void *data;
	size_t capacity;
	size_t size;
	size_t element_size;
} Vector;

typedef enum {
	VECTOR_SUCCESS = 0,
	VECTOR_ERR_NULL_PTR = 1,
	VECTOR_ERR_OUT_OF_BOUNDS = -2,
	VECTOR_ERR_ALLOC_FAILED = -3
} VectorStatus;

/* =========================
   Lifecycle
   ========================= */
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

/* =========================
   TYPE-SAFE MACROS (Generics via C99)
   ========================= */

//========================= Element access =========================
#define VECTOR_AT(vec_ptr, type, index)                                                            \
	(assert(sizeof(type) == (vec_ptr)->element_size && "VECTOR_AT: Incorrect type!"),          \
	 *(type *)vector_at((vec_ptr), (index)))

#define VECTOR_GET(vec_ptr, type, index, dest_ptr)                                                 \
	(assert(sizeof(type) == (vec_ptr)->element_size && "VECTOR_GET:  Incorrect type!"),        \
	 vector_get((vec_ptr), (index), (void *)(dest_ptr)))

#define VECTOR_FIND(vec_ptr, type, value, compare_fn)                                              \
	(assert(sizeof(type) == (vec_ptr)->element_size && "VECTOR_FIND:  Incorrect type"),        \
	 (type *)vector_find((vec_ptr), &(type){value}, (bool (*)(void *, void *))(compare_fn)))

//========================= Modifiers =========================
#define VECTOR_PUSH(vec_ptr, type, value)                                                          \
	(assert(sizeof(type) == (vec_ptr)->element_size && "VECTOR_PUSH: Size type incorrect!"),   \
	 vector_push((vec_ptr), &(type){value}))

#define VECTOR_INSERT(vec_ptr, type, value, index)                                                 \
	(assert(sizeof(type) == (vec_ptr)->element_size && "VECTOR_INSERT:  Incorrect type!"),     \
	 vector_insert((vec_ptr), &(type){value}, (index)))

#define VECTOR_SET(vec_ptr, type, value, index)                                                    \
	(assert(sizeof(type) == (vec_ptr)->element_size && "VECTOR_SET:  Incorrect type!"),        \
	 vector_set((vec_ptr), (index), &(type){value}))

#endif

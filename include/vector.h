#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Vector {
  void* data;
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
VectorStatus vector_init(struct Vector* v, size_t capacity, size_t element_size);
VectorStatus vector_free(struct Vector* v);

/* =========================
   capacity
   ========================= */
VectorStatus vector_reserve(struct Vector* v, size_t new_capacity);
VectorStatus vector_shrink_to_fit(struct Vector* v);

/* =========================
   Element acess
   ========================= */
const void* vector_at(struct Vector* v, size_t index);
VectorStatus vector_get(struct Vector* v, size_t index, void* dest);
void* vector_begin(struct Vector* v);
void* vector_end(struct Vector* v);
void* vector_find(struct Vector* v, void* target, bool (*compare)(void*, void*));
VectorStatus vector_foreach(struct Vector* v, void (*action)(void*));

/* =========================
   Modifiers
   ========================= */
VectorStatus vector_push(struct Vector* v, void* value);
VectorStatus vector_insert(struct Vector* v, void* value, size_t index);
VectorStatus vector_remove(struct Vector* v, size_t index);
VectorStatus vector_remove_range(struct Vector*v , size_t start_index, size_t count);
VectorStatus vector_pop(struct Vector* v);
VectorStatus vector_set(struct Vector* v, size_t index, void* value);

/* =========================
   Utils
   ========================= */
VectorStatus vector_print(struct Vector* v, void (*print_fn)(void*));

#endif

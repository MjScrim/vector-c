#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

struct Vector {
  void* data;
  size_t capacity;
  size_t size;
  size_t element_size;
};

/* =========================
   Lifecycle
   ========================= */
void vector_init(struct Vector* v, size_t capacity, size_t element_size);
void vector_free(struct Vector* v);

/* =========================
   capacity
   ========================= */
void vector_reserve(struct Vector* v, size_t new_capacity);

/* =========================
   Element acess
   ========================= */
const void* vector_at(struct Vector* v, size_t index);
void vector_get(struct Vector* v, size_t index, void* dest);
void* vector_begin(struct Vector* v);
void* vector_end(struct Vector* v);
void* vector_find(struct Vector* v, void* target, bool (*compare)(void*, void*));
void vector_foreach(struct Vector* v, void (*action)(void*));

/* =========================
   Modifiers
   ========================= */
void vector_push(struct Vector* v, void* value);
void vector_insert(struct Vector* v, void* value, size_t index);
void vector_remove(struct Vector* v, size_t index);
void vector_pop(struct Vector* v);
void vector_set(struct Vector* v, size_t index, void* value);

/* =========================
   Utils
   ========================= */
void vector_print(struct Vector* v, void (*print_fn)(const void*));

#endif

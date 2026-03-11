#include "vector.h"
#include <stdlib.h>
#include <string.h>

void vector_init(struct Vector *v, size_t capacity, size_t element_size) {
  if (capacity == 0) capacity = 1; 

  if (element_size == 0) {
    v->data = NULL;
    v->capacity = 0;
    v->size = 0;
    v->element_size = 0;
    return;
  }

  v->data = malloc(capacity * element_size);  

  if (!v->data) { 
    v->capacity = 0;
    v->size = 0;
    v->element_size = 0;
    return;
  }

  v->element_size = element_size;
  v->capacity = capacity;
  v->size = 0;
}

void vector_push(struct Vector *v, void* value) {
  if (v->size == v->capacity) {
    size_t new_capacity = 2 * v->capacity;

    void* temp = realloc(v->data, new_capacity * v->element_size);

    if (!temp) return;

    v->data = temp;
    v->capacity = new_capacity;
  }   

  memcpy(vector_ptr(v, v->size), value, v->element_size);

  v->size++;
}

void vector_remove(struct Vector *v, size_t index) {
  if (index >= v->size) return;

  if (index == v->size - 1) {
    vector_pop(v);
    return;
  }

  size_t n = (v->size - index - 1) * v->element_size;

  memmove(
    vector_ptr(v ,index)
    ,vector_ptr(v, index + 1),
    n);

  v->size--;
}

void vector_pop(struct Vector *v) {
  if (v->size == 0) return;
  v->size--;
}

void vector_reserve(struct Vector *v, size_t new_capacity) {
  if (new_capacity <= v->capacity) return;

  void* temp = realloc(v->data, new_capacity * v->element_size);

  if(!temp) return;

  v->data = temp;
  v->capacity = new_capacity;
}

void* vector_ptr(struct Vector* v, size_t index) {
  return (char*)v->data + index * v->element_size;
}

void* vector_at(struct Vector *v, size_t index) {
  if (index >= v->size) return NULL;

  return (char*)v->data + index * v->element_size;
}

void vector_free(struct Vector *v) {
  free(v->data);

  v->data = NULL;
  v->size = 0;
  v->capacity = 0;
  v->element_size = 0;
}

void vector_print(struct Vector *v, void (*print_fn)(void*)) {
  for (size_t i = 0; i < v->size; i++) {
    void* element = vector_at(v, i);

    print_fn(element);
  }

  printf("\n");
}

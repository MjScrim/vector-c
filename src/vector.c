#include "vector.h"
#include <stdlib.h>
#include <string.h>

static void vector_grow_if_needed(struct Vector *v);
static void* vector_ptr(struct Vector* v, size_t index);

/* === Lifecycle ================================================= */
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

void vector_free(struct Vector *v) {
  free(v->data);

  v->data = NULL;
  v->size = 0;
  v->capacity = 0;
  v->element_size = 0;
}

/* === Capacity ================================================= */
void vector_reserve(struct Vector* v, size_t new_capacity) {
  if (new_capacity <= v->capacity) return;

  void* temp = realloc(v->data, new_capacity * v->element_size);

  if (!temp) return;

  v->data = temp;
  v->capacity = new_capacity;
}

/* === Element acess ================================================= */
const void* vector_at(struct Vector* v, size_t index) {
  if (index >= v->size) return NULL;

  return (const char*)v->data + index * v->element_size;
}

void vector_get(struct Vector* v, size_t index, void* dest) {
  if (index >= v->size) return;

  void* src = vector_ptr(v, index);

  memcpy(dest, src, v->element_size);
}

void* vector_begin(struct Vector* v) {
  return v->data;
}

void* vector_end(struct Vector* v) {
  return vector_ptr(v, v->size);
}

/* === Modifiers ================================================= */
void vector_push(struct Vector* v, void* value) {
  vector_grow_if_needed(v); 

  memcpy(vector_ptr(v, v->size), value, v->element_size);

  v->size++;
}

void vector_insert(struct Vector* v, void* value, size_t index) {
  if (index > v->size) return;

   vector_grow_if_needed(v);

  size_t n = (v->size - index) * v->element_size; 

  memmove(
    vector_ptr(v,index + 1),
    vector_ptr(v, index),
    n
  );

  memcpy(vector_ptr(v, index), value, v->element_size);

  v->size++;
}

void vector_remove(struct Vector* v, size_t index) {
  if (index >= v->size) return;

  if (index == v->size - 1) {
    vector_pop(v);
    return;
  }

  size_t n = (v->size - index - 1) * v->element_size;

  memmove(
    vector_ptr(v ,index)
    ,vector_ptr(v, index + 1),
    n
  );

  v->size--;
}

void vector_pop(struct Vector* v) {
  if (v->size == 0) return;
  v->size--;
}

void vector_set(struct Vector *v, size_t index, void *value) {
  if (index >= v->size) return;

  memcpy(vector_ptr(v, index), value, v->element_size);
}

/* === Utils ================================================= */
void vector_print(struct Vector* v, void (*print_fn)(const void*)) {
  if (!print_fn) return;

  char* begin = (char*)vector_begin(v);
  char* end = (char*)vector_end(v);

  for (char* it = begin; it != end; it += v->element_size) { 
    print_fn(it);

    printf(" ");
  }

  printf("\n");
}

/* === Static intern ================================================= */
static void vector_grow_if_needed(struct Vector* v) {
  if (v->size == v->capacity) {
    size_t new_capacity = 2 * v->capacity;

    void* temp = realloc(v->data, new_capacity * v->element_size);

    if (!temp) return;

    v->data = temp;
    v->capacity = new_capacity;
  } 
}

static void* vector_ptr(struct Vector* v, size_t index) {
  return (char*)v->data + index * v->element_size;
}

#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static void vector_grow_if_needed(struct Vector* v);
static void vector_shrink_if_needed(struct Vector* v);
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

void vector_shrink_to_fit(struct Vector* v) {
  if (v->size == v->capacity) {
    return;
  }

  if (v->size == 0) {
    free(v->data);
    v->data = NULL;
    v->capacity = 0;
    return;
  }

  size_t new_capacity_bytes = v->size * v->element_size;

  void* new_data = realloc(v->data, new_capacity_bytes);

  if (new_data != NULL) {
    v->data = new_data;
    v->capacity = v->size;
  }
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

void* vector_find(struct Vector* v, void* target, bool (*compare)(void*, void*)) {
  char* it = vector_begin(v);
  char* end = vector_end(v);

  while (it != end) {
    if (compare(it, target)) {
      return it;
    }

    it += v->element_size;
  }

  return NULL;
}

void vector_foreach(struct Vector* v, void (*action)(void*)) {
  char* it = vector_begin(v);
  char* end = vector_end(v);

  while (it != end) {
    action(it);
    it += v->element_size;
  }
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

  vector_shrink_if_needed(v);
}

void vector_remove_range(struct Vector *v, size_t start_index, size_t count) {
  if (start_index >= v->size || count == 0) return;

  if (start_index + count > v->size) {
    count = v->size - start_index;
  }

  size_t elements_to_moive = v->size - (start_index + count);

  if (elements_to_moive > 0) {
    size_t n = elements_to_moive * v->element_size;
    memmove(
      vector_ptr(v, start_index),
      vector_ptr(v, start_index + count),
      n
    );
  }

  v->size -= count;

  vector_shrink_if_needed(v);
}

void vector_pop(struct Vector* v) {
  if (v->size == 0) return;
  v->size--;

  vector_shrink_if_needed(v);
}

void vector_set(struct Vector *v, size_t index, void *value) {
  if (index >= v->size) return;

  memcpy(vector_ptr(v, index), value, v->element_size);
}

/* === Utils ================================================= */
void vector_print(struct Vector* v, void (*print_fn)(void*)) {
  if (!print_fn) return;

  vector_foreach(v, print_fn); 

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

static void vector_shrink_if_needed(struct Vector* v) {
  if (v->capacity > 4 && v->capacity / 4) {
    size_t new_capacity = v->capacity / 2;
    size_t new_capacity_bytes = new_capacity * v->element_size;

    void* new_data = realloc(v->data, new_capacity_bytes);

    if (new_data != NULL) {
      v->data = new_data;
      v->capacity = new_capacity;
    }
  }
}

static void* vector_ptr(struct Vector* v, size_t index) {
  return (char*)v->data + index * v->element_size;
}

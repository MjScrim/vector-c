#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static VectorStatus vector_grow_if_needed(struct Vector* v);
static VectorStatus vector_shrink_if_needed(struct Vector* v);
static void* vector_ptr(struct Vector* v, size_t index);

/* === Lifecycle ================================================= */
VectorStatus vector_init(struct Vector *v, size_t capacity, size_t element_size) {
  if (capacity == 0) capacity = 1; 

  if (element_size == 0) {
    v->data = NULL;
    v->capacity = 0;
    v->size = 0;
    v->element_size = 0;
    return VECTOR_ERR_OUT_OF_BOUNDS;
  }

  v->data = malloc(capacity * element_size);  

  if (!v->data) { 
    v->capacity = 0;
    v->size = 0;
    v->element_size = 0;
    return VECTOR_ERR_ALLOC_FAILED;
  }

  v->element_size = element_size;
  v->capacity = capacity;
  v->size = 0;

  return VECTOR_SUCCESS;
}

VectorStatus vector_free(struct Vector *v) {
  if (!v) return VECTOR_ERR_NULL_PTR;

  free(v->data);

  v->data = NULL;
  v->size = 0;
  v->capacity = 0;
  v->element_size = 0;

  return VECTOR_SUCCESS;
}

/* === Capacity ================================================= */
VectorStatus vector_reserve(struct Vector* v, size_t new_capacity) {
  if (!v) return VECTOR_ERR_NULL_PTR;

  if (new_capacity <= v->capacity) return VECTOR_ERR_OUT_OF_BOUNDS;

  void* temp = realloc(v->data, new_capacity * v->element_size);

  if (!temp) return VECTOR_ERR_ALLOC_FAILED;

  v->data = temp;
  v->capacity = new_capacity;

  return VECTOR_SUCCESS;
}

VectorStatus vector_shrink_to_fit(struct Vector* v) {
  if (!v) return VECTOR_ERR_NULL_PTR;
  
  if (v->size == v->capacity) {
    return VECTOR_ERR_OUT_OF_BOUNDS;
  }

  if (v->size == 0) {
    free(v->data);
    v->data = NULL;
    v->capacity = 0;
    return VECTOR_SUCCESS;
  }

  size_t new_capacity_bytes = v->size * v->element_size;

  void* new_data = realloc(v->data, new_capacity_bytes);

  if (new_data == NULL) {
    return VECTOR_ERR_ALLOC_FAILED;
  }

  v->data = new_data;
  v->capacity = v->size;

  return VECTOR_SUCCESS;
}

/* === Element acess ================================================= */
const void* vector_at(struct Vector* v, size_t index) {
  if (!v) return NULL;

  if (index >= v->size) return NULL;

  return (const char*)v->data + index * v->element_size;
}

VectorStatus vector_get(struct Vector* v, size_t index, void* dest) {
  if (!v) return VECTOR_ERR_NULL_PTR;

  if (index >= v->size) return VECTOR_ERR_OUT_OF_BOUNDS;

  void* src = vector_ptr(v, index);

  if (src == NULL) {
    return VECTOR_ERR_NULL_PTR;
  }

  memcpy(dest, src, v->element_size);

  return VECTOR_SUCCESS;
}

void* vector_begin(struct Vector* v) {
  if (!v) return NULL;

  return v->data;
}

void* vector_end(struct Vector* v) {
  if (!v) return NULL;

  return vector_ptr(v, v->size);
}

void* vector_find(struct Vector* v, void* target, bool (*compare)(void*, void*)) {
  if (!v) return NULL;

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

VectorStatus vector_foreach(struct Vector* v, void (*action)(void*)) {
  if (!v) return VECTOR_ERR_NULL_PTR;

  char* it = vector_begin(v);
  char* end = vector_end(v);

  while (it != end) {
    action(it);
    it += v->element_size;
  }

  return VECTOR_SUCCESS;
}

/* === Modifiers ================================================= */
VectorStatus vector_push(struct Vector* v, void* value) {
  if (!v || !value) return VECTOR_ERR_NULL_PTR;

  VectorStatus status = vector_grow_if_needed(v); 

  if (status != VECTOR_SUCCESS) {
    return status;
  }

  memcpy(vector_ptr(v, v->size), value, v->element_size);
  v->size++;

  return VECTOR_SUCCESS;
}

VectorStatus vector_insert(struct Vector* v, void* value, size_t index) {
  if (!v || !value) return VECTOR_ERR_NULL_PTR;

  if (index > v->size) return VECTOR_ERR_OUT_OF_BOUNDS;

  VectorStatus status = vector_grow_if_needed(v);

  if (status != VECTOR_SUCCESS) {
    return status;
  }

  size_t n = (v->size - index) * v->element_size; 

  memmove(
    vector_ptr(v,index + 1),
    vector_ptr(v, index),
    n
  );

  memcpy(vector_ptr(v, index), value, v->element_size);

  v->size++;

  return VECTOR_SUCCESS;
}

VectorStatus vector_remove(struct Vector* v, size_t index) {
  if (!v) return VECTOR_ERR_NULL_PTR;

  if (index >= v->size) return VECTOR_ERR_OUT_OF_BOUNDS;

  if (index == v->size - 1) {
    VectorStatus status = vector_pop(v);
    return status;
  }

  size_t n = (v->size - index - 1) * v->element_size;

  memmove(
    vector_ptr(v ,index)
    ,vector_ptr(v, index + 1),
    n
  );

  v->size--;
  
  return vector_shrink_if_needed(v);
}

VectorStatus vector_remove_range(struct Vector *v, size_t start_index, size_t count) {
  if (!v) return VECTOR_ERR_NULL_PTR;

  if (start_index >= v->size || count == 0) return VECTOR_ERR_OUT_OF_BOUNDS;

  if (start_index + count > v->size) {
    count = v->size - start_index;
  }

  size_t elements_to_move = v->size - (start_index + count);

  if (elements_to_move > 0) {
    size_t n = elements_to_move * v->element_size;
    memmove(
      vector_ptr(v, start_index),
      vector_ptr(v, start_index + count),
      n
    );
  }

  v->size -= count;

  return vector_shrink_if_needed(v);
}

VectorStatus vector_pop(struct Vector* v) {
  if (!v) return VECTOR_ERR_NULL_PTR;

  if (v->size == 0) return VECTOR_SUCCESS;
  v->size--;

  return vector_shrink_if_needed(v);
}

VectorStatus vector_set(struct Vector *v, size_t index, void *value) {
  if (!v) return VECTOR_ERR_NULL_PTR;

  if (index >= v->size) return VECTOR_ERR_OUT_OF_BOUNDS;

  memcpy(vector_ptr(v, index), value, v->element_size);

  return VECTOR_SUCCESS;
}

VectorStatus vector_clear(struct Vector* v) {
  v->size = 0;

  return VECTOR_SUCCESS;
}

/* === Utils ================================================= */
VectorStatus vector_print(struct Vector* v, void (*print_fn)(void*)) {
  if (!v) return VECTOR_ERR_NULL_PTR;

  if (!print_fn) return VECTOR_ERR_NULL_PTR;

  VectorStatus status = vector_foreach(v, print_fn); 

  printf("\n");

  return status;
}

/* === Static intern ================================================= */
static VectorStatus vector_grow_if_needed(struct Vector* v) {
  if (v->size == v->capacity) {
    size_t new_capacity = v->capacity ? v->capacity * 2 : 1;

    void* temp = realloc(v->data, new_capacity * v->element_size);

    if (!temp) return VECTOR_ERR_ALLOC_FAILED;

    v->data = temp;
    v->capacity = new_capacity;
  } 

  return VECTOR_SUCCESS;
}

static VectorStatus vector_shrink_if_needed(struct Vector* v) {
  if (v->capacity > 4 && v->size <= v->capacity / 4) {
    size_t new_capacity = v->capacity / 2;
    size_t new_capacity_bytes = new_capacity * v->element_size;

    void* new_data = realloc(v->data, new_capacity_bytes);

    if (new_data == NULL) {
      return VECTOR_ERR_ALLOC_FAILED;
    }

    v->data = new_data;
    v->capacity = new_capacity;

    return VECTOR_SUCCESS;
  }

  return VECTOR_SUCCESS;
}

static void* vector_ptr(struct Vector* v, size_t index) {
  return (char*)v->data + index * v->element_size;
}

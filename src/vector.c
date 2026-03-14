#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static VectorStatus vector_grow_if_needed(struct Vector *v);
static VectorStatus vector_shrink_if_needed(struct Vector *v);
static void *vector_ptr(struct Vector *v, size_t index);

/* ==========================================================================
 * LIFECYCLE
 * ========================================================================== */

/**
 * vector_init() - Initializes a dynamic array
 * @v: pointer to the vector structure
 * @capacity: initial number of elements
 * @element_size: size of each element in bytes
 *
 * Return: VECTOR_SUCCESS or error code.
 */
VectorStatus vector_init(struct Vector *v, size_t capacity, size_t element_size)
{
	if (capacity == 0)
		capacity = 1;

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

/**
 * vector_free() - Deallocates the vector's memory and resets its state
 * @v: pointer to the vector to be freed
 *
 * Return: VECTOR_SUCCESS or VECTOR_ERR_NULL_PTR if v is null.
 */
VectorStatus vector_free(struct Vector *v)
{
	if (!v)
		return VECTOR_ERR_NULL_PTR;

	free(v->data);

	v->data = NULL;
	v->size = 0;
	v->capacity = 0;
	v->element_size = 0;

	return VECTOR_SUCCESS;
}

/* ==========================================================================
 * CAPACITY
 * ========================================================================== */

/**
 * vector_reserve() - Manually increases the capacity of the vector
 * @v: pointer to the vector
 * @new_capacity: the minimum number of elements to pre-allocate
 *
 * Return: VECTOR_SUCCESS, VECTOR_ERR_ALLOC_FAILED, or VECTOR_ERR_OUT_OF_BOUNDS.
 */
VectorStatus vector_reserve(struct Vector *v, size_t new_capacity)
{
	if (!v)
		return VECTOR_ERR_NULL_PTR;

	if (new_capacity <= v->capacity)
		return VECTOR_ERR_OUT_OF_BOUNDS;

	void *temp = realloc(v->data, new_capacity * v->element_size);

	if (!temp)
		return VECTOR_ERR_ALLOC_FAILED;

	v->data = temp;
	v->capacity = new_capacity;

	return VECTOR_SUCCESS;
}

/**
 * vector_shrink_to_fit() - Reduces capacity to match the current size
 * @v: pointer to the vector
 *
 * Useful for freeing unused pre-allocated memory.
 *
 * Return: VECTOR_SUCCESS or error code.
 */
VectorStatus vector_shrink_to_fit(struct Vector *v)
{
	if (!v)
		return VECTOR_ERR_NULL_PTR;

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

	void *new_data = realloc(v->data, new_capacity_bytes);

	if (new_data == NULL) {
		return VECTOR_ERR_ALLOC_FAILED;
	}

	v->data = new_data;
	v->capacity = v->size;

	return VECTOR_SUCCESS;
}

/* ==========================================================================
 * ELEMENT ACCESS
 * ========================================================================== */

/**
 * vector_at() - Returns a constant pointer to an element at a specific index
 * @v: pointer to the vector
 * @index: index of the element to access
 *
 * Return: Pointer to the element, or NULL if out of bounds.
 */
const void *vector_at(struct Vector *v, size_t index)
{
	if (!v)
		return NULL;

	if (index >= v->size)
		return NULL;

	return (const char *)v->data + index * v->element_size;
}

/**
 * vector_get() - Copies an element from the vector into a destination pointer
 * @v: pointer to the vector
 * @index: index of the element
 * @dest: pointer where the data will be copied to
 *
 * Return: VECTOR_SUCCESS or error code.
 */
VectorStatus vector_get(struct Vector *v, size_t index, void *dest)
{
	if (!v)
		return VECTOR_ERR_NULL_PTR;

	if (index >= v->size)
		return VECTOR_ERR_OUT_OF_BOUNDS;

	void *src = vector_ptr(v, index);

	if (src == NULL) {
		return VECTOR_ERR_NULL_PTR;
	}

	memcpy(dest, src, v->element_size);

	return VECTOR_SUCCESS;
}

/**
 * vector_begin() - Returns a pointer to the first element
 * @v: pointer to the vector
 *
 * Return: Pointer to data or NULL if vector is empty or null.
 */
void *vector_begin(struct Vector *v)
{
	if (!v)
		return NULL;

	return v->data;
}

/**
 * vector_end() - Returns a pointer to the memory address after the last element
 * @v: pointer to the vector
 *
 * Return: Pointer to end of data or NULL.
 */
void *vector_end(struct Vector *v)
{
	if (!v)
		return NULL;

	return vector_ptr(v, v->size);
}

/**
 * vector_find() - Performs a linear search using a predicate function
 * @v: pointer to the vector
 * @target: the value to search for
 * @compare: function pointer that returns true if elements match
 *
 * Return: Pointer to the first matching element, or NULL if not found.
 */
void *vector_find(struct Vector *v, void *target, bool (*compare)(void *, void *))
{
	if (!v)
		return NULL;

	char *it = vector_begin(v);
	char *end = vector_end(v);

	while (it != end) {
		if (compare(it, target)) {
			return it;
		}

		it += v->element_size;
	}

	return NULL;
}

/**
 * vector_foreach() - Executes a function for each element in the vector
 * @v: pointer to the vector
 * @action: function pointer to be executed for each element
 *
 * Return: VECTOR_SUCCESS or error code.
 */
VectorStatus vector_foreach(struct Vector *v, void (*action)(void *))
{
	if (!v)
		return VECTOR_ERR_NULL_PTR;

	return vector_foreach_range(v, 0, v->size, action);
}

/**
 * vector_foreach_range() - Executes a function on a specific range of elements
 * @v: pointer to the vector
 * @start_index: starting index (inclusive)
 * @end_index: ending index (exclusive)
 * @action: function pointer to be executed
 *
 * Return: VECTOR_SUCCESS or error code.
 */
VectorStatus vector_foreach_range(struct Vector *v, size_t start_index, size_t end_index, void (*action)(void *))
{
	if (!v || !action)
		return VECTOR_ERR_NULL_PTR;

	if (start_index >= v->size || end_index > v->size || start_index > end_index)
		return VECTOR_ERR_OUT_OF_BOUNDS;

	if (start_index == end_index)
		return VECTOR_SUCCESS;

	char *it = vector_ptr(v, start_index);
	char *end_ptr = vector_ptr(v, end_index);

	while (it < end_ptr) {
		action(it);
		it += v->element_size;
	}

	return VECTOR_SUCCESS;
}

/* ==========================================================================
 * MODIFIERS
 * ========================================================================== */

/**
 * vector_push() - Appends a value to the end of the vector
 * @v: pointer to the vector
 * @value: pointer to the data to be added
 *
 * Return: VECTOR_SUCCESS or error code.
 */
VectorStatus vector_push(struct Vector *v, void *value)
{
	if (!v || !value)
		return VECTOR_ERR_NULL_PTR;

	VectorStatus status = vector_grow_if_needed(v);

	if (status != VECTOR_SUCCESS) {
		return status;
	}

	memcpy(vector_ptr(v, v->size), value, v->element_size);
	v->size++;

	return VECTOR_SUCCESS;
}

/**
 * vector_insert() - Inserts a value at a specific index, shifting elements
 * @v: pointer to the vector
 * @value: pointer to the data to be inserted
 * @index: position to insert the data
 *
 * Return: VECTOR_SUCCESS or error code.
 */
VectorStatus vector_insert(struct Vector *v, void *value, size_t index)
{
	if (!v || !value)
		return VECTOR_ERR_NULL_PTR;

	if (index > v->size)
		return VECTOR_ERR_OUT_OF_BOUNDS;

	VectorStatus status = vector_grow_if_needed(v);

	if (status != VECTOR_SUCCESS) {
		return status;
	}

	size_t n = (v->size - index) * v->element_size;

	memmove(vector_ptr(v, index + 1), vector_ptr(v, index), n);

	memcpy(vector_ptr(v, index), value, v->element_size);

	v->size++;

	return VECTOR_SUCCESS;
}

/**
 * vector_remove() - Removes an element at a specific index
 * @v: pointer to the vector
 * @index: index of the element to remove
 *
 * Return: VECTOR_SUCCESS or error code.
 */
VectorStatus vector_remove(struct Vector *v, size_t index)
{
	if (!v)
		return VECTOR_ERR_NULL_PTR;

	if (index >= v->size)
		return VECTOR_ERR_OUT_OF_BOUNDS;

	if (index == v->size - 1) {
		VectorStatus status = vector_pop(v);
		return status;
	}

	size_t n = (v->size - index - 1) * v->element_size;

	memmove(vector_ptr(v, index), vector_ptr(v, index + 1), n);

	v->size--;

	return vector_shrink_if_needed(v);
}

/**
 * vector_remove_range() - Removes multiple elements in a specific range
 * @v: pointer to the vector
 * @start_index: starting index of the range
 * @count: number of elements to remove
 *
 * Return: VECTOR_SUCCESS or error code.
 */
VectorStatus vector_remove_range(struct Vector *v, size_t start_index, size_t count)
{
	if (!v)
		return VECTOR_ERR_NULL_PTR;

	if (start_index >= v->size || count == 0)
		return VECTOR_ERR_OUT_OF_BOUNDS;

	if (start_index + count > v->size) {
		count = v->size - start_index;
	}

	size_t elements_to_move = v->size - (start_index + count);

	if (elements_to_move > 0) {
		size_t n = elements_to_move * v->element_size;
		memmove(
			vector_ptr(v, start_index),
			vector_ptr(v, start_index + count)
			, n
		);
	}

	v->size -= count;

	return vector_shrink_if_needed(v);
}

/**
 * vector_sort() - Sorts the vector using the standard quicksort algorithm
 * @v: pointer to the vector
 * @compare: function pointer for element comparison
 *
 * Wrapper around the standard library qsort(). The comparison function
 * must return an integer less than, equal to, or greater than zero if
 * the first argument is considered to be respectively less than, equal to,
 * or greater than the second.
 *
 * Return: VECTOR_SUCCESS or error code.
 */
VectorStatus vector_sort(struct Vector *v, int (*compare)(const void *, const void *))
{
	if (!v || !compare)
		return VECTOR_ERR_NULL_PTR;

	if (v->size <= 1)
		return VECTOR_SUCCESS;

	qsort(v->data, v->size, v->element_size, compare);

	return VECTOR_SUCCESS;
}

/**
 * vector_pop() - Removes the last element of the vector
 * @v: pointer to the vector
 *
 * Return: VECTOR_SUCCESS or error code.
 */
VectorStatus vector_pop(struct Vector *v)
{
	if (!v)
		return VECTOR_ERR_NULL_PTR;

	if (v->size == 0)
		return VECTOR_SUCCESS;
	v->size--;

	return vector_shrink_if_needed(v);
}

/**
 * vector_set() - Overwrites an existing element with new data
 * @v: pointer to the vector
 * @index: index to overwrite
 * @value: pointer to the new data
 *
 * Return: VECTOR_SUCCESS or error code.
 */
VectorStatus vector_set(struct Vector *v, size_t index, void *value)
{
	if (!v)
		return VECTOR_ERR_NULL_PTR;

	if (index >= v->size)
		return VECTOR_ERR_OUT_OF_BOUNDS;

	memcpy(vector_ptr(v, index), value, v->element_size);

	return VECTOR_SUCCESS;
}

/**
 * vector_clear() - Resets the vector size to zero without freeing memory
 * @v: pointer to the vector
 *
 * Return: VECTOR_SUCCESS or error code.
 */
VectorStatus vector_clear(struct Vector *v)
{
	if (!v)
		return VECTOR_ERR_NULL_PTR;

	v->size = 0;

	return VECTOR_SUCCESS;
}

/* ==========================================================================
 * UTILS
 * ========================================================================== */

/**
 * vector_print() - Iterates and prints elements using a provided function
 * @v: pointer to the vector
 * @print_fn: function pointer that handles the printing of a single element
 *
 * Return: VECTOR_SUCCESS or error code.
 */
VectorStatus vector_print(struct Vector *v, void (*print_fn)(void *))
{
	if (!v)
		return VECTOR_ERR_NULL_PTR;

	if (!print_fn)
		return VECTOR_ERR_NULL_PTR;

	VectorStatus status = vector_foreach(v, print_fn);

	printf("\n");

	return status;
}

/**
 * vector_clone() - Creates a deep copy of a vector
 * @dest: pointer to an uninitialized vector structure
 * @src: pointer to the source vector to copy from
 *
 * Return: VECTOR_SUCCESS or error code.
 */
VectorStatus vector_clone(struct Vector *dest, struct Vector *src)
{
	if (!dest || !src)
		return VECTOR_ERR_NULL_PTR;

	if (src->element_size == 0 || src->capacity == 0) {
		return VECTOR_ERR_OUT_OF_BOUNDS;
	}

	VectorStatus status = vector_init(
		dest,
		src->capacity,
		src->element_size
	);
	if (status != VECTOR_SUCCESS) {
		return status;
	}

	if (src->size > 0) {
		size_t bytes_to_copy = src->size * src->element_size;
		memcpy(dest->data, src->data, bytes_to_copy);

		dest->size = src->size;
	}

	return VECTOR_SUCCESS;
}


/**
 * vector_extend() - Copy vector to the end of another vector  
 * @dest: pointer to vector structure
 * @src: pointer to the source vector to copy from
 *
 * Return: VECTOR_SUCCESS or error code.
 */
VectorStatus vector_extend(struct Vector *dest, struct Vector *src) 
{
	if (!dest || !src)
		return VECTOR_ERR_NULL_PTR;

	if (src->size == 0)
		return VECTOR_SUCCESS;

	if (dest->element_size != src->element_size)
		return VECTOR_ERR_OUT_OF_BOUNDS;

	size_t required_capacity = dest->size + src->size;

	if (required_capacity > dest->capacity) {
		VectorStatus status = vector_reserve(dest, required_capacity);
		if (status != VECTOR_SUCCESS)
			return status;
	}

	void *insert_ptr = vector_ptr(dest, dest->size);
	size_t bytes_to_copy = src->size * src->element_size;

	memmove(insert_ptr, src, bytes_to_copy);

	dest->size = required_capacity;

	return VECTOR_SUCCESS;
}

/* ==========================================================================
 * INTERNAL HELPERS
 * ========================================================================== */

/**
 * vector_grow_if_needed() - Internal check to resize data when capacity is reached
 * @v: pointer to the vector
 *
 * Return: VECTOR_SUCCESS or VECTOR_ERR_ALLOC_FAILED.
 */
static VectorStatus vector_grow_if_needed(struct Vector *v)
{
	if (v->size == v->capacity) {
		size_t new_capacity = v->capacity ? v->capacity * 2 : 1;

		void *temp = realloc(v->data, new_capacity * v->element_size);

		if (!temp)
			return VECTOR_ERR_ALLOC_FAILED;

		v->data = temp;
		v->capacity = new_capacity;
	}

	return VECTOR_SUCCESS;
}

/**
 * vector_grow_if_needed() - Internal check to resize data when capacity is reached
 * @v: pointer to the vector
 *
 * Return: VECTOR_SUCCESS or VECTOR_ERR_ALLOC_FAILED.
 */
static VectorStatus vector_shrink_if_needed(struct Vector *v)
{
	if (v->capacity > 4 && v->size <= v->capacity / 4) {
		size_t new_capacity = v->capacity / 2;
		size_t new_capacity_bytes = new_capacity * v->element_size;

		void *new_data = realloc(v->data, new_capacity_bytes);

		if (new_data == NULL) {
			return VECTOR_ERR_ALLOC_FAILED;
		}

		v->data = new_data;
		v->capacity = new_capacity;

		return VECTOR_SUCCESS;
	}

	return VECTOR_SUCCESS;
}

/**
 * vector_ptr() - Calculates the raw memory address of an element
 * @v: pointer to the vector
 * @index: element index
 *
 * Return: Pointer to the element.
 */
static void *vector_ptr(struct Vector *v, size_t index)
{
	return (char *)v->data + index * v->element_size;
}

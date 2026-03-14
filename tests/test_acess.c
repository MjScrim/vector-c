#include "test_helpers.h"
#include "vector.h"
#include <assert.h>

static void test_vector_get()
{
	Vector v;
	ASSERT_OK(vector_init(&v, 1, sizeof(char)));

	ASSERT_OK(VECTOR_PUSH(&v, char, 'a'));

	char get_char;
	ASSERT_OK(VECTOR_GET(&v, char, 0, &get_char));

	ASSERT_OK(VECTOR_PUSH(&v, char, 's'));
	ASSERT_OK(VECTOR_PUSH(&v, char, 'd'));

	assert(v.size == 3);

	assert(vector_at(&v, 0) != NULL);
	assert(get_char == VECTOR_AT(&v, char, 0));

	assert(VECTOR_AT(&v, char, 1) == 's');
	assert(VECTOR_AT(&v, char, 2) == 'd');

	ASSERT_OK(vector_free(&v));
}

static void test_begin()
{
	Vector v;
	setup_int(&v);

	void *ptr = vector_begin(&v);
	assert(ptr != NULL);

	int begin = *(int *)ptr;

	assert(v.size == 3);
	assert(begin == 10);

	ASSERT_OK(vector_free(&v));
}

static void test_end()
{
	Vector v;
	ASSERT_OK(vector_init(&v, 1, sizeof(char)));

	assert(vector_begin(&v) == vector_end(&v));

	ASSERT_OK(VECTOR_PUSH(&v, char, 'a'));
	ASSERT_OK(VECTOR_PUSH(&v, char, 'b'));
	ASSERT_OK(VECTOR_PUSH(&v, char, 'c'));

	char *begin = (char *)vector_begin(&v);
	char *end = (char *)vector_end(&v);

	size_t expected_distance_in_bytes = v.size * v.element_size;

	assert((size_t)(end - begin) == expected_distance_in_bytes);

	char *expected_end_address = (char *)v.data + (v.size * v.element_size);
	assert(end == expected_end_address);

	ASSERT_OK(vector_free(&v));
}

static void test_find()
{
	Vector v;
	setup_int(&v);

	int *src = VECTOR_FIND(&v, int, 20, compare_int);

	assert(src != NULL);
	assert(*src == 20);
	assert((void *)src == vector_at(&v, 1));

	assert(VECTOR_FIND(&v, int, 99, compare_int) == NULL);

	ASSERT_OK(vector_free(&v));
}

static void test_foreach()
{
	Vector v;
	setup_int(&v);

	ASSERT_OK(vector_foreach(&v, double_value));

	assert_int(&v, 0, 20);
	assert_int(&v, 1, 40);
	assert_int(&v, 2, 60);

	ASSERT_OK(vector_free(&v));
}

static void test_foreach_range()
{

	Vector v;
	setup_int(&v);

	size_t start_index = 1;

	ASSERT_OK(vector_foreach_range(&v, start_index, v.size, double_value));

	assert_int(&v, 0, 10);
	assert_int(&v, 1, 40);
	assert_int(&v, 2, 60);

	ASSERT_OK(vector_free(&v));
}

static void test_bsearh()
{
	Vector v;
	setup_int(&v);

	ASSERT_OK(VECTOR_PUSH(&v, int, 50));
	ASSERT_OK(VECTOR_PUSH(&v, int, 60));
	ASSERT_OK(VECTOR_PUSH(&v, int, 40));
	ASSERT_OK(VECTOR_PUSH(&v, int, 80));
	ASSERT_OK(VECTOR_PUSH(&v, int, 90));

	ASSERT_OK(vector_sort(&v, compare_ints));

	int target = 40;

	void *ptr = vector_bsearch(&v, &target, compare_ints);
	assert(target == *(int *)ptr);

	ASSERT_OK(vector_free(&v));
}

void run_access_tests()
{
	test_vector_get();
	test_begin();
	test_end();
	test_find();
	test_foreach();
	test_foreach_range();
	test_bsearh();
}

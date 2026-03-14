#include "test_helpers.h"
#include "vector.h"

static void test_push()
{
	Vector v;
	setup_int(&v);

	assert(v.size == 3);
	assert(v.capacity == 4);

	assert_int(&v, 0, 10);
	assert_int(&v, 1, 20);
	assert_int(&v, 2, 30);

	ASSERT_OK(vector_free(&v));
}

static void test_remove()
{
	Vector v;
	setup_int(&v);

	ASSERT_OK(vector_remove(&v, 1));

	assert(v.size == 2);

	assert_int(&v, 0, 10);
	assert_int(&v, 1, 30);

	ASSERT_OK(vector_free(&v));
}

static void test_remove_first_and_last()
{
	Vector v;
	setup_char(&v);

	ASSERT_OK(vector_remove(&v, 0));
	ASSERT_OK(vector_remove(&v, 1));

	assert(v.size == 1);
	assert_char(&v, 0, 'b');

	ASSERT_OK(vector_free(&v));
}

static void test_remove_range()
{
	Vector v;
	setup_int(&v);

	ASSERT_OK(vector_remove_range(&v, 0, 1));

	assert(v.size == 2);
	assert_int(&v, 0, 20);

	ASSERT_OK(vector_free(&v));
}

static void test_insert()
{
	Vector v;
	ASSERT_OK(vector_init(&v, 1, sizeof(char)));

	ASSERT_OK(VECTOR_INSERT(&v, char, 't', 0));

	ASSERT_OK(VECTOR_PUSH(&v, char, 'q'));
	ASSERT_OK(VECTOR_PUSH(&v, char, 'w'));
	ASSERT_OK(VECTOR_PUSH(&v, char, 'e'));

	ASSERT_OK(VECTOR_INSERT(&v, char, 'r', 1));

	assert(v.size == 5);
	assert_char(&v, 1, 'r');

	ASSERT_OK(vector_free(&v));
}

static void test_pop()
{
	Vector v;
	setup_int(&v);

	ASSERT_OK(vector_pop(&v));

	assert(v.size == 2);
	assert_int(&v, 0, 10);

	ASSERT_OK(vector_free(&v));
}

static void test_set()
{
	Vector v;
	setup_char(&v);

	ASSERT_OK(VECTOR_SET(&v, char, 'j', 1));

	assert(v.size == 3);
	assert(VECTOR_AT(&v, char, 1) == 'j');

	ASSERT_OK(vector_free(&v));
}

static void test_clear()
{
	Vector v;
	setup_char(&v);

	ASSERT_OK(vector_clear(&v));

	assert(v.size == 0);

	ASSERT_OK(vector_free(&v));
}

void run_modifiers_tests()
{
	test_push();
	test_remove();
	test_remove_first_and_last();
	test_remove_range();
	test_insert();
	test_pop();
	test_set();
	test_clear();
}

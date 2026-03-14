#include "test_helpers.h"

void setup_int(Vector *v)
{
	ASSERT_OK(vector_init(v, 1, sizeof(int)));

	ASSERT_OK(VECTOR_PUSH(v, int, 10));
	ASSERT_OK(VECTOR_PUSH(v, int, 20));
	ASSERT_OK(VECTOR_PUSH(v, int, 30));
}

void setup_char(Vector *v)
{
	ASSERT_OK(vector_init(v, 1, sizeof(char)));

	ASSERT_OK(VECTOR_PUSH(v, char, 'a'));
	ASSERT_OK(VECTOR_PUSH(v, char, 'b'));
	ASSERT_OK(VECTOR_PUSH(v, char, 'c'));
}

void assert_int(Vector *v, size_t index, int expected)
{
	assert(vector_at(v, index) != NULL);
	assert(VECTOR_AT(v, int, index) == expected);
}

void assert_char(Vector *v, size_t index, char expected)
{
	assert(vector_at(v, index) != NULL);
	assert(VECTOR_AT(v, char, index) == expected);
}

bool compare_int(void *element, void *target) { return *(int *)element == *(int *)target; }
bool compare_char(void *element, void *target) { return *(char *)element == *(char *)target; }

int compare_ints(const void *a, const void *b)
{
	int arg1 = *(const int *)a;
	int arg2 = *(const int *)b;

	if (arg1 < arg2)
		return -1;
	if (arg1 > arg2)
		return 1;
	return 0;
}

void double_value(void *n)
{
	int *ptr = (int *)n;
	*ptr = (*ptr) * 2;
}

#include "test_helpers.h"
#include "vector.h"
#include <assert.h>
#include <string.h>

static void test_clone()
{
	Vector v;
	Vector clone;
	setup_int(&v);

	ASSERT_OK(vector_clone(&clone, &v));

	assert(v.size == clone.size);
	assert(v.capacity == clone.capacity);
	assert(v.element_size == clone.element_size);

	assert(v.data != clone.data);

	size_t bytes_to_compare = v.size * v.element_size;
	assert(memcmp(v.data, clone.data, bytes_to_compare) == 0);

	ASSERT_OK(VECTOR_SET(&clone, int, 999, 0));

	assert_int(&v, 0, 10);
	assert_int(&clone, 0, 999);

	ASSERT_OK(vector_free(&v));
	ASSERT_OK(vector_free(&clone));
}

static void test_extend()
{
	Vector dest;
	Vector src;
	setup_char(&dest);
	setup_char(&src);

	size_t old_dest_size = dest.size;

	ASSERT_OK(vector_extend(&dest, &src));

	assert(dest.size == old_dest_size + src.size);

	assert(vector_end(&dest) == vector_end(&dest));

	ASSERT_OK(vector_free(&dest));
	ASSERT_OK(vector_free(&dest));
}

void run_utils_tests() 
{ 
	test_clone(); 
	test_extend();
}

#include "test_helpers.h"

/* === Capacity Tests ================================================= */
static void test_reserve() {
  Vector v;
  setup_char(&v);

  char a = 'a', b = 'b', c = 'c';

  size_t new_capacity = 10;
  size_t old_size = v.size;

  ASSERT_OK(vector_reserve(&v, new_capacity));

  assert(v.capacity == new_capacity);
  assert(old_size == v.size);

  assert_char(&v, 0, a);
  assert_char(&v, 1, b);
  assert_char(&v, 2, c);

  char d = 'd';
  ASSERT_OK(vector_push(&v, &d));
  assert(v.size == 4);
  assert_char(&v, 3, 'd');

  ASSERT_OK(vector_free(&v));
}

static void test_shrink_to_fit() {
  Vector v;
  setup_int(&v);

  ASSERT_OK(vector_shrink_to_fit(&v));

  assert(v.capacity == v.size);

  ASSERT_OK(vector_free(&v));
}

static void test_shrink_if_needed() {
  Vector v;
  setup_char(&v);

  size_t new_capacity = 12;

  ASSERT_OK(vector_reserve(&v, new_capacity));

  ASSERT_OK(vector_pop(&v));

  assert(v.capacity == new_capacity / 2);

  ASSERT_OK(vector_free(&v));
}

void run_capacity_tests() {
  test_reserve();
  test_shrink_to_fit();
  test_shrink_if_needed();
}

#include "test_helpers.h"

static void test_push_stress() {
  Vector v;
  ASSERT_OK(vector_init(&v, 1, sizeof(int)));

  int N = 100000;

  for (int i = 0; i < N; i++) {
    ASSERT_OK(VECTOR_PUSH(&v, int, i));
  }

  assert(v.size == (size_t)N);

  for (int i = 0; i < N; i++) {
    assert_int(&v, i, i);
  } 

  ASSERT_OK(vector_free(&v));
}

static void test_random_operations() {
  Vector v;
  ASSERT_OK(vector_init(&v, 2, sizeof(int)));

  for (int i = 0; i < 50000; i++) {
    ASSERT_OK(VECTOR_PUSH(&v, int, i));

    if (i % 3 == 0 && v.size > 0) {
      ASSERT_OK(vector_pop(&v));
    }
  }
  
  ASSERT_OK(vector_free(&v));
}

void run_stress_tests() {
  test_push_stress();
  test_random_operations();
}

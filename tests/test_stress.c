#include "test_helpers.h"

/* === Stress Tests ================================================= */
static void test_push_stress() {
  Vector v;
  vector_init(&v, 1, sizeof(int));

  int N = 100000;

  for (int i = 0; i < N; i++) {
    vector_push(&v, &i);
  }

  assert(v.size == (size_t)N);

  for (int i = 0; i < N; i++) {
    assert_int(&v, i, i);
  } 

  vector_free(&v);
}

static void test_random_operations() {
  Vector v;
  vector_init(&v, 2, sizeof(int));

  for (int i = 0; i < 50000; i++) {
    int value = i;
    vector_push(&v, &value);

      if (i % 3 == 0 && v.size > 0) {
      vector_pop(&v);
      }
  }
  
  vector_free(&v);
}

void run_stress_tests() {
  test_push_stress();
  test_random_operations();
}

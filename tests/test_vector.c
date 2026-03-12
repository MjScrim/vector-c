#include <assert.h>
#include <stdio.h>
#include "vector.h"

static void assert_int(struct Vector* v, size_t index, int expected) {
  const int* n = vector_at(v, index);
  assert(n != NULL);
  assert(*n == expected);
}

void test_push() {
  struct Vector v;
  vector_init(&v, 2, sizeof(int));

  int a = 10, b = 20, c = 30;

  vector_push(&v, &a);
  vector_push(&v, &b);
  vector_push(&v, &c);

  assert(v.size == 3);
  assert(v.capacity == 4);

  assert_int(&v, 0, 10);
  assert_int(&v, 1, 20);
  assert_int(&v, 2, 30);

  vector_free(&v);
}

int main() {
  test_push();

  return 0;
}

#include <assert.h>
#include <stdio.h>
#include "vector.h"

static void assert_int(struct Vector* v, size_t index, int expected);

/* === Modifieris Tests ================================================= */
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

void test_pop() {
  struct Vector v;
  vector_init(&v, 2, sizeof(int));

  int a = 10, b = 20;

  vector_push(&v, &a);
  vector_push(&v, &b);

  vector_pop(&v);

  assert(v.size == 1);
  assert_int(&v, 0, 10);

  vector_free(&v);
}

void test_remove() {
  struct Vector v;
  vector_init(&v, 3, sizeof(int));

  int a = 12, b = 13, c = 14;

  vector_push(&v, &a);
  vector_push(&v, &b);
  vector_push(&v, &c);

  vector_remove(&v, 1);

  assert(v.size == 2);

  assert_int(&v, 0, 12);
  assert_int(&v, 1, 14);

  vector_free(&v);
}

int main() {
  test_push();
  test_remove();
  test_pop();

  return 0;
}

static void assert_int(struct Vector* v, size_t index, int expected) {
  const int* n = vector_at(v, index);
  assert(n != NULL);
  assert(*n == expected);
}

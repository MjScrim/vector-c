#include "test_helpers.h"

void setup_int(Vector* v) {
  ASSERT_OK(vector_init(v, 1, sizeof(int)));

  int a = 10, b = 20, c = 30;

  ASSERT_OK(vector_push(v, &a));
  ASSERT_OK(vector_push(v, &b));
  ASSERT_OK(vector_push(v, &c));
}

void setup_char(Vector* v) {
  ASSERT_OK(vector_init(v, 1, sizeof(char)));

  char a = 'a', b = 'b', c = 'c';

  ASSERT_OK(vector_push(v, &a));
  ASSERT_OK(vector_push(v, &b));
  ASSERT_OK(vector_push(v, &c));
}

void assert_int(Vector* v, size_t index, int expected) {
  const int* n = vector_at(v, index);
  assert(n != NULL);
  assert(*n == expected);
}

void assert_char(Vector* v, size_t index, char expected) {
  const char* n = vector_at(v, index);
  assert(n != NULL);
  assert(*n == expected);
}

bool compare_int(void* element, void* target) {
  return *(int*)element == *(int*)target;
}

void double_value(void* n) {
  int* ptr = (int*)n;
  *ptr = (*ptr) * 2;
}

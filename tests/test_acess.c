#include "test_helpers.h"

/* === Element Acess Tests ================================================= */
static void test_vector_get() {
  Vector v;
  vector_init(&v, 1, sizeof(char));

  char a = 'a', b = 's', c = 'd';

  vector_push(&v, &a);

  char get_char;
  vector_get(&v, 0, &get_char);

  vector_push(&v, &b);
  vector_push(&v, &c);

  assert(v.size == 3);
  assert(get_char == *(char*)vector_at(&v, 0));

  assert(*(char*)vector_at(&v, 1) == b);
  assert(*(char*)vector_at(&v, 2) == c);
  
  vector_free(&v);
}

static void test_begin() {
  Vector v;
  setup_int(&v);

  int begin = *(int*)vector_begin(&v);

  assert(v.size == 3);
  assert(begin == 10);

  vector_free(&v);
}

static void test_end() {
  Vector v;
  vector_init(&v, 1, sizeof(char));

  assert(vector_begin(&v) == vector_end(&v));

  char a = 'a', b = 'b', c = 'c';

  vector_push(&v, &a);
  vector_push(&v, &b);
  vector_push(&v, &c);

  char* begin = (char*)vector_begin(&v);
  char* end = (char*)vector_end(&v);

  size_t expected_distance_in_bytes = v.size * v.element_size;

  assert((size_t)(end - begin) == expected_distance_in_bytes);

  char* expected_end_address = (char*)v.data + (v.size * v.element_size);
  assert(end == expected_end_address);

  vector_free(&v);
}

static void test_find() {
  Vector v;
  setup_int(&v);

  int b = 20; 

  void* src = vector_find(&v, &b, compare_int);

  assert(src != NULL);

  int value = *(int*)src;

  assert(value == b);

  assert(src == vector_at(&v, 1));

  int d = 99;
  assert(vector_find(&v, &d, compare_int) == NULL);

  vector_free(&v);
}

static void test_foreach() {
  Vector v;
  setup_int(&v);

  int a = 10, b = 20, c = 30;

  vector_foreach(&v, double_value);

  assert_int(&v,0, a * 2);
  assert_int(&v, 1, b * 2);
  assert_int(&v, 2, c * 2);

  vector_free(&v);
}

void run_access_tests() {
  test_vector_get();
  test_begin();
  test_end();
  test_find();
  test_foreach();
}

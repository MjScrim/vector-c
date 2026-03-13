#include "test_helpers.h"

/* === Modifiers Tests ================================================= */
static void test_push() {
  Vector v;
  setup_int(&v); 

  int a = 10, b = 20, c = 30;

  assert(v.size == 3);
  assert(v.capacity == 4);

  assert_int(&v, 0, a);
  assert_int(&v, 1, b);
  assert_int(&v, 2, c);

  vector_free(&v);
}

static void test_remove() {
  Vector v;
  setup_int(&v); 

  vector_remove(&v, 1);

  assert(v.size == 2);

  assert_int(&v, 0, 10);
  assert_int(&v, 1, 30);

  vector_free(&v);
}

static void test_remove_first_and_last() {
  Vector v;
  setup_char(&v); 

  char b = 'b';

  vector_remove(&v, 0);
  //0 -> removed, last element back 1 index. 2 - 1 = 1.
  vector_remove(&v, 1);
 
  assert(v.size == 1); 
  assert_char(&v, 0, b);

  vector_free(&v);
}

static void test_remove_range() {
  Vector v;
  setup_int(&v);

  int b = 20;

  vector_remove_range(&v, 0, 1);

  assert(v.size == 2);
  assert_int(&v, 0, b);

  vector_free(&v);
}

static void test_insert() {
  Vector v;
  vector_init(&v, 1, sizeof(char));
  
  char a = 'q', b = 'w', c = 'e', d = 'r', e = 't';

  vector_insert(&v, &e, 0);

  vector_push(&v, &a);
  vector_push(&v, &b);
  vector_push(&v, &c);

  vector_insert(&v, &d, 1);

  assert(v.size == 5);
  assert_char(&v, 1, d);

  vector_free(&v);
}

static void test_pop() {
  Vector v;
  setup_int(&v);

  vector_pop(&v);

  assert(v.size == 2);
  assert_int(&v, 0, 10);

  vector_free(&v);
}

static void test_set() {
  Vector v;
  setup_char(&v);

  char d = 'j';

  vector_set(&v, 1, &d);

  assert(v.size == 3);
  assert(*(char*)vector_at(&v, 1) == d);

  vector_free(&v);
}

void run_modifiers_tests() {
  test_push();
  test_remove();
  test_remove_first_and_last();
  test_remove_range();
  test_insert();
  test_pop();
  test_set();
}

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

static void assert_int(struct Vector* v, size_t index, int expected);
static void assert_char(struct Vector* v, size_t index, char expected);
static bool compare_int(void* element, void* target);

/* === Element Acess Tests ================================================= */
void test_vector_get() {
  struct Vector v;
  vector_init(&v, 1, sizeof(char));

  char a = 'a', b = 's', c = 'd';

  vector_push(&v, &a);

  char get_char;
  vector_get(&v, 0, &get_char);

  vector_push(&v, &b);
  vector_push(&v, &c);

  assert(v.size == 3);
  assert(get_char == *(char*)vector_at(&v, 0));

  assert(*(char*)vector_at(&v, 1) == 's');
  assert(*(char*)vector_at(&v, 2) == 'd');
  
  vector_free(&v);
}

void test_begin() {
  struct Vector v;
  vector_init(&v, 1, sizeof(int));

  int i = 1, o = 2, p = 3;

  vector_push(&v, &i);
  vector_push(&v, &o);
  vector_push(&v, &p);

  int begin = *(int*)vector_begin(&v);

  assert(v.size == 3);
  assert(begin == i);

  vector_free(&v);
}

void test_end() {
  struct Vector v;
  vector_init(&v, 1, sizeof(char));

  assert(vector_begin(&v) == vector_end(&v));

  char a = 'a', b = 's', c = 'd';

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

void test_find() {
  struct Vector v;
  vector_init(&v, 1, sizeof(int));

  int a = 10, b = 20, c = 30;

  vector_push(&v, &a);
  vector_push(&v, &b);
  vector_push(&v, &c);

  void* src = vector_find(&v, &b, compare_int);

  assert(src != NULL);

  int value = *(int*)src;

  assert(value == b);

  assert(src == vector_at(&v, 1));

  int d = 99;
  assert(vector_find(&v, &d, compare_int) == NULL);

  vector_free(&v);
}

/* === Modifiers Tests ================================================= */
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

void test_remove_first_and_last() {
  struct Vector v;
  vector_init(&v, 2, sizeof(char));
  
  char a = 'a', b = 's', c = 'd';

  vector_push(&v, &a);
  vector_push(&v, &b);
  vector_push(&v, &c);

  vector_remove(&v, 0);
  //0 -> removed, last element back 1 index. 2 - 1 = 1.
  vector_remove(&v, 1);
 
  assert(v.size == 1); 
  assert_char(&v, 0, b);

  vector_free(&v);
}

void insert_test() {
  struct Vector v;
  vector_init(&v, 2, sizeof(char));
  
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

void test_set() {
  struct Vector v;
  vector_init(&v, 1, sizeof(char));

  char a = 'f', b = 'g', c = 'h', d = 'j';

  vector_push(&v, &a);
  vector_push(&v, &b);
  vector_push(&v, &c);

  vector_set(&v, 1, &d);

  assert(v.size == 3);
  assert(*(char*)vector_at(&v, 1) == d);

  vector_free(&v);
}

/* === Stress Tests ================================================= */
void test_push_stress() {
  struct Vector v;
  vector_init(&v, 1, sizeof(int));

  int N = 100000;

  for (int i = 0; i < N; i++) {
    vector_push(&v, &i);
  }

  assert(v.size = N);

  for (int i = 0; i < N; i++) {
    assert_int(&v, i, i);
  } 

  vector_free(&v);
}

void test_random_operations() {
  struct Vector v;
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

int main() {
  //Element Acess
  test_vector_get();
  test_begin();
  test_end();

  //Modifiers
  test_push();
  test_remove();
  test_remove_first_and_last();
  insert_test();
  test_pop();
  test_set();

  //Stress
  test_push_stress();
  test_random_operations();

  printf("All tests passed\n");

  return 0;
}

static void assert_int(struct Vector* v, size_t index, int expected) {
  const int* n = vector_at(v, index);
  assert(n != NULL);
  assert(*n == expected);
}

static void assert_char(struct Vector* v, size_t index, char expected) {
  const char* n = vector_at(v, index);
  assert(n != NULL);
  assert(*n == expected);
}

static bool compare_int(void* element, void* target) {
  return *(int*)element == *(int*)target;
}

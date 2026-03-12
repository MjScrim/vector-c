#include <assert.h>
#include <stdio.h>
#include "vector.h"

static void assert_int(struct Vector* v, size_t index, int expected);
static void assert_char(struct Vector* v, size_t index, char expected);

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
  //Modifiers
  test_push();
  test_remove();
  test_pop();
  insert_test();
  test_remove_first_and_last();

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

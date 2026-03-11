#include "vector.h"

void print_int(void *p) {
    int *x = (int*)p;
    printf("%d ", *x);
}

int main() {
  struct Vector v;

  vector_init(&v, 2, sizeof(int));

  int a = 10;
  int b = 20;
  int c = 30;
  int d = 15;

  vector_push(&v, &a);
  vector_push(&v, &b);
  vector_push(&v, &c); 

  vector_insert(&v, &d, 1);
  
  vector_print(&v, print_int);

  vector_free(&v);

  return 0;
}

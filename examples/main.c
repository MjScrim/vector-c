#include "vector.h"

void print_int(void *p) {
    int *x = (int*)p;
    printf("%d ", *x);
}

bool search_number(void* a, void* b) {
    return *(int*)a == *(int*)b;
}

int main() {
  Vector v;
  vector_init(&v, 2, sizeof(int));

  VECTOR_PUSH(&v, int, 10);
  VECTOR_PUSH(&v, int, 20);
  VECTOR_INSERT(&v, int, 99, 1);
  VECTOR_SET(&v, int, 100, 0);

  for (size_t i = 0; i < v.size; i++) {
    printf("v[%zu] = %d\n", i, VECTOR_AT(&v, int, i));
  }

  int* target = VECTOR_FIND(&v, int, 100, search_number);
  if(target) {
    printf("%d\n", *target);
  }

  vector_free(&v);

  return 0;
}

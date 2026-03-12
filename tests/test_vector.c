#include <assert.h>
#include "vector.h"

int main() {
  struct Vector v;

    vector_init(&v, 2, sizeof(int));

    int a = 10;
    int b = 20;

    vector_push(&v, &a);
    vector_push(&v, &b);

    const int *x = vector_at(&v, 0);
    const int *y = vector_at(&v, 1);

    assert(*x == 10);
    assert(*y == 20);

    assert(v.size == 2);

    vector_free(&v);

    return 0;
}

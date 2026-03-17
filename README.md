# vector-c
![Build and Test](https://github.com/MjScrim/vector-c/actions/workflows/ci.yml/badge.svg)

A generic dynamic array (vector) implementation in C.

This project was built as a learning exercise to better understand
memory layout, pointer arithmetic, and how dynamic containers work
internally.

## Features

- Generic storage using `void*`
- Automatic resizing
- Push, pop and reserve operations
- Remove elements by index
- Custom element printing via function pointers

## Example

```c
#include "vector.h"

void print_int(void *p) {
    int *x = (int*)p;
    printf("%d ", *x);
}

bool search_number(void* a, void* b) {
    return *(int*)a == *(int*)b;
}

int main() {
    __free Vector v;
    VectorStatus status = vector_init(&v, 2, sizeof(int));

    if (status != VECTOR_SUCCESS)
        return 1;

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

    return 0;
}
```

## Build and Run

You can build this project using either **CMake** (recommended) or the provided **Makefile**.

### Using CMake via Makefile script

1. Generate the build directory:
   ```bash
   make setup
   ```
2. Compile the project:
    ```bash
   make build-cmake
   ```
3. Compile and run the compiled example:
    ```bash
   make run-cmake
   ```
4. Run tests:
    ```bash
    make ctest
    ```
5. Build and run test:
    ```bash
    make check
    ```

### Using only Makefile

1. Compile the project:
   ```bash
   make
   ```
2. Run example:
    ```bash
   make run
   ```
3. Run tests:
    ```bash
   make test
   ```
4. Clean the build files:
    ```bash
   make clean
   ```

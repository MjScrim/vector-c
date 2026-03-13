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
Vector v;

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

# vector-c

A generic dynamic array (vector) implementation in C.

This project was built as a learning exercise to better understand
memory layout, pointer arithmetic, and how dynamic containers work
internally.

## Features

- Generic storage using `void*`
- Automatic resizing
- Push and pop operations
- Remove elements by index
- Custom element printing via function pointers

## Example

```c
struct Vector v;

vector_init(&v, 2, sizeof(int));

int a = 10;
int b = 20;
int c = 30;

vector_push(&v, &a);
vector_push(&v, &b);
vector_push(&v, &c);

vector_print(&v, print_int);

vector_remove(&v, 2);

vector_print(&v, print_int);

vector_free(&v);

#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "vector.h"

#define ASSERT_OK(func) \
    do { \
        VectorStatus status_ = (func); \
        assert(status_ == VECTOR_SUCCESS); \
    } while (0)

void setup_int(Vector* v);
void setup_char(Vector* v);
void assert_int(Vector* v, size_t index, int expected);
void assert_char(Vector* v, size_t index, char expected);
bool compare_int(void* element, void* target);
void double_value(void* n);

void run_capacity_tests();
void run_access_tests();
void run_modifiers_tests();
void run_stress_tests();
void run_utils_tests();

#endif

#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "vector.h"

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

#endif

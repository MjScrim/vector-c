#include "test_helpers.h"
#include <stdio.h>

int main() {
  printf("Running Vector Tests...\n");

  run_capacity_tests();
  run_access_tests();
  run_modifiers_tests();
  run_stress_tests();

  return 0;
}

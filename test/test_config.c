
#include "unity/unity.h"

#include "../src/hamon/headers/config.h"

void setUp() {}

void tearDown() {}

void test_gen() { gen_default_config(); }

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_gen);

  UNITY_END();
  return 0;
}

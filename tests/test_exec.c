// DEPS=("exec")

#include "unity/unity.h"

#include "../src/hamon/headers/exec.h"

void setUp() {}

void tearDown() {}

char *envp[1] = {0};

void test_exec(void) {
  char *argv0[3] = {"touch", "file.txt", 0};
  int status = 0;

  int status0 = execute(argv0[0], argv0, status, envp);
  TEST_ASSERT_EQUAL_INT8(0, status0);

  remove("file.txt");
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_exec);

  UNITY_END();
  return 0;
}

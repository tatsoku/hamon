#include "../src/hamon/headers/builtins.h"
#include "unity/unity.h"

void setUp() {}

void tearDown() {}

void test_echo() {
  char *secondary_args[2] = {"-n", "-E"};
  char *test_messages[3] = {"no flag", "with -n", "with -E"};
  char *argv[3] = {"echo"};
  char *test_message = "Test message!";

  printf("Running echo with: %s\n", test_messages[0]);

  argv[2] = 0;
  argv[1] = test_message;
  builtin_echo(2, argv);

  for (int test = 0; test < 2; test++) {
    printf("Running echo with: %s\n", test_messages[test + 1]);
    argv[1] = secondary_args[test];
    argv[2] = test_message;
    builtin_echo(3, argv);
  }
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_echo);

  UNITY_END();
  return 0;
}

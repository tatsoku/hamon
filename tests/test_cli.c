// DEPS=("cli")

#include "unity/unity.h"
#include "unity/unity_internals.h"

#define COLORS

#include "../src/hamon/headers/cli.h"
#include "../src/hamon/headers/escape.h"

void setUp(void) {}

void tearDown(void) {}

void test_help(void) {
  char *argv0[2] = {"hsh", "--help"};
  char *argv1[2] = {"hsh", "-h"};
  char *argv2[3] = {"hsh", "--help", "gabagool"};

  printf(BLUE ">%s Running %s with --help\n", CLEAR, argv0[0]);
  int status0 = process_args(2, argv0);
  TEST_ASSERT_EQUAL_INT8(0, status0);

  printf(BLUE ">%s Running %s with -h\n", CLEAR, argv0[0]);
  int status1 = process_args(2, argv1);
  TEST_ASSERT_EQUAL_INT8(0, status1);

  printf(BLUE ">%s Running %s with --help with extra args\n", CLEAR, argv0[0]);
  int status2 = process_args(3, argv2);
  TEST_ASSERT_EQUAL_INT8(1, status2);
}

void test_version(void) {
  char *argv0[2] = {"hsh", "--version"};
  char *argv1[2] = {"hsh", "-v"};
  char *argv2[3] = {"hsh", "--version", "gabagool"};

  printf(BLUE ">%s Running %s with --version\n", CLEAR, argv0[0]);
  int status0 = process_args(2, argv0);
  TEST_ASSERT_EQUAL_INT8(0, status0);

  printf(BLUE ">%s Running %s with -v\n", CLEAR, argv0[0]);
  int status1 = process_args(2, argv1);
  TEST_ASSERT_EQUAL_INT8(0, status1);

  printf(BLUE ">%s Running %s with --version with extra args\n", CLEAR,
         argv0[0]);
  int status2 = process_args(3, argv2);
  TEST_ASSERT_EQUAL_INT8(1, status2);
}

/*
 * todo: create a template config to test with.
 * todo: Once configuration actually works.
 * todo: Aswell as check for nonexistent paths.
 */

void test_config(void) {
  char *argv0[3] = {"hsh", "--config", "foo"};
  char *argv1[3] = {"hsh", "-c", "foo"};
  char *argv2[2] = {"hsh", "--config"};

  printf(BLUE ">%s Running %s with --config\n", CLEAR, argv0[0]);
  int status0 = process_args(3, argv0);
  TEST_ASSERT_EQUAL_INT8(0, status0);

  printf(BLUE ">%s Running %s with -c\n", CLEAR, argv0[0]);
  int status1 = process_args(3, argv1);
  TEST_ASSERT_EQUAL_INT8(0, status1);

  printf(BLUE ">%s Running %s with --config without a file\n", CLEAR, argv0[0]);
  int status2 = process_args(2, argv2);
  TEST_ASSERT_EQUAL_INT8(1, status2);
}

void test_incorrect_flag(void) {
  char *argv0[2] = {"hsh", "--gabagool"};

  printf(BLUE ">%s Running %s with an incorrect flag\n", CLEAR, argv0[0]);
  int status0 = process_args(2, argv0);
  TEST_ASSERT_EQUAL_INT8(1, status0);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_help);
  RUN_TEST(test_version);
  RUN_TEST(test_config);
  RUN_TEST(test_incorrect_flag);

  UNITY_END();
  return 0;
}

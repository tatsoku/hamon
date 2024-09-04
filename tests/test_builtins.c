// DEPS=("builtins")

#define COLORS
#include "../src/hamon/headers/builtins.h"
#include "../src/hamon/headers/escape.h"
#include "unity/unity.h"

char *envp[1] = {0};

void setUp(void) {}

void tearDown(void) {}

void test_echo(void) {

  char *test_message = "Test message!";
  char *argv0[2] = {"echo", test_message};
  char *argv1[3] = {"echo", "-n", test_message};
  char *argv2[3] = {"echo", "-E", test_message};
  char *argv3[4] = {"echo", "-E", "-n", test_message};

  printf(BLUE ">%s Running echo with no flags\n", CLEAR);
  int status0 = builtin_echo(2, argv0, envp);
  TEST_ASSERT_EQUAL_INT8(0, status0);

  printf(BLUE ">%s Running echo with -n\n", CLEAR);
  int status1 = builtin_echo(3, argv1, envp);
  TEST_ASSERT_EQUAL_INT8(0, status1);

  printf(BLUE ">%s Running echo with -E\n", CLEAR);
  int status2 = builtin_echo(3, argv2, envp);
  TEST_ASSERT_EQUAL_INT8(0, status2);

  printf(BLUE ">%s Running echo with too many args\n", CLEAR);
  int status3 = builtin_echo(4, argv3, envp);
  TEST_ASSERT_EQUAL_INT8(1, status3);
}

void test_help(void) {
  char *argv0[1] = {"help"};
  char *argv1[2] = {"help", "-h"};
  char *argv2[3] = {"help", "-h", "--help"};

  printf(BLUE ">%s Running help without args.\n", CLEAR);
  int status0 = builtin_help(1, argv0, envp);
  TEST_ASSERT_EQUAL_INT8(0, status0);

  printf(BLUE ">%s Running help with 1 arg of -h\n", CLEAR);
  int status1 = builtin_help(2, argv1, envp);
  TEST_ASSERT_EQUAL_INT8(0, status1);

  printf(BLUE ">%s Running help with too many args.\n", CLEAR);
  int status2 = builtin_help(3, argv2, envp);
  TEST_ASSERT_EQUAL_INT8(1, status2);
}

void test_pwd(void) {
  char *argv0[1] = {"pwd"};
  char *argv1[2] = {"pwd"
                    "useless-argument"};

  printf(BLUE ">%s Running pwd like normal.\n", CLEAR);
  int status0 = builtin_pwd(1, argv0, envp);
  TEST_ASSERT_EQUAL_INT8(0, status0);

  printf(BLUE ">%s Running pwd with args (pwd shouldn't have any args).\n",
         CLEAR);
  int status1 = builtin_pwd(2, argv1, envp);
  TEST_ASSERT_EQUAL_INT8(1, status1);
}

void test_cd(void) {
  char *pwd[1] = {"pwd"};
  char *argv0[2] = {"cd", ".."};
  char *argv1[2] = {"cd", "bin"};
  char *argv2[2] = {"cd", "-"};
  char *argv3[2] = {"cd", "~"};
  char *argv4[1] = {"cd"};
  char *argv5[3] = {"cd", "foo", "bar"};

  builtin_pwd(1, pwd, envp);
  printf(BLUE ">%s Changing directory to ..\n", CLEAR);
  int status0 = builtin_cd(2, argv0, envp);
  TEST_ASSERT_EQUAL_INT8(0, status0);
  builtin_pwd(1, pwd, envp);

  printf(BLUE ">%s Changing directory to bin\n", CLEAR);
  int status1 = builtin_cd(2, argv1, envp);
  TEST_ASSERT_EQUAL_INT8(0, status1);
  builtin_pwd(1, pwd, envp);

  printf(BLUE ">%s Changing directory to -\n", CLEAR);
  int status2 = builtin_cd(2, argv2, envp);
  TEST_ASSERT_EQUAL_INT8(0, status2);
  builtin_pwd(1, pwd, envp);

  printf(BLUE ">%s Changing directory to ~\n", CLEAR);
  int status3 = builtin_cd(2, argv3, envp);
  TEST_ASSERT_EQUAL_INT8(0, status3);
  builtin_pwd(1, pwd, envp);

  // restore path
  builtin_cd(2, argv2, envp);
  builtin_pwd(1, pwd, envp);

  printf(BLUE ">%s Running cd with too little arguments\n", CLEAR);
  int status4 = builtin_cd(1, argv4, envp);
  TEST_ASSERT_EQUAL_INT8(1, status4);

  printf(BLUE ">%s Running cd with too many arguments\n", CLEAR);
  int status5 = builtin_cd(3, argv5, envp);
  TEST_ASSERT_EQUAL_INT8(1, status5);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_echo);
  RUN_TEST(test_help);
  RUN_TEST(test_pwd);
  RUN_TEST(test_cd);

  UNITY_END();
  return 0;
}

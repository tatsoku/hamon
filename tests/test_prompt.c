// DEPS=("prompt" "builtins" "exec")

#include <stdio.h>
#include <stdlib.h>

#include "unity/unity.h"

#define COLORS
#define GRAPHICS

#include <hamon_escape.h>
#include <hamon_prompt.h>

char *username = {0};
char *hostname = {0};

void setUp() {}

void tearDown() {}

void test_username() {
  username = get_username();
  TEST_ASSERT_NOT_NULL(username);

  printf("[%sTEST%s] <%stest_username%s> %s %s username: %s\n", BLUE, CLEAR,
         RED, CLEAR, STRIKETHROUGH, CLEAR, username);
  free(username);
}

void test_hostname() {
  hostname = get_hostname();
  TEST_ASSERT_NOT_NULL(hostname);

  printf("[%sTEST%s] <%stest_hostname%s> %s %s hostname: %s\n", BLUE, CLEAR,
         RED, CLEAR, STRIKETHROUGH, CLEAR, hostname);
  free(hostname);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_username);
  RUN_TEST(test_hostname);

  UNITY_END();
  return 0;
}

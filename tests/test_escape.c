// DEPS=("escape")

#include <stdlib.h>

#define COLORS
#define GRAPHICS

#include "../src/hamon/headers/escape.h"
#include "unity/unity.h"

char *codes[1] = {CYAN_CODE};
char *assembled_escape = {0};

void setUp() {};
void tearDown() { free(assembled_escape); };

void test_assemble(void) {
  assembled_escape = assemble(codes, 1);
  TEST_ASSERT_NOT_NULL(assembled_escape);

  printf("[%sTEST%s] <%stest_assemble%s> %s %s %sThis is what the escape "
         "looks like on your terminal!\n",
         BLUE, CLEAR, RED, CLEAR, STRIKETHROUGH, CLEAR, assembled_escape);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_assemble);

  UNITY_END();
}

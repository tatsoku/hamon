#define COLORS
#define GRAPHICS

#include "../src/hamon/headers/escape.h"
#include "unity/unity.h"

char *codes[1] = {CYAN_CODE};
char *assembled_escape = {0};

void setUp() { assembled_escape = assemble(codes, 1); };
void tearDown() { free(assembled_escape); };

void test_assemble(void) {
  printf("\n%sThis is what the escape looks like on your terminal! \n" CLEAR,
         assembled_escape);
  TEST_ASSERT_EQUAL_INT8(1, 2);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_assemble);
  UNITY_END();
}

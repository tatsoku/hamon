// DEPS=("file")

#include "unity/unity.h"

#define COLORS
#define GRAPHICS

#include "../src/hamon/headers/escape.h"
#include "../src/hamon/headers/file.h"

char *folder = "./test_folder/";
char *file = "./testfile.txt";
char *file_content = "test";
char *file_buf = {0};

void setUp(void) {}

void tearDown(void) {}

void test_write_file(void) {
  int status = write_file(file, file_content, 4);
  TEST_ASSERT_EQUAL_INT8(1, status);
}

void test_read_file(void) {
  file_buf = read_file(file);
  TEST_ASSERT_NOT_NULL(file_buf);
  printf("[%sTEST%s] <%stest_read_file%s> %s %s read contents: %s\n", BLUE,
         CLEAR, RED, CLEAR, STRIKETHROUGH, CLEAR, file_buf);
}

void test_compare_file_contents(void) {
  int status = compare_file_contents(file, file_content, 4);
  TEST_ASSERT_EQUAL_INT8(1, status);
}

void test_create_folder(void) {
  int status = create_folder(folder);
  TEST_ASSERT_EQUAL_INT8(1, status);
}

void test_check_folder(void) {
  int status = check_if_folder_exists(folder);
  TEST_ASSERT_EQUAL_INT8(1, status);
}

void test_remove_folder(void) {
  int status = remove_folder(folder);
  TEST_ASSERT_EQUAL_INT8(1, status);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_write_file);
  RUN_TEST(test_read_file);
  RUN_TEST(test_compare_file_contents);
  RUN_TEST(test_create_folder);
  RUN_TEST(test_check_folder);
  RUN_TEST(test_remove_folder);

  UNITY_END();
  return 0;
}

// DEPS=("config" "file" "error")

#ifdef _WIN32
#include <hamon_error.h>
#include <windows.h>
#endif

#include "unity/unity.h"
#include <stdlib.h>

#include <hamon_config.h>
#include <hamon_file.h>

#ifdef _WIN32
char folder_buffer[MAX_PATH] = {0};
char absolute_path_buffer[MAX_PATH] = {0};
#endif

#ifdef __linux__
char folder_buffer[1024] = {0};
char absolute_path_buffer[1024] = {0};
#endif

void setUp(void) {

#ifdef _WIN32
  int res1 =
      snprintf(folder_buffer, MAX_PATH, "%s\\hamon\\", getenv("APPDATA"));
  if (res1 < 0 || res1 >= MAX_PATH) {
    win_perror("snprintf");
    return;
  }

  int res2 =
      snprintf(absolute_path_buffer, MAX_PATH, "%sconfig.toml", folder_buffer);
  if (res2 < 0 || res2 >= 1024) {
    win_perror("snprintf");
    return;
  }

  printf("Folder: %s\n", folder_buffer);
  printf("Path: %s\n", absolute_path_buffer);
#elif __linux__
  int res1 = snprintf(folder_buffer, 1024, "%s/.config/hamon/", getenv("HOME"));
  if (res1 < 0 || res1 >= 1024) {
    perror("snprintf");
    return;
  }

  int res2 =
      snprintf(absolute_path_buffer, 1024, "%sconfig.toml", folder_buffer);
  if (res2 < 0 || res2 >= 1024) {
    perror("snprintf");
    return;
  }
#else
#error "Use a better operating system, loser
#endif
}

void tearDown(void) {}

void test_gen(void) {
  printf("Folder: %s\n", folder_buffer);
  int status0 = gen_default_config();

  TEST_ASSERT_EQUAL_INT8(1, status0);

  int status1 = check_if_folder_exists(folder_buffer);
  TEST_ASSERT_EQUAL_INT8(1, status1);

  int status2 = remove_folder(folder_buffer);
  TEST_ASSERT_EQUAL_INT8(1, status2);
}

/*
 * todo: Write test function and way to test if config read and interpret is
 * correct.
 */

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_gen);

  UNITY_END();
  return 0;
}

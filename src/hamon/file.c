
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32

#include <io.h>

#endif

#define COLORS

#include "headers/escape.h"
#include "headers/file.h"

int write_file(const char *filename, const char *content, const size_t size) {
  FILE *file = fopen(filename, "w");
  if (!file) {
    perror("Failed to open file");
    fclose(file);
    return 1;
  }

  fwrite(content, size, 1, file);
  fclose(file);
  return 0;
}

int check_if_folder_exists(const char *folder_path) {
  struct stat sb;

  if (stat(folder_path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
    // success!
    return 1;
  }
  return 0;
}

int create_folder(const char *folder_path) {
#ifdef __linux__
  if (mkdir(folder_path, 0700) == -1) {
    perror("Failed to create directory");
    return 0;
  }
#elif _WIN32
  if (_mkdir(dir_name) == -1) {
    perror("Failed to create directory");
    return 0;
  }
#else
#error "Use a better operating system, loser"
#endif
  return 1;
}

char *read_file(const char *file_path) {
  FILE *file = fopen(file_path, "r");
  size_t file_len = 0;

  if (!file) {
    perror("Failed to open file");
    fclose(file);
    return 0;
  }

  fseek(file, 0L, SEEK_END);
  file_len = ftell(file);
  rewind(file);

  if (file_len <= 0) {
    fprintf(stderr,
            RED "!%s Failed to read file length, can't read file. exiting..\n",
            CLEAR);
    fclose(file);
    return 0;
  }

  size_t buffer_size = file_len;
  char *buffer = (char *)malloc(buffer_size);
  if (!buffer) {
    perror("malloc");
    fclose(file);
    return 0;
  }

  size_t bytes_read = 0;
  bytes_read = fread(buffer, 1, buffer_size, file);
  if (bytes_read != buffer_size) {
    fprintf(stderr,
            RED "!%s Did not read the whole file.. Something definitely went "
                "very wrong.\n",
            CLEAR);
    free(buffer);
    fclose(file);
    return 0;
  }

  fclose(file);
  return buffer;
}

int compare_file_contents(const char *file_path, char *contents) {
  char *file_content = read_file(file_path);

  if (strcmp(contents, file_content) == 0) {
    return 1;
  }

  return 0;
}

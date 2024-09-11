#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32

#include "../../include/hamon_strl.h"

#include <windows.h>
#include <fileapi.h>
#include <io.h>
#include <winnt.h>

#include "../../include/hamon_error.h"

#elif __linux__

#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#else
#error Use a better operating system, loser
#endif

#define COLORS

#include "../../include/hamon_escape.h"
#include "../../include/hamon_file.h"

#ifdef _WIN32
void *convert_string_to_wide(const char *str) {
  int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
  wchar_t *wstr = (wchar_t *)malloc(len * sizeof(wchar_t));
  MultiByteToWideChar(CP_ACP, 0, str, -1, wstr, len);
  return wstr;
}
#endif

int write_file(const char *filename, const char *content) {
  FILE *file = fopen(filename, "w");
  if (!file) {
    perror("Failed to open file");
    fclose(file);
    return -1;
  }

  fwrite(content, sizeof(char), strlen(content), file);

  fseek(file, 0, SEEK_END);
  fputc('\0', file);

  fclose(file);
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

  char *buffer = (char *)malloc(file_len);
  if (!buffer) {
    perror("malloc");
    fclose(file);
    return 0;
  }

  size_t bytes_read = 0;
  bytes_read = fread(buffer, 1, file_len, file);
  if (bytes_read != file_len) {
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

int compare_file_contents(const char *file_path, char *contents,
                          size_t content_len) {
  char *file_content = read_file(file_path);
  if (file_content == 0)
    return -1;

  if (strncmp(contents, file_content, content_len) == 0) {
    return 1;
  }

  return -1;
}

int remove_file(const char *file_path) {
#ifdef __linux__
  if (remove(file_path))
    return -1;
#elif _WIN32
  LPCWSTR wfile_path = (LPCWSTR)convert_string_to_wide(file_path);
  if (DeleteFileW(wfile_path) == FALSE) {
    win_perror("DeleteFile");
    free((void *)wfile_path);
    return -1;
  }
  free((void *)wfile_path);
#else
#error Use a better operating system, loser
#endif
  return 1;
}

int create_folder(const char *folder_path) {
#ifdef __linux__
  if (mkdir(folder_path, 0700) == -1) {
    perror("Failed to create directory");
    return -1;
  }
#elif _WIN32
  LPSECURITY_ATTRIBUTES sa_attr = NULL;
  LPCWSTR wfolder_path = (LPCWSTR)convert_string_to_wide(folder_path);
  if (CreateDirectoryW(wfolder_path, sa_attr) == -1) {
    win_perror("Failed to create directory");
    free((void *)wfolder_path);
    return -1;
  }
  free((void *)wfolder_path);
#else
#error "Use a better operating system, loser"
#endif
  return 1;
}

int check_if_folder_exists(const char *folder_path) {
#ifdef __linux__
  struct stat sb;

  if (stat(folder_path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
    // success!
    return 1;
  }
  return -1;
#elif _WIN32
  LPCWSTR wfolder_path = (LPCWSTR)convert_string_to_wide(folder_path);
  DWORD dw_attrib = GetFileAttributesW(wfolder_path);
  if (dw_attrib == INVALID_FILE_ATTRIBUTES) {
    return -1; // Error occurred
  }
  free((void *)wfolder_path);
  return (dw_attrib & FILE_ATTRIBUTE_DIRECTORY) ? 1 : -1;
#endif
}

int remove_folder(const char *path) {
#ifdef __linux__
  DIR *dir;
  struct dirent *ent;

  if ((dir = opendir(path)) != 0) {
    while ((ent = readdir(dir)) != 0) {
      if (strncmp(ent->d_name, ".", 1) == 0 ||
          strncmp(ent->d_name, "..", 2) == 0)
        continue;

      char full_path[1024] = {0};
      snprintf(full_path, sizeof(full_path), "%s/%s", path, ent->d_name);

      if (check_if_folder_exists(full_path))
        remove_folder(full_path);
      else
        remove(full_path);
    }
    closedir(dir);
  }

#elif _WIN32
  WIN32_FIND_DATA findFileData;
  HANDLE hFind = INVALID_HANDLE_VALUE;

  char canon_path[MAX_PATH] = {0};
  GetFullPathNameA(path, MAX_PATH, canon_path, NULL);
  snprintf(canon_path, sizeof(canon_path), "%s\\*", canon_path);

  hFind = FindFirstFileA(canon_path, &findFileData);
  if (hFind == INVALID_HANDLE_VALUE) {
    win_perror("FindFirstFile");
    return -1;
  }

  do {
    if (strncmp(findFileData.cFileName, ".", 1) != 0 &&
        strncmp(findFileData.cFileName, "..", 2) != 0) {
      char full_path[MAX_PATH] = {0};
      snprintf(full_path, sizeof(full_path), "%s\\%s", path,
               findFileData.cFileName);

      if (check_if_folder_exists(full_path)) {
        remove_folder(full_path);
      } else {
        LPCWSTR wfull_path = (LPCWSTR)convert_string_to_wide(full_path);
        wprintf(L"Removing file: %s\n", wfull_path);
        BOOL remove_status = RemoveDirectoryW(wfull_path);
        if (remove_status == FALSE) {
          win_perror("RemoveDirectory");
          free((void *)wfull_path);
          return -1;
        }
        free((void *)wfull_path);
      }
    }
  } while (FindNextFile(hFind, &findFileData));

  FindClose(hFind);
#else
#error Use a real operating system, loser
#endif

#ifdef _WIN32
  LPCWSTR wpath = (LPCWSTR)convert_string_to_wide(path);
  if (RemoveDirectoryW(wpath) == FALSE) {
    free((void *)wpath);
    return -1;
  }
  free((void *)wpath);
#elif __linux__
  if (remove(path))
    return -1;
#endif
  return 1;
}

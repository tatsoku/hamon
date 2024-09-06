#ifndef FILE_H
#define FILE_H

#include <stdio.h>

int write_file(const char *filename, const char *content);
int check_if_folder_exists(const char *folder_path);
int compare_file_contents(const char *file_path, char *contents,
                          size_t content_len);
int remove_file(const char *file_path);
int create_folder(const char *folder_path);
int remove_folder(const char *path);
char *read_file(const char *file_path);

#endif

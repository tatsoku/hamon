#ifndef FILE_H
#define FILE_H

#include <stdio.h>

int write_file(const char *filename, const char *content, const size_t size);
int check_if_folder_exists(const char *folder_path);
int create_folder(const char *folder_path);
char *read_file(const char *file_path);

#endif

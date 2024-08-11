#ifndef PROMPT_H
#define PROMPT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#include <unistd.h>

#elif _WIN32
#include <windows.h>

#else
#error "Use a better operating system, loser"

#endif

char *get_hostname();
char *get_username();
int init_prompt();

#endif // !PROMPT_H

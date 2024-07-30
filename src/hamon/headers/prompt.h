#ifndef PROMPT_H
#define PROMPT_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#else
#error "Use a better operating system, loser"
#endif

enum PromptType{
  Default,
  Custom
};

char *get_hostname();
char *get_username();
int init_prompt();

#endif // !PROMPT_H

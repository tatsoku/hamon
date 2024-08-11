#ifndef EXEC_H
#define EXEC_H

#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__

#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

#elif _WIN32

#include <windows.h>

#endif

int execute(char *executable, char **argv, int status);

#endif // !EXEC_H

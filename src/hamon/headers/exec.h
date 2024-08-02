#ifndef EXEC_H
#define EXEC_H

#include <stdlib.h>
#include <stdio.h>

#if __linux__

#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#elif _WIN32

#include <windows.h>

#endif

int execute(char *executable, char**argv, int status);

#endif // !EXEC_H

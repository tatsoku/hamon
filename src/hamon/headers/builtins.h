#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int builtin_echo(int argc, char *argv[]);
int builtin_exit(int argc, char *argv[]);
int builtin_cd(int argc, char *argv[]);
int builtin_help(int argc, char *argv[]);
int check_builtins(int argc, char *argv[]);

#endif // !BUILTINS_H

#ifndef BUILTINS_H
#define BUILTINS_H

int builtin_echo(int argc, char *argv[], char *const *envp);
int builtin_exit(int argc, char *argv[], char *const *envp);
int builtin_cd(int argc, char *argv[], char *const *envp);
int builtin_pwd(int argc, char *argv[], char *const *envp);
int builtin_help(int argc, char *argv[], char *const *envp);
int check_builtins(int argc, char *argv[], char *const *envp);

#endif // !BUILTINS_H

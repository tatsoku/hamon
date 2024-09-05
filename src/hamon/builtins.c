#define COLORS
#include <hamon_builtins.h>

#include <dirent.h>
#include <libgen.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#ifdef __linux__

#include <string.h>
#include <unistd.h>

#elif _WIN32

#include <bsd/string.h>
#include <windows.h>

#include <hamon_error.h>

#else
#error Get a better operating system, loser.
#endif

enum BuiltinType { Echo, Exit, Cd, Pwd, Help };

int print_builtin_help(enum BuiltinType builtin) {
  switch (builtin) {
  case Echo:
    printf("echo: print to stdout\n\n"

           "echo: USAGE\n"
           "echo [optional flag] [message]\n\n"

           "-h --help  Writes this message\n"
           "-E         Write to stdout without any escapes whatsoever.\n"
           "-n         Write to stdout without a trailing \\n character.\n\n"

           "hamon! made with <3\n");
    break;
  case Exit:
    printf("exit: exits the current hamon session\n\n"

           "exit: USAGE\n"
           "exit [code]\n\n"

           "Exits with 0 by default.\n"

           "hamon! made with <3\n");
    break;
  case Cd:
    printf("cd: change directory\n\n"

           "cd: USAGE\n"
           "cd [dir]\n\n"

           "Allows certain special tokens for special functionality.\n"
           "~  represents $HOME\n"
           "-  represents the previous directory\n"
           ".. represents the relative parent directory\n\n"

           "hamon! made with <3\n");
    break;
  case Pwd:
    printf("pwd: print working directory\n\n"

           "pwd: USAGE\n"
           "pwd\n\n"

           "If you somehow find this help text, you must be either artistic\n"
           "or stupid enough to think pwd will accept arguments\n\n"

           "hamon! made with <3\n");
    break;
  case Help:
    printf(
        "help: show builtin commands\n\n"

        "help: USAGE\n"
        "help [flags]\n\n"

        "-h --help Writes this message\n\n I genuinely don't know what you're "
        "doing to even get this message to appear..\n\n"

        "hamon! made with <3\n");
    break;
  }

  return 1;
}

int builtin_echo(int argc, char *argv[], char *const *envp) {
  if (argc > 3 || argc < 2)
    return print_builtin_help(Echo);

  char *message = {0};

  if (argc > 2) {
    message = argv[2];
    if (strncmp(argv[1], "-E", 2) == 0) {
      fwrite(message, 1, strlen(argv[2]), stdout);
      return 0;
    }

    if (strncmp(argv[1], "-n", 2) == 0) {
      printf("%s", message);
      return 0;
    }

    if (strncmp(argv[1], "-h", 2) == 0 || strncmp(argv[1], "--help", 6) == 0) {
      print_builtin_help(Echo);
      return 0;
    }
  }

  message = argv[1];

  puts(message);
  return 0;
}

int builtin_exit(int argc, char *argv[], char *const *envp) {
  int code = 0;

  if (argc > 1) {
    code = atoi(argv[1]);
  }

  exit(code);
  return code;
}

#ifdef __linux__
char last_dir[1024] = {0};
#elif _WIN32
char last_dir[MAX_PATH] = {0};
#else
#error "Get a better operating system, loser"
#endif

int builtin_cd(int argc, char *argv[], char *const *envp) {
  if (argc > 2 || argc < 2)
    return print_builtin_help(Cd);

  char path[1024] = {0};
  strlcpy(path, argv[1], 1024);

#ifdef __linux__
  char cwd_buf[1024] = {0};
  char *cwd_ptr = {0};

  cwd_ptr = getcwd(cwd_buf, 1024);
  printf("cwd_ptr: %s\n", cwd_ptr);

  if (strncmp(path, "~", 1) == 0) {
    strlcpy(path, getenv("HOME"), 1024);
  } else if (strncmp(path, "-", 1) == 0) {
    strlcpy(path, last_dir, 1024);
  }

  if (chdir(path)) {
    perror("Couldn't change directory");
    return 1;
  }

  strlcpy(last_dir, cwd_buf, 1024);
#elif _WIN32
  char cwd_buf[MAX_PATH] = {0};

  GetCurrentDirectory(MAX_PATH, (LPWSTR)cwd_buf);
  strlcpy(last_dir, cwd_buf, MAX_PATH);

  if (strncmp(path, "~", 1) == 0) {
    strlcpy(path, getenv("USERPROFILE"), MAX_PATH);
  } else if (strncmp(path, "-", 1) == 0) {
    strlcpy(path, last_dir, MAX_PATH);
  }

  if (!SetCurrentDirectory((LPWSTR)path)) {
    win_perror("Couldn't change directory.");
    return 1;
  }
#else
#error "Get a better operating system, loser"
#endif
  return 0;
}

int builtin_pwd(int argc, char *argv[], char *const *envp) {
  if (argc > 1)
    return print_builtin_help(Pwd);

#ifdef __linux__
  char cwd_buf[1024] = {0};

  if (getcwd(cwd_buf, 1024) != 0) {
    printf("Current directory: %s\n", cwd_buf);
  } else {
    perror("Can't get current directory");
    return 1;
  }
#elif _WIN32
  char cwd_buf[MAX_PATH];
  DWORD len = GetCurrentDirectoryW(MAX_PATH, (LPWSTR)cwd_buf);

  if (len > 0 && len <= MAX_PATH) {
    wprintf(L"Current directory: %s\n", (LPWSTR)cwd_buf);
  } else {
    win_perror("Can't get current directory");
    return 1;
  }
#else
#error Get a better operating system, loser
#endif

  return 0;
}

int builtin_test(int argc, char *argv[], char *const *envp) {
  for (int i = 0; i != argc; i++) {
    printf("%d: %s\n", i, argv[i]);
  }

  while (*envp) {
    if (strncmp(*envp, "PATH", 4) == 0) {
      puts(*envp);
    }
    envp++;
  }
  return 0;
}

int builtin_help(int argc, char *argv[], char *const *envp) {
  if (argc > 2)
    return print_builtin_help(Help);

  printf("hamon: help\n\n"

         "echo [-N, -n, optional] [message]\n"
         "exit [code, optional]\n"
         "cd [dir]\n"
         "help\n\n"

         "hamon! made with <3\n");

  return 0;
}

int check_builtins(int argc, char *argv[], char *const *envp) {
  const char *builtin_strs[] = {"cd", "pwd", "help", "echo", "exit", "test"};
  int (*builtin_funcs[])(int argc, char *argv[], char *const *envp) = {
      &builtin_cd,   &builtin_pwd,  &builtin_help,
      &builtin_echo, &builtin_exit, builtin_test};
  int num_builtins = sizeof(builtin_strs) / sizeof(char *);

  for (int builtin_index = 0; builtin_index < num_builtins; builtin_index++) {
    if (strncmp(builtin_strs[builtin_index], argv[0], 4) == 0) {
      return builtin_funcs[builtin_index](argc, argv, envp);
    }
  }

  return -1;
}

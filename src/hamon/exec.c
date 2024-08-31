
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__

#include <errno.h>
#include <linux/fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#elif _WIN32

#include <windows.h>

#endif

#define COLORS

#include "headers/escape.h"
#include "headers/exec.h"

#if _WIN32

BOOL FindExecutableInPath(LPCSTR executable, LPCSTR *path_found) {
  LPCSTR env_var = getenv("PATH");
  if (env_var == NULL)
    fprintf(stderr,
            RED "!%s Failed to find path variable, is your windows pc ok?",
            CLEAR) return FALSE;

  LPCSTR path = env_var;
  do {
    size_t len = strlen(path);
    if (len > 0 && path[len - 1] == ';') {
      path[len - 1] = '\0'; // Remove trailing semicolon
    }
    if (_stricmp(executable, path) == 0) {
      *path_found = path;
      return TRUE;
    }
    path += len + 1; // Move to next directory in PATH
  } while (*path);

  return FALSE;
}

#endif

int execute(char *executable, char **argv, int status) {

#ifdef __linux__
  pid_t pid = fork();

  if (pid < 0) {
    perror("failed to fork");
  } else if (pid == 0) {
    if (execvp(executable, argv) == -1) {
      if (errno == ENOENT) {
        fprintf(stderr, RED "!%s %s: Command not found\n", CLEAR, executable);
        exit(-1);
      }

      perror("Failed to exec to childprocess. (execvp)");
      exit(-1);
    } else {
      argv = 0;
      exit(0);
    }
  } else {
    wait(&status);
  }

#elif _WIN32

  LPCSTR win_executable = (LPCSTR)executable;
  LPCSTR path_found = {0};

  if (find_executable_in_path(win_executable, &path_found)) {
    STARTUPINFO start_i;
    PROCESS_INFORMATION proc_i;

    size_t full_path_size = strlen(path_found) + strlen(win_executable) + 2;

    ZeroMemory(&start_i, sizeof(start_i));
    start_i.cb = sizeof(start_i);
    ZeroMemory(&proc_i, sizeof(proc_i));

    LPSTR full_path = malloc(full_path_size);
    strlcpy(full_path, path_found, full_path_size + 1);
    strlcat(full_path, "\\", full_path_size + 1);
    strlcat(full_path, win_executable,
            full_path_size + strlen(win_executable) + 1);

    if (!CreateProcess(0, full_path, 0, 0, FALSE, 0, 0, 0, &start_i, &proc_i)) {
      fprintf(stderr, RED "!%s CreateProcess failed (%d).\n", CLEAR,
              GetLastError());
      return -1;
    }

    CloseHandle(proc_i.hProcess);
    CloseHandle(proc_i.hThread);
    free(full_path);
  } else {
    // Signify nonexistent executable.
    return 1;
  }

#endif
  return 0;
}

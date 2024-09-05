#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__

#include <errno.h>
#include <linux/fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#elif _WIN32

#include <bsd/string.h>
#include <windows.h>

#include <hamon_error.h>

#else
#error Get a better operating system, loser
#endif

#define COLORS

#include <hamon_escape.h>
#include <hamon_exec.h>

#if _WIN32
BOOL find_executable_in_path(LPCSTR executable, LPCSTR *path_found) {
  LPCSTR env_var = getenv("PATH");
  if (env_var == NULL) {
    fprintf(stderr, "!%s Failed to find path variable\n", CLEAR);
    return FALSE;
  }

  LPCSTR path = env_var;
  do {
    size_t len = strlen(path);
    char temp[len + 1];
    strlcpy(temp, path, len);
    if (temp[len - 1] == ';') {
      temp[len - 1] = '\0';
    }
    if (_stricmp(executable, temp) == 0) {
      *path_found = temp;
      return TRUE;
    }
    path += len + 1;
  } while (*path);

  return FALSE;
}
#endif

char *get_exec_path(const char *name, char *const *envp) {
  char path[4096] = {0};

  while (*envp) {
    if (strncmp(*envp, "PATH", 4) == 0) {
      strlcpy(path, *envp, 4096);
      break;
    }
    envp++;
  }

  char *full_path = (char *)malloc(1024);
  char *token = 0;

  token = strtok(strdup(path), ":");
  while (token != 0) {
    snprintf(full_path, 1024, "%s/%s", token, name);

    if (access(full_path, X_OK) == 0) {
      return full_path;
    }
    token = strtok(0, ":");
  }
  return 0;
}

int execute(char *executable, char *argv[], char *const *envp) {
#ifdef __linux__
  pid_t pid = fork();
  int status = 0;
  char *path = 0;

  if (access(executable, X_OK) == 0) {
    path = executable;
  } else {
    path = get_exec_path(executable, envp);
  }

  if (pid < 0) {
    perror("failed to fork");
  } else if (pid == 0) {
    if (execve(path, argv, envp) != 0) {
      if (errno == ENOENT) {
        fprintf(stderr, RED "!%s %s: Command not found\n", CLEAR, executable);
        printf("%s\n", path);
        path = 0;
        free(path);
        exit(-1);
      }

      perror("Failed to exec to childprocess. (execvp)");
      return -1;
    } else {
      free(path);
      return 0;
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
      win_perror("CreareProcess");
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
  free(path);
  return 0;
}

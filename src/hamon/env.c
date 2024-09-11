#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
#include <string.h>
#include <unistd.h>
#elif WIN32_
#include <bsd/string.h>
#endif

#ifdef _WIN32
char **get_environment_variables() {
  LPCH env_strings = GetEnvironmentStrings();
  LPCH ptr = env_strings;
  char **env_array = calloc(4096, sizeof(char *));
  int count = 0;

  while (*ptr != '\0' && count < 4096 - 1) {
    size_t len = strlen(ptr);
    env_array[count] = malloc(len + 1);
    strlcpy(env_array[count], ptr, strlen(env_array[count]) + 1);
    ptr += len + 1;
    count++;
  }

  env_array[count] = NULL;
  FreeEnvironmentStrings(env_strings);
  return env_array;
}
#endif

char *env[4096];
int envc = 0;

bool is_env_format(const char *str) {
  const char *equals = strchr(str, '=');
  return equals != 0 && equals != str && *(equals + 1) != '\0';
}

void init_env() {
#ifdef __linux__
  char **envp = __environ;
#elif _WIN32
  char **envp = get_environment_variables();
#endif
  while (*envp) {
    env[envc++] = (char *)*envp;
    envp++;
  }
#ifdef _WIN32
  free(envp);
#endif
}

void deinit_env() {
#ifdef __linux__
  char **envp = __environ;
  int environ_len = 0;
  while (envp[environ_len] != 0)
    environ_len++;
  for (int envi = 0; envi != envc - environ_len; envi++) {
    printf("Freeing: %s\n", env[envi + environ_len]);
    free(env[envi + environ_len]);
    memset(env, 0, sizeof(env));
  }
#elif _WIN32
  for (int envi = 0; envi != envc; envi++) {
    free(env[envi]);
    memset(env, 0, sizeof(env));
  }
#endif
  envc = 0;
}

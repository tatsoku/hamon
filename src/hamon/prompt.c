
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

#define CURSORS

#include "headers/builtins.h"
#include "headers/escape.h"
#include "headers/exec.h"
#include "headers/prompt.h"

char *get_hostname() {
  char *hostname = {0};
#ifdef __linux__
  size_t hostname_len = 512;
  hostname = (char *)malloc(hostname_len + 1);
  if (gethostname(hostname, hostname_len) != 0) {
    free(hostname);
    return 0;
  }
#elif _WIN32
  DWORD hostname_size = 0;
  if (!GetComputerName(NULL, &hostname_size)) {
    fprintf(stderr, "Failed to get hostname_size\n");
    return 0;
  }

  hostname = (char *)malloc(hostname_size);
  if (!GetComputerName(hostname, &hostname_size)) {
    fprintf(stderr, "Failed to get hostname\n");
    free(hostname);
    return 0;
  }
#else
#error "Use a better operating system, loser"
#endif
  return hostname;
}

char *get_username(void) {
  char *username = 0;
#ifdef __linux__
  size_t username_len = 512;
  username = (char *)malloc(username_len + 1);
  if (getlogin_r(username, username_len) != 0) {
    perror("getlogin_r");
    free(username);
    return 0;
  }
#elif _WIN32
  DWORD buffer_size = 512;
  username = (char *)malloc(buffer_size + 1);
  if (!GetUserName(username, &buffer_size)) {
    fprintf(stderr, "Failed to get username\n");
    free(username);
    return 0;
  }
#else
#error "Use a better operating system, loser"
#endif
  return username;
}

#ifdef _WIN32
LPCH *get_env() {
  LPCH *env_block = GetEnvironmentStrings();
  int count = 0;
  LPCH *var = env_block;
  while (*var) {
    count++;
    var++;
  }
  FreeEnvironmentStrings(env_block);
  LPCH *env_array = (LPCH *)malloc((count + 1) * sizeof(LPWCHAR));
  var = env_block;
  for (int i = 0; i < count; i++) {
    env_array[i] = var;
    var += wcslen(var) + 1;
  }
  env_array[count] = NULL;
  return env_array;
}
#endif

// char *assemble_prompt() { return ">"; }

bool is_env_format(const char *str) {
  const char *equals = strchr(str, '=');
  return equals != 0 && equals != str && *(equals + 1) != '\0';
}

void tokenize(char *input, char **tokens_buffer, char **save_ptr) {
  int token_index = 0;
  char *token = strtok_r(input, " \n", save_ptr);
  while (token != 0) {
    tokens_buffer[token_index] = token;
    token = strtok_r(0, " \n", save_ptr);
    token_index++;
  }
  tokens_buffer[token_index] = 0;
}

int init_prompt(void) {
  char *prompt = "hsh >";
  char input_buf[4096] = {0};
  char *argv[4096] = {0};
  char *save_ptr = {0};
  char executable[128] = {0};
  char *env[1024] = {0};

#ifdef __linux__
  char **envp = __environ;
#elif _WIN32
  LPCH *envp = get_env();
#endif

  int status = 0;
  int argc = 0;
  int env_count = 0;

  while (*envp) {
    env[env_count++] = (char *)*envp;
    envp++;
  }

#ifdef _WIN32
  free(envp);
#endif

  for (;;) {
    printf("%s%s ", VERTICAL_CURSOR, prompt);
    if (fgets(input_buf, 4096, stdin) == 0) {
      perror("fgets");
      continue;
    }

    input_buf[strcspn(input_buf, "\n")] = 0;
    if (strncmp(input_buf, "", 1) == 0)
      continue;

    tokenize(input_buf, argv, &save_ptr);

    while (argv[argc] != 0)
      argc++;

    for (int argi = 0; argi != argc; argi++) {
      if (is_env_format(argv[argi])) {
        printf("User specified env variable: %s\n", argv[argi]);
        env[env_count++] = argv[argi];
      } else {
        strlcpy(executable, argv[argi], 128);
        break;
      }
    }

    if (check_builtins(argc, argv, env) == -1) {
      if (execute(executable, argv, status, env) == -1)
        continue;
    }

    argc = 0;
  }
}

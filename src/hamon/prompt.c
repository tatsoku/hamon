#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__

#include <string.h>
#include <unistd.h>

#elif _WIN32

#include <bsd/string.h>
#include <windows.h>

#else
#error "Use a better operating system, loser"
#endif

#define CURSORS

#include <hamon_builtins.h>
#include <hamon_env.h>
#include <hamon_escape.h>
#include <hamon_exec.h>
#include <hamon_prompt.h>

extern char *env[4096];
extern int envc;

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

// char *assemble_prompt() { return ">"; }

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

void tokenize(char *input, char **tokens_buffer) {
  int token_index = 0;
  bool in_quotes = false;
  char *token_start = input;
  char *p_input = input;

  while (*p_input != '\0') {
    if (*p_input == '"' || *p_input == '\'') {
      in_quotes = !in_quotes;
      p_input++;
    } else if (!in_quotes && isspace(*p_input)) {
      if (p_input > token_start) {
        *p_input = '\0';
        tokens_buffer[token_index++] = token_start;
      }
      p_input++;
      token_start = p_input;
    } else {
      p_input++;
    }
  }

  if (p_input > token_start) {
    tokens_buffer[token_index++] = token_start;
  }

  for (int i = 0; i < token_index; i++) {
    char *token = tokens_buffer[i];
    if (*token == '"' || *token == '\'') {
      memmove(token, token + 1, strlen(token));
      token[strlen(token) - 1] = '\0';
    }
  }

  tokens_buffer[token_index] = 0;
}

int init_prompt(void) {
  char *prompt = "hsh >";
  char input_buf[4096] = {0};
  char *argv[4096] = {0};
  char executable[128] = {0};

  int argc = 0;

  init_env();

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

    tokenize(input_buf, argv);

    while (argv[argc] != 0)
      argc++;

    for (int argi = 0; argi != argc; argi++) {
      if (is_env_format(argv[argi])) {
        printf("User specified env variable: %s\n", argv[argi]);
        env[envc++] = argv[argi];
      } else {
        strlcpy(executable, argv[argi], 128);
        break;
      }
    }

    if (check_builtins(argc, argv) == -1) {
      if (execute(executable, argv) == -1)
        continue;
    }

    argc = 0;
  }
}

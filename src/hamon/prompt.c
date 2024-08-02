#define CURSORS
#include "headers/prompt.h"
#include "headers/builtins.h"
#include "headers/config.h"
#include "headers/escape.h"
#include "headers/exec.h"

char *get_hostname() {
  char *hostname = 0;
#if __linux__
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
#endif
  return hostname;
}

char *get_username() {
  char *username = 0;
#if __linux__
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
#endif
  return username;
}

enum PromptType get_prompt_type() { return Default; }

// char *assemble_prompt() { return ">"; }

void tokenize(char *input, char **tokens_buffer, char **save_ptr) {
  int token_index = 0;
  char *token = strtok_r(input, " \n", save_ptr);
  while (token != 0) {
    tokens_buffer[token_index] = token;
    token = strtok_r(0, " \n", save_ptr);
    token_index++;
  }
  tokens_buffer[token_index] = "\0";
  input = 0;
}

int init_prompt() {
  char *prompt = "hsh >";
  char input_buf[4096] = {0};
  char *args[4096] = {0};
  char *save_ptr = {0};
  int status = 0;

  while (1) {
    printf("%s%s ", VERTICAL_CURSOR, prompt);
    if (fgets(input_buf, 4096, stdin) == 0) {
      perror("fgets");
      continue;
    }

    input_buf[strcspn(input_buf, "\n")] = 0;

    if (strncmp(input_buf, "", 1) == 0) {
      continue;
    }

    tokenize(input_buf, args, &save_ptr);

    int argc = sizeof(args) / sizeof(args[0]);

    if (!check_builtins(argc, args)) {
      execute(args[0], args, status);
    }
  }

  return 0;
}

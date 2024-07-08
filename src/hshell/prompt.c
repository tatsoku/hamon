#include "headers/prompt.h"
#include "headers/config.h"

char *get_hostname() {
  char *hostname = 0;
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
#endif
  return hostname;
}

char *get_username() {
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
#endif
  return username;
}

enum PromptType get_prompt_type() { return Default; }

char *assemble_prompt(enum PromptType prompt_type) {
  switch (prompt_type) {
  case Default:
    puts("todo");
    break;
  case Custom:
    puts("todo");
    break;
  default:
    return 0;
  }
  return 0;
}

int init_prompt() {
  char *prompt = assemble_prompt(Default);
  return 0;
}

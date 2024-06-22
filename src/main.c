#include "hshell/headers/color.h"
#include "hshell/headers/prompt.h"

/*
▄▄          ▄▄▄▄    ▄▄                 ▄▄▄▄   ▄▄▄▄
██        ▄█▀▀▀▀█   ██                 ▀▀██   ▀▀██
██▄████▄  ██▄       ██▄████▄   ▄████▄    ██     ██
██▀   ██   ▀████▄   ██▀   ██  ██▄▄▄▄██   ██     ██
██    ██       ▀██  ██    ██  ██▀▀▀▀▀▀   ██     ██
██    ██  █▄▄▄▄▄█▀  ██    ██  ▀██▄▄▄▄█   ██▄▄▄  ██▄▄▄
▀▀    ▀▀   ▀▀▀▀▀    ▀▀    ▀▀    ▀▀▀▀▀     ▀▀▀▀   ▀▀▀▀
*/

int main(int argc, char *argv[]) {
  char *hostname = get_hostname();
  puts("Welcome to..");
  printf(CYAN);
  puts("▄▄          ▄▄▄▄    ▄▄                 ▄▄▄▄   ▄▄▄▄");
  puts("██        ▄█▀▀▀▀█   ██                 ▀▀██   ▀▀██");
  puts("██▄████▄  ██▄       ██▄████▄   ▄████▄    ██     ██");
  puts("██▀   ██   ▀████▄   ██▀   ██  ██▄▄▄▄██   ██     ██");
  puts("██    ██       ▀██  ██    ██  ██▀▀▀▀▀▀   ██     ██");
  puts("██    ██  █▄▄▄▄▄█▀  ██    ██  ▀██▄▄▄▄█   ██▄▄▄  ██▄▄▄");
  puts("▀▀    ▀▀   ▀▀▀▀▀    ▀▀    ▀▀    ▀▀▀▀▀     ▀▀▀▀   ▀▀▀▀");
  printf(CLEAR);
  printf(RED ">%s a homemade command shell, in %sC\n" CLEAR, CLEAR, CYAN);
  printf("Made with %s❤%s by h4rl\n", RED, CLEAR);
  printf("Hostname: %s\n", hostname);
  free(hostname);
  return 0;
}

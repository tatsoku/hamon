#include "hshell/headers/color.h"
#include <stdio.h>

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
  return 0;
}

#define COLORS
#define GRAPHICS
#include "headers/escape.h"

char *assemble(char *codes[], int codesc) {
  int buffer_size = codesc * 2 + 6;
  char *buffer = {0};
  buffer = (char *)malloc(buffer_size * sizeof(char));

  if (!buffer) {
    perror("buffer malloc");
    return 0;
  }

  snprintf(buffer, sizeof(BASE), BASE);

  for (int code_index = 0; code_index < codesc; code_index++) {
    strncat(buffer, codes[code_index], sizeof(codes[code_index]));
    if (code_index != (codesc - 1)) {
      strncat(buffer, ";", 1);
    }
  }

  strncat(buffer, END, 1);
  return buffer;
}

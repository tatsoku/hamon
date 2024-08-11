#define COLORS
#define GRAPHICS
#include "headers/escape.h"

char *assemble(char *codes[], int codesc) {
  size_t buffer_size = codesc * 2 + 6;
  char *buffer = {0};
  buffer = (char *)malloc(buffer_size);

  if (!buffer) {
    perror("buffer malloc");
    return 0;
  }

  snprintf(buffer, sizeof(BASE), BASE);

  for (int code_index = 0; code_index < codesc; code_index++) {
    strlcat(buffer, codes[code_index],
            buffer_size + sizeof(codes[code_index] + 1));
    if (code_index != (codesc - 1)) {
      strlcat(buffer, ";", buffer_size + 2);
    }
  }

  strlcat(buffer, END, buffer_size + 2);
  return buffer;
}

//{RED_CODE, ITALIC_CODE}, 2
//  \033[31;3m

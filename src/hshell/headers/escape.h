#ifndef ESCAPE_H
#define ESCAPE_H

#define BASE                "\033["
#define END                 "m"

// Colors like Red, Green, Blue, may extend more.
#ifdef COLORS

#define RED_CODE            "31"
#define GREEN_CODE          "32"
#define YELLOW_CODE         "33"
#define BLUE_CODE           "34"
#define MAGENTA_CODE        "35"
#define CYAN_CODE           "36"

#define RED                 BASE RED_CODE END
#define GREEN               BASE GREEN_CODE END
#define YELLOW              BASE YELLOW_CODE END
#define BLUE                BASE BLUE_CODE END
#define MAGENTA             BASE MAGENTA_CODE END
#define CYAN                BASE CYAN_CODE END

#endif

// Graphics such as bold, strike through, italics, dim, blink, underline, etc.
#ifdef GRAPHICS

#define BOLD_CODE           "1"
#define DIM_CODE            "2"
#define ITALIC_CODE         "3"
#define UNDERLINE_CODE      "4"
#define BLINK_CODE          "5"
#define REVERSE_CODE        "7"
#define INVISIBLE_CODE      "8"
#define STRIKETHROUGH_CODE  "9"

#define BOLD                BASE BOLD_CODE END
#define DIM                 BASE DIM_CODE END
#define ITALIC              BASE ITALIC_CODE END
#define UNDERLINE           BASE UNDERLINE_CODE END
#define BLINK               BASE BLINK_CODE END
#define REVERSE             BASE REVERSE_CODE END
#define INVISIBLE           BASE INVISIBLE_CODE END
#define STRIKETHROUGH       BASE STRIKETHROUGH_CODE END

#endif

// Special CLEARS for graphics, possibly not needed but can be useful in the future
#ifdef GRAPHICS_CLEAR

#define BOLD_CLEAR          BASE "22" END
#define DIM_CLEAR           BOLD_CLEAR
#define ITALIC_CLEAR        BASE "23" END
#define UNDERLINE_CLEAR     BASE "24" END
#define BLINK_CLEAR         BASE "25" END
#define REVERSE_CLEAR       BASE "27" END
#define INVISIBLE_CLEAR     BASE "28" END
#define STRIKETHROUGH_CLEAR BASE "29" END

#endif

// Global clear color/graphics mode string.
#define CLEAR               "\033[0m"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *assemble(char *codes[], int codesc);

#endif

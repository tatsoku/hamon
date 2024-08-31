#define COLORS
#define GRAPHICS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/cli.h"
#include "headers/escape.h"
#include "headers/meta.h"

typedef struct {
  char *first;
  char *second;
} SplitPair;

void split_array(char *array[], const int size, SplitPair *pairs) {
  int index = 0;
  for (int i = 0; i < size; i += 2) {
    pairs[index].first = array[i];
    pairs[index].second = array[i + 1];
    index++;
  }
}

typedef struct {
  char **flags;
  const int flagsc;
  const char *flags_esc;
  const char *info;
  const char *info_esc;
  const char *use;
  const char *use_esc;
  char *first_arg;
  const int first_arg_len;
} Info;

int info_for_flag(Info *info) {
  char **flags = info->flags;
  const char *flags_esc = info->flags_esc;
  char *first_arg = info->first_arg;
  int first_arg_len = info->first_arg_len;
  const char *use = info->use;

  char *buffer = {0};
  buffer = (char *)malloc((first_arg_len + strlen(use)) * sizeof(char));
  if (!buffer) {
    perror("buffer malloc");
    return 1;
  }

  snprintf(buffer, ((first_arg_len * 2 + strlen(use)) - 3), use, first_arg,
           first_arg);

  printf("%s%s", flags_esc, flags[0]);
  for (int flag_row = 1; flag_row < info->flagsc - 1; flag_row++) {
    printf(CLEAR " | %s%s" CLEAR, flags_esc, flags[flag_row]);
  }
  printf("\n%s%s\n" CLEAR, info->info_esc, info->info);
  printf("USAGE: %s%s\n\n" CLEAR, info->use_esc, buffer);
  free(buffer);
  return 0;
}

typedef struct {
  char **flags;
  const int flagsc;
  const char **info;
  const int infoc;
  const char **use;
  char *first_arg;
  const int first_arg_len;
} Help;

int help(Help *help) {
  const int infoc = help->infoc;
  const char **use = help->use;

  SplitPair flag_pairs[infoc];

  char *codes[][3] = {{ITALIC_CODE, GREEN_CODE},
                      {BOLD_CODE, UNDERLINE_CODE, CYAN_CODE},
                      {BOLD_CODE, BLUE_CODE},
                      {BOLD_CODE, GREEN_CODE},
                      {BOLD_CODE, MAGENTA_CODE}};
  char *flags_esc = assemble(codes[0], 2);
  char *title_esc = assemble(codes[1], 3);
  char *lic_esc = assemble(codes[2], 2);
  char *aut_esc = assemble(codes[3], 2);
  char *use_esc = assemble(codes[4], 2);

  printf("%s%s%s %sv%s%s %s %s help\n\n" CLEAR, title_esc, __NAME__, CLEAR,
         BOLD, __PROGRAM_VERSION__, CLEAR, STRIKETHROUGH, CLEAR);

  free(title_esc);

  split_array(help->flags, help->flagsc, flag_pairs);
  for (int help_line = 0; help_line < help->infoc; help_line++) {
    char *split_flags[2] = {flag_pairs[help_line].first,
                            flag_pairs[help_line].second};
    Info flag_info = {
        split_flags,    infoc,   flags_esc,       help->info[help_line], BOLD,
        use[help_line], use_esc, help->first_arg, help->first_arg_len};
    info_for_flag(&flag_info);
  }
  free(use_esc);
  free(flags_esc);

  printf("Licensed under the %s%s\n" CLEAR, lic_esc, __LICENSE__);
  printf("Made with %s❤%s by %s%s\n" CLEAR, RED, CLEAR, aut_esc, __AUTHOR__);

  free(lic_esc);
  free(aut_esc);

  return 0;
}

int print_ver(void) {
  char *title_c[3] = {BOLD_CODE, UNDERLINE_CODE, CYAN_CODE};
  char *title_esc = assemble(title_c, (sizeof(title_c) / sizeof(title_c[0])));
  char *aut_c[2] = {BOLD_CODE, GREEN_CODE};
  char *aut_esc = assemble(aut_c, (sizeof(aut_c) / sizeof(aut_c[0])));

  printf("%s%s%s %sv%s%s %s %s Made with %s❤%s by %s%s\n" CLEAR, title_esc,
         __NAME__, CLEAR, BOLD, __PROGRAM_VERSION__, CLEAR, STRIKETHROUGH,
         CLEAR, RED, CLEAR, aut_esc, __AUTHOR__);

  free(title_esc);
  free(aut_esc);
  return 0;
}

int get_flag_int(const char *flag, char *flags[], const int flagsc) {
  for (int flag_index = 0; flag_index < flagsc; flag_index++) {
    if (strncmp(flags[flag_index], flag, strlen(flag)) == 0) {
      return flag_index;
    }
  }
  return -1;
}

int process_args(char *argv[]) {
  char *flags[] = {"-h", "--help", "-v", "--version", "-c", "--config"};
  const char *infos[] = {"Prints this message.", "Shows program version.",
                         "Load a config file from other path than default"};
  const char *uses[] = {
      "%s --help\n       %s -h", "%s --version\n       %s -v",
      "%s --config {config file path}\n       %s -c {config file path}"};

  int flagsc = sizeof(flags) / sizeof(flags[0]);
  int infosc = sizeof(infos) / sizeof(infos[0]);

  Help help_data = {flags, flagsc,  infos,          infosc,
                    uses,  argv[0], strlen(argv[0])};

  int cur_flag = get_flag_int(argv[1], flags, flagsc);

  switch (cur_flag) {
  case 0:
  case 1:
    help(&help_data);
    break;

  case 2:
  case 3:
    print_ver();
    break;

  case 4:
  case 5:
    puts("config file thing");
    break;

  default:
    puts("Incorrect command.");
    help(&help_data);
  }

  return 0;
}

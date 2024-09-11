#define COLORS
#define GRAPHICS

#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__

#include <string.h>

#elif _WIN32


#endif

#include <hamon_cli.h>
#include <hamon_escape.h>
#include <hamon_meta.h>
#include <string.h>



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
  const char *info;
  const char *use;
  char *first_arg;
  const int first_arg_len;
} Info;

int info_for_flag(Info *info) {
  char **flags = info->flags;
  char *first_arg = info->first_arg;
  int first_arg_len = info->first_arg_len;
  const char *use = info->use;

  printf("%s\n", use);
  printf("%zu\n", strlen(use));

  char *buffer = {0};
  buffer = (char *)malloc((first_arg_len + strlen(use)) * sizeof(char));
  if (!buffer) {
    perror("buffer malloc");
    return 1;
  }

  snprintf(buffer, first_arg_len * 2 + strlen(use) + 1, use, first_arg,
           first_arg);

  printf("%s", flags[0]);
  for (int flag_row = 1; flag_row < info->flagsc - 1; flag_row++) {
    printf(" | %s", flags[flag_row]);
  }
  printf("\n%s\n", info->info);
  printf("USAGE: %s\n\n", buffer);
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

  SplitPair* flag_pairs = malloc(infoc);

  printf("%s v%s %s %s help\n\n", __NAME__, __PROGRAM_VERSION__, STRIKETHROUGH,
         CLEAR);

  split_array(help->flags, help->flagsc, flag_pairs);
  for (int help_line = 0; help_line < help->infoc; help_line++) {
    char *split_flags[2] = {flag_pairs[help_line].first,
                            flag_pairs[help_line].second};
    Info flag_info = {split_flags,    infoc,           help->info[help_line],
                      use[help_line], help->first_arg, help->first_arg_len};
    info_for_flag(&flag_info);
  }

  printf("Licensed under the %s\n", __LICENSE__);
  printf("Made with %s❤%s by %s\n", RED, CLEAR, __AUTHOR__);

  free(flag_pairs);
  return 0;
}

int print_ver(void) {
  printf("%s v%s %s %s Made with %s❤%s by %s\n", __NAME__, __PROGRAM_VERSION__,
         STRIKETHROUGH, CLEAR, RED, CLEAR, __AUTHOR__);

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

int process_args(int argc, char *argv[]) {

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
    if (argc > 2) {
      puts("Too many arguments passed.");
      help(&help_data);
      return 1;
    }

    help(&help_data);
    break;

  case 2:
  case 3:
    if (argc > 2) {
      puts("Too many arguments passed.");
      help(&help_data);
      return 1;
    }

    print_ver();
    break;

  case 4:
  case 5:
    if (argc < 3) {
      puts("Provide a config file.");
      help(&help_data);
      return 1;
    }
    puts("config file thing");
    break;

  default:
    puts("Incorrect command.");
    help(&help_data);
    return 1;
  }

  return 0;
}

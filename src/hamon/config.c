#define COLORS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/config.h"
#include "headers/escape.h"
#include "headers/file.h"

#ifdef _WIN32
#include "headers/error.h"
#include "windows.h"
#endif

/*
 * Default configuration:
 * ▄▄                                                ▄▄
 * ██                                                ██
 * ██▄████▄   ▄█████▄  ████▄██▄   ▄████▄   ██▄████▄  ██
 * ██▀   ██   ▀ ▄▄▄██  ██ ██ ██  ██▀  ▀██  ██▀   ██  ██
 * ██    ██  ▄██▀▀▀██  ██ ██ ██  ██    ██  ██    ██  ▀▀
 * ██    ██  ██▄▄▄███  ██ ██ ██  ▀██▄▄██▀  ██    ██  ▄▄
 * ▀▀    ▀▀   ▀▀▀▀ ▀▀  ▀▀ ▀▀ ▀▀    ▀▀▀▀    ▀▀    ▀▀  ▀▀
 *
 * # !! Main configuration for theming and prompt
 * # More advanced theming will happen at a way later date.
 *
 * [prompt]
 * hostname          = true
 * username          = true
 * two_line          = true
 * cd                = true
 * git               = true
 * color             = true
 * prompt            = ">"
 * nerd_fonts        = false
 *
 * # Format settings.
 * # If something isn't enabled, just remove it from the format,
 * # or leave it in, it will be ignored
 *
 * format = "{username_icon} {username} @ {hostname_icon} {hostname} | {cd}
 * {git_icon}{git_tree} {git}"
 *
 * # !! Color config,
 * # !IMPORTANT! None of these settings will be applied if color is false.
 *
 * # Color variations available:
 * # [ "black", "red", "green", "yellow", "blue", "magenta", "cyan" ]
 *
 * # To keep something uncolored, just set it to "".
 * # 256 colors will be supported at a later date.
 *
 * [colors]
 * prompt_bg_color   = ""
 * prompt_fg_color   = "green"
 *
 * hostname_bg_color = ""
 * hostname_fg_color = "blue"
 *
 * cd_bg_color       = ""
 * cd_fg_color       = "yellow"
 *
 * git_bg_color      = ""
 * git_fg_color      = "red"
 *
 * # !! Nerd fonts config.
 * # !IMPORTANT! None of these settings will be applied if nerd_fonts is false.
 *
 * [nerd_fonts]
 * hostname_icon     = "󰌢"
 * username_icon     = ""
 * cd_icon           = ""
 *
 * # git branch statuses
 * git_tree          = [ "󰘬", "󱓏", "󱓋", "󱓊", "󱓍", "󱓌", "󱓎"]
 * git_icon          = "󰊢"
 */

const char *default_config =
    "# ▄▄                                                ▄▄\n"
    "# ██                                                ██\n"
    "# ██▄████▄   ▄█████▄  ████▄██▄   ▄████▄   ██▄████▄  ██\n"
    "# ██▀   ██   ▀ ▄▄▄██  ██ ██ ██  ██▀  ▀██  ██▀   ██  ██\n"
    "# ██    ██  ▄██▀▀▀██  ██ ██ ██  ██    ██  ██    ██  ▀▀\n"
    "# ██    ██  ██▄▄▄███  ██ ██ ██  ▀██▄▄██▀  ██    ██  ▄▄\n"
    "# ▀▀    ▀▀   ▀▀▀▀ ▀▀  ▀▀ ▀▀ ▀▀    ▀▀▀▀    ▀▀    ▀▀  ▀▀\n\n"

    "# !! Main configuration for theming and prompt\n"
    "# More advanced theming will happen at a way later date.\n\n"

    "[prompt]\n"
    "hostname          = true\n"
    "username          = true\n"
    "two_line          = true\n"
    "cd                = true\n"
    "git               = true\n"
    "color             = true\n"
    "nerd_fonts        = true\n"
    "prompt            = \">\"\n\n"

    "# Format settings.\n"
    "# If something isn't enabled, just remove it from the format\n"
    "# or leave it in, it will be ignored anyway.\n\n"

    "format = \"{username_icon} {username} @ {hostname_icon} {hostname} | "
    "{cd}{git_icon}{git_tree} {git}\"\n\n"

    "# !! Color config,\n"
    "# !IMPORTANT! None of these settings will be applied if color is "
    "false.\n\n"

    "# Color variations available:\n"
    "# [ \"black\", \"red\", \"green\", \"yellow\", \"blue\", \"magenta\", "
    "\"cyan\" ]\n\n"

    "# To keep something uncolored, just set it to \"\".\n"
    "# 256 colors will be supported at a later date. (I'm too lazy to make it "
    "use 256 color ansi escapes currently)\n\n"

    "[colors]\n"
    "prompt_bg_color   = \"\"\n"
    "prompt_fg_color   = \"green\"\n\n"
    "hostname_bg_color = \"\"\n"
    "hostname_fg_color = \"blue\"\n\n"
    "cd_bg_color       = \"\"\n"
    "cd_fg_color       = \"yellow\"\n\n"
    "git_bg_color      = \"\"\n"
    "git_fg_color      = \"red\"\n\n"
    "# !! Nerd fonts config.\n"
    "# !IMPORTANT! None of these settings will be applied if nerd_fonts is "
    "false.\n\n"

    "[nerd_fonts]\n"
    "hostname_icon     = \"󰌢\"\n"
    "username_icon     = \"\"\n"
    "cd_icon           = \"\"\n\n"

    "# git branch statuses.\n"
    "git_tree          = [ \"󰘬\", \"󱓏\", \"󱓋\", \"󱓊\", \"󱓍\", "
    "\"󱓌\", \"󱓎\" ]\n"
    "git_icon          = \"󰊢\"\n";

/*
 * Default config generation func.
 */

int gen_default_config(void) {
  size_t config_buffer_size = strlen(default_config);
  char *config_buffer = {0};

  config_buffer = (char *)malloc(config_buffer_size);
  if (!config_buffer) {
    perror("malloc");
    return -1;
  }

  size_t length = strlcpy(config_buffer, default_config, config_buffer_size);
  if (length > sizeof(config_buffer_size)) {
    free(config_buffer);
    return -1;
  }

#ifdef _WIN32
  char folder_buffer[MAX_PATH] = {0};
  char absolute_path_buffer[MAX_PATH] = {0};

  int res1 =
      snprintf(folder_buffer, MAX_PATH, "%s\\hamon\\", getenv("APPDATA"));
  if (res1 < 0 || res1 >= MAX_PATH) {
    win_perror("snprintf");
    return -1;
  }

  int res2 =
      snprintf(absolute_path_buffer, MAX_PATH, "%sconfig.toml", folder_buffer);
  if (res2 < 0 || res2 >= 1024) {
    win_perror("snprintf");
    return -1;
  }
#elif __linux__
  char folder_buffer[1024] = {0};
  char absolute_path_buffer[1024] = {0};

  int res1 = snprintf(folder_buffer, 1024, "%s/.config/hamon/", getenv("HOME"));
  if (res1 < 0 || res1 >= 1024) {
    perror("snprintf");
    return -1;
  }

  int res2 =
      snprintf(absolute_path_buffer, 1024, "%sconfig.toml", folder_buffer);
  if (res2 < 0 || res2 >= 1024) {
    perror("snprintf");
    return -1;
  }
#else
#error "Use a better operating system, loser
#endif
  if (!check_if_folder_exists(folder_buffer)) {
    if (!create_folder(folder_buffer)) {
      fprintf(stderr, RED "!%s Failed to create config directory, exiting..\n",
              CLEAR);
      return -1;
    }
  }

  if (read_file(absolute_path_buffer)) {
    int status =
        write_file(absolute_path_buffer, config_buffer, config_buffer_size);

    if (!status) {
      free(config_buffer);
      return -1;
    }
  }
  free(config_buffer);
  return 1;
}

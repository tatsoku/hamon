#define COLORS

#include "headers/config.h"
#include "headers/escape.h"
#include "headers/file.h"

// includes.
#include "../../include/toml-c.h"

/*
 * Default configuration:
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
  size_t env_size = 0;
  size_t folder_buffer_size = 0;
  char *folder_buffer = {0};

  size_t config_buffer_size = strlen(default_config);
  char *config_buffer = {0};

  config_buffer = (char *)malloc(config_buffer_size);
  if (!config_buffer) {
    perror("malloc");
    return 1;
  }

  strncpy(config_buffer, default_config, config_buffer_size);

#if _WIN32
  env_size = strlen(getenv("APPDATA"));
  folder_buffer_size = strlen("\\hamon\\") + env_size + 1;
  folder_buffer = (char *)malloc(folder_buffer_size);
  snprintf(folder_buffer, folder_buffer_size, "%s\\hamon\\", getenv("APPDATA"));
#elif __linux__
  env_size = strlen(getenv("HOME"));
  folder_buffer_size = strlen("/.config/hamon/") + env_size + 1;
  folder_buffer = (char *)malloc(folder_buffer_size);
  snprintf(folder_buffer, folder_buffer_size, "%s/.config/hamon/",
           getenv("HOME"));
#endif

  if (!check_if_folder_exists(folder_buffer)) {
    if (!create_folder(folder_buffer)) {
      fprintf(stderr, RED "!%s Failed to create config directory, exiting..\n",
              CLEAR);
      free(folder_buffer);
      return 1;
    }
  }

  size_t absolute_path_buffer_size = folder_buffer_size + strlen("config.toml");
  char *absolute_path_buffer = (char *)malloc(absolute_path_buffer_size);
  snprintf(absolute_path_buffer, absolute_path_buffer_size, "%sconfig.toml",
           folder_buffer);

  int status =
      write_file(absolute_path_buffer, config_buffer, config_buffer_size);
  free(config_buffer);
  free(folder_buffer);
  free(absolute_path_buffer);
  return 0;
}

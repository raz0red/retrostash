/* RetroArch - A frontend for libretro.
 * Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 * Copyright (C) 2011-2017 - Daniel De Matteis
 * Copyright (C) 2012-2015 - Michael Lelli
 *
 * RetroArch is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Found-
 * ation, either version 3 of the License, or (at your option) any later version.
 *
 * RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with RetroArch.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <string.h>

#include <file/config_file.h>
#include <queues/task_queue.h>
#include <file/file_path.h>
#include <string/stdstring.h>
#include <retro_timers.h>
#include <gfx/video_frame.h>
#include <glsym/glsym.h>

#ifdef HAVE_CONFIG_H
#include "../../config.h"
#endif

#ifndef IS_SALAMANDER
#include <lists/file_list.h>
#endif

#include "../frontend.h"
#include "../frontend_driver.h"
#include "../../configuration.h"
#include "../../defaults.h"
#include "../../content.h"
#include "../../retroarch.h"
#include "../../verbosity.h"
#include "../../command.h"
#include "../../tasks/tasks_internal.h"
#include "../../file_path_special.h"
#include "../../paths.h"
#ifdef WRC
#include "../../deps/rcheevos/include/rc_hash.h"
#include "../../../wrc.h"
#include "../../driver.h"
#endif

void dummyErrnoCodes(void);
void emscripten_mainloop(void);
extern void em_cmd_savefiles();

void cmd_savefiles(void)
{
   em_cmd_savefiles();
   command_event(CMD_EVENT_SAVE_FILES, NULL);
}

void cmd_save_state(void)
{
   command_event(CMD_EVENT_SAVE_STATE, NULL);
}

void cmd_load_state(void)
{
   command_event(CMD_EVENT_LOAD_STATE, NULL);
}

void cmd_take_screenshot(void)
{
   command_event(CMD_EVENT_TAKE_SCREENSHOT, NULL);
}

#ifdef WRC
void cmd_audio_reinit(void) {
   command_event(CMD_EVENT_AUDIO_REINIT, NULL);
}

void cmd_audio_stop(void) {
   command_event(CMD_EVENT_AUDIO_STOP, NULL);
}

void cmd_audio_start(void) {
   command_event(CMD_EVENT_AUDIO_START, NULL);
}

void cmd_pause(void) {
   command_event(CMD_EVENT_PAUSE_TOGGLE, NULL);
}

void cmd_unpause(void) {
   command_event(CMD_EVENT_UNPAUSE, NULL);
}

void wrc_enable_bilinear_filter(int enable) {
   printf("## wrc_enable_bilinear_filter: %d\n", enable);
   settings_t *settings = config_get_ptr();
   settings->bools.video_smooth = (enable == 1);
   video_driver_reinit(DRIVER_VIDEO_MASK);
}

extern void rcheevos_file_reader_init();
static bool rcheevos_init = false;

extern int task_database_chd_get_serial(const char *name, char* serial);

void hash_generate_from_file(int console_id, const char* path) {
   if (!rcheevos_init) {
      rcheevos_file_reader_init();
      rcheevos_init = true;
   }

   char hash[33] = "";
   int result = rc_hash_generate_from_file(hash, console_id, path);
   printf("## result=%d, file=%s, hash=%s\n", result, path, hash);

   char serial[33] = "";
   task_database_chd_get_serial(path, serial);
   printf("## serial=%s\n", serial);
}

unsigned int wrc_options = {0};
unsigned int wrc_input_state[GAMEPAD_COUNT] = {0};
float wrc_input_state_analog[GAMEPAD_COUNT][4] = {0};

void wrc_set_input(int index, unsigned int state, float alx, float aly, float arx, float ary) {
   wrc_input_state_analog[index][0] = alx;
   wrc_input_state_analog[index][1] = aly;
   wrc_input_state_analog[index][2] = arx;
   wrc_input_state_analog[index][3] = ary;
   wrc_input_state[index] = state;
}

extern void wrc_on_set_options(int opts);

void wrc_set_options(int opts) {
   wrc_options = opts;
   wrc_on_set_options(opts);
}

#endif

static void frontend_emscripten_get_env(int *argc, char *argv[],
      void *args, void *params_data)
{
   char base_path[PATH_MAX] = {0};
   char user_path[PATH_MAX] = {0};
   const char *home         = getenv("HOME");

   if (home)
   {
      snprintf(base_path, sizeof(base_path),
            "%s/retroarch", home);
      snprintf(user_path, sizeof(user_path),
            "%s/retroarch/userdata", home);
   }
   else
   {
      snprintf(base_path, sizeof(base_path), "retroarch");
      snprintf(user_path, sizeof(user_path), "retroarch/userdata");
   }

   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_CORE], base_path,
         "cores", sizeof(g_defaults.dirs[DEFAULT_DIR_CORE]));

   /* bundle data */
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_ASSETS], base_path,
         "bundle/assets", sizeof(g_defaults.dirs[DEFAULT_DIR_ASSETS]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_AUTOCONFIG], base_path,
         "bundle/autoconfig", sizeof(g_defaults.dirs[DEFAULT_DIR_AUTOCONFIG]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_CURSOR], base_path,
         "bundle/database/cursors", sizeof(g_defaults.dirs[DEFAULT_DIR_CURSOR]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_DATABASE], base_path,
         "bundle/database/rdb", sizeof(g_defaults.dirs[DEFAULT_DIR_DATABASE]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_CORE_INFO], base_path,
         "bundle/info", sizeof(g_defaults.dirs[DEFAULT_DIR_CORE_INFO]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_OVERLAY], base_path,
         "bundle/overlays", sizeof(g_defaults.dirs[DEFAULT_DIR_OVERLAY]));
#ifdef HAVE_VIDEO_LAYOUT
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_VIDEO_LAYOUT], base_path,
         "bundle/layouts", sizeof(g_defaults.dirs[DEFAULT_DIR_VIDEO_LAYOUT]));
#endif
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_SHADER], base_path,
         "bundle/shaders", sizeof(g_defaults.dirs[DEFAULT_DIR_SHADER]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_AUDIO_FILTER], base_path,
         "bundle/filters/audio", sizeof(g_defaults.dirs[DEFAULT_DIR_AUDIO_FILTER]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_VIDEO_FILTER], base_path,
         "bundle/filters/video", sizeof(g_defaults.dirs[DEFAULT_DIR_VIDEO_FILTER]));

   /* user data dirs */
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_CHEATS], user_path,
         "cheats", sizeof(g_defaults.dirs[DEFAULT_DIR_CHEATS]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_MENU_CONFIG], user_path,
         "config", sizeof(g_defaults.dirs[DEFAULT_DIR_MENU_CONFIG]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_MENU_CONTENT], user_path,
         "content", sizeof(g_defaults.dirs[DEFAULT_DIR_MENU_CONTENT]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_CORE_ASSETS], user_path,
         "content/downloads", sizeof(g_defaults.dirs[DEFAULT_DIR_CORE_ASSETS]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_PLAYLIST], user_path,
         "playlists", sizeof(g_defaults.dirs[DEFAULT_DIR_PLAYLIST]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_REMAP], g_defaults.dirs[DEFAULT_DIR_MENU_CONFIG],
         "remaps", sizeof(g_defaults.dirs[DEFAULT_DIR_REMAP]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_SRAM], user_path,
         "saves", sizeof(g_defaults.dirs[DEFAULT_DIR_SRAM]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_SCREENSHOT], user_path,
         "screenshots", sizeof(g_defaults.dirs[DEFAULT_DIR_SCREENSHOT]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_SAVESTATE], user_path,
         "states", sizeof(g_defaults.dirs[DEFAULT_DIR_SAVESTATE]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_SYSTEM], user_path,
         "system", sizeof(g_defaults.dirs[DEFAULT_DIR_SYSTEM]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_THUMBNAILS], user_path,
         "thumbnails", sizeof(g_defaults.dirs[DEFAULT_DIR_THUMBNAILS]));
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_LOGS], user_path,
         "logs", sizeof(g_defaults.dirs[DEFAULT_DIR_LOGS]));

   /* cache dir */
   fill_pathname_join(g_defaults.dirs[DEFAULT_DIR_CACHE], "/tmp/",
         "retroarch", sizeof(g_defaults.dirs[DEFAULT_DIR_CACHE]));

   /* history and main config */
   strlcpy(g_defaults.dirs[DEFAULT_DIR_CONTENT_HISTORY],
         user_path, sizeof(g_defaults.dirs[DEFAULT_DIR_CONTENT_HISTORY]));
   fill_pathname_join(g_defaults.path_config, user_path,
         FILE_PATH_MAIN_CONFIG, sizeof(g_defaults.path_config));

#ifndef IS_SALAMANDER
   dir_check_defaults("custom.ini");
#endif
}

int main(int argc, char *argv[])
{
   dummyErrnoCodes();

   emscripten_set_canvas_element_size("#canvas", 800, 600);
   emscripten_set_element_css_size("#canvas", 800.0, 600.0);
#ifndef WRC
   emscripten_set_main_loop(emscripten_mainloop, 0, 0);
#endif
   rarch_main(argc, argv, NULL);

   return 0;
}

frontend_ctx_driver_t frontend_ctx_emscripten = {
   frontend_emscripten_get_env,  /* environment_get */
   NULL,                         /* init */
   NULL,                         /* deinit */
   NULL,                         /* exitspawn */
   NULL,                         /* process_args */
   NULL,                         /* exec */
   NULL,                         /* set_fork */
   NULL,                         /* shutdown */
   NULL,                         /* get_name */
   NULL,                         /* get_os */
   NULL,                         /* get_rating */
   NULL,                         /* load_content */
   NULL,                         /* get_architecture */
   NULL,                         /* get_powerstate */
   NULL,                         /* parse_drive_list */
   NULL,                         /* get_total_mem */
   NULL,                         /* get_free_mem  */
   NULL,                         /* install_sighandlers */
   NULL,                         /* get_signal_handler_state */
   NULL,                         /* set_signal_handler_state */
   NULL,                         /* destroy_signal_handler_state */
   NULL,                         /* attach_console */
   NULL,                         /* detach_console */
   NULL,                         /* get_lakka_version */
   NULL,                         /* set_screen_brightness */
   NULL,                         /* watch_path_for_changes */
   NULL,                         /* check_for_path_changes */
   NULL,                         /* set_sustained_performance_mode */
   NULL,                         /* get_cpu_model_name */
   NULL,                         /* get_user_language */
   NULL,                         /* is_narrator_running */
   NULL,                         /* accessibility_speak */
   "emscripten",                 /* ident               */
   NULL                          /* get_video_driver    */
};

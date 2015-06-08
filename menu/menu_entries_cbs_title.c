/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2011-2015 - Daniel De Matteis
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string/string_list.h>
#include <string/stdstring.h>
#include <file/file_path.h>

#include <rhash.h>

#include "menu.h"
#include "menu_entries_cbs.h"

static INLINE void replace_chars(char *str, char c1, char c2)
{
   char *pos;
   while((pos = strchr(str, c1)))
      *pos = c2;
}

static INLINE void sanitize_to_string(char *s, const char *label, size_t len)
{
   char new_label[PATH_MAX_LENGTH];
   strlcpy(new_label, label, sizeof(new_label));
   strlcpy(s, string_to_upper(new_label), len);
   replace_chars(s, '_', ' ');
}

static int action_get_title_generic(char *s, size_t len, const char *path,
      const char *text)
{
   char elem0_path[PATH_MAX_LENGTH], elem1_path[PATH_MAX_LENGTH];
   struct string_list *list_path  = string_split(path, "|");

   *elem0_path = *elem1_path = 0;

   if (list_path)
   {
      if (list_path->size > 0)
      {
         strlcpy(elem0_path, list_path->elems[0].data, sizeof(elem0_path));
         if (list_path->size > 1)
            strlcpy(elem1_path, list_path->elems[1].data, sizeof(elem1_path));
      }
      string_list_free(list_path);
   }
   snprintf(s, len, "%s - %s", text,
         (elem0_path[0] != '\0') ? path_basename(elem0_path) : "");

   return 0;
}

static int action_get_title_deferred_database_manager_list(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   return action_get_title_generic(s, len, path, "DATABASE SELECTION");
}

static int action_get_title_deferred_cursor_manager_list(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   return action_get_title_generic(s, len, path, "DATABASE CURSOR LIST");
}

static int action_get_title_list_rdb_entry_developer(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   return action_get_title_generic(s, len, path, "DATABASE CURSOR LIST - FILTER: DEVELOPER ");
}

static int action_get_title_list_rdb_entry_publisher(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   return action_get_title_generic(s, len, path, "DATABASE CURSOR LIST - FILTER: PUBLISHER ");
}

static int action_get_title_list_rdb_entry_origin(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   return action_get_title_generic(s, len, path, "DATABASE CURSOR LIST - FILTER: ORIGIN ");
}

static int action_get_title_list_rdb_entry_franchise(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   return action_get_title_generic(s, len, path, "DATABASE CURSOR LIST - FILTER: FRANCHISE ");
}

static int action_get_title_list_rdb_entry_edge_magazine_rating(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   return action_get_title_generic(s, len, path, "DATABASE CURSOR LIST - FILTER: EDGE MAGAZINE RATING ");
}

static int action_get_title_list_rdb_entry_edge_magazine_issue(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   return action_get_title_generic(s, len, path, "DATABASE CURSOR LIST - FILTER: EDGE MAGAZINE ISSUE ");
}

static int action_get_title_list_rdb_entry_releasedate_by_month(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   return action_get_title_generic(s, len, path, "DATABASE CURSOR LIST - FILTER: RELEASEDATE BY MONTH ");
}

static int action_get_title_list_rdb_entry_releasedate_by_year(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   return action_get_title_generic(s, len, path, "DATABASE CURSOR LIST - FILTER: RELEASEDATE BY YEAR ");
}

static int action_get_title_list_rdb_entry_esrb_rating(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   return action_get_title_generic(s, len, path, "DATABASE CURSOR LIST - FILTER: ESRB RATING ");
}

static int action_get_title_list_rdb_entry_database_info(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
#if 0
   snprintf(s, len, "DATABASE INFO: %s", elem1);
#endif
   return action_get_title_generic(s, len, path, "DATABASE INFO ");
}

static int action_get_title_list_rdb_entry_elspa_rating(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   return action_get_title_generic(s, len, path, "DATABASE CURSOR LIST - FILTER: ELSPA RATING ");
}

static int action_get_title_list_rdb_entry_pegi_rating(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   return action_get_title_generic(s, len, path, "DATABASE CURSOR LIST - FILTER: PEGI RATING ");
}

static int action_get_title_list_rdb_entry_cero_rating(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   return action_get_title_generic(s, len, path, "DATABASE CURSOR LIST - FILTER: CERO RATING ");
}

static int action_get_title_list_rdb_entry_bbfc_rating(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   return action_get_title_generic(s, len, path, "DATABASE CURSOR LIST - FILTER: BBFC RATING ");
}

static int action_get_title_list_rdb_entry_max_users(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   return action_get_title_generic(s, len, path, "DATABASE CURSOR LIST - FILTER: MAX USERS ");
}

static int action_get_title_deferred_core_list(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "DETECTED CORES %s", path);
   return 0;
}

static int action_get_title_default(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   uint32_t label_hash = 0;
   char elem1[PATH_MAX_LENGTH];
   driver_t               *driver = driver_get_ptr();

   *elem1 = 0;

   label_hash = djb2_calculate(label);

#if 0
   RARCH_LOG("label %s, elem0 %s, elem1 %s\n", label, elem0, elem1);
#endif

   switch (label_hash)
   {
      case MENU_LABEL_DISK_IMAGE_APPEND:
         snprintf(s, len, "DISK APPEND %s", path);
         break;
      case MENU_LABEL_VIDEO_SHADER_PRESET:
         snprintf(s, len, "SHADER PRESET %s", path);
         break;
      case MENU_LABEL_CHEAT_FILE_LOAD:
         snprintf(s, len, "CHEAT FILE %s", path);
         break;
      case MENU_LABEL_REMAP_FILE_LOAD:
         snprintf(s, len, "REMAP FILE %s", path);
         break;
      case MENU_LABEL_CONTENT_COLLECTION_LIST:
         snprintf(s, len, "CONTENT COLLECTION LIST");
         break;
      case MENU_LABEL_CUSTOM_VIEWPORT_2:
         strlcpy(s, "CUSTOM VIEWPORT", len);
         break;
      case MENU_LABEL_HELP:
         strlcpy(s, "HELP", len);
         break;
      case MENU_LABEL_INPUT_OVERLAY:
         snprintf(s, len, "OVERLAY %s", path);
         break;
      case MENU_LABEL_VIDEO_FONT_PATH:
         snprintf(s, len, "FONT %s", path);
         break;
      case MENU_LABEL_VIDEO_FILTER:
         snprintf(s, len, "FILTER %s", path);
         break;
      case MENU_LABEL_AUDIO_DSP_PLUGIN:
         snprintf(s, len, "DSP FILTER %s", path);
         break;
      case MENU_LABEL_CHEAT_DATABASE_PATH:
         snprintf(s, len, "CHEAT DIR %s", path);
         break;
      case MENU_LABEL_LIBRETRO_DIR_PATH:
         snprintf(s, len, "CORE DIR %s", path);
         break;
      case MENU_LABEL_LIBRETRO_INFO_PATH:
         snprintf(s, len, "CORE INFO DIR %s", path);
         break;
      default:
         switch (menu_type)
         {
            case MENU_SETTINGS_CUSTOM_VIEWPORT:
               strlcpy(s, "CUSTOM VIEWPORT", len);
               break;
            case MENU_SETTINGS:
               snprintf(s, len, "MENU %s", path);
               break;
            case MENU_SETTINGS_CUSTOM_BIND:
            case MENU_SETTINGS_CUSTOM_BIND_KEYBOARD:
               strlcpy(s, "INPUT SETTINGS", len);
               if (elem1[0] != '\0')
               {
                  strlcat(s, " - ", len);
                  strlcat(s, string_to_upper(elem1), len);
               }
               break;
            default:
               if (driver->menu->defer_core)
                  snprintf(s, len, "CONTENT %s", path);
               else
               {
                  global_t *global      = global_get_ptr();
                  const char *core_name = global->menu.info.library_name;

                  if (!core_name)
                     core_name = global->system.info.library_name;
                  if (!core_name)
                     core_name = "No Core";
                  snprintf(s, len, "CONTENT (%s) %s", core_name, path);
               }
               break;
         }
         break;
   }
   
   return 0;
}

static int action_get_title_group_settings(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   char elem0[PATH_MAX_LENGTH], elem1[PATH_MAX_LENGTH];
   struct string_list *list_label = string_split(label, "|");

   if (list_label)
   {
      if (list_label->size > 0)
      {
         strlcpy(elem0, list_label->elems[0].data, sizeof(elem0));
         if (list_label->size > 1)
            strlcpy(elem1, list_label->elems[1].data, sizeof(elem1));
      }
      string_list_free(list_label);
   }

   strlcpy(s, string_to_upper(elem0), len);
   if (elem1[0] != '\0')
   {
      strlcat(s, " - ", len);
      strlcat(s, string_to_upper(elem1), len);
   }
   return 0;
}

static int action_get_title_action_generic(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   sanitize_to_string(s, label, len);
   return 0;
}

static int action_get_title_configurations(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "CONFIG %s", path);
   return 0;
}

static int action_get_title_content_database_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "DATABASE DIR %s", path);
   return 0;
}

static int action_get_title_savestate_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "SAVESTATE DIR %s", path);
   return 0;
}

static int action_get_title_dynamic_wallpapers_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "DYNAMIC WALLPAPERS DIR %s", path);
   return 0;
}

static int action_get_title_core_assets_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "CORE ASSETS DIR %s", path);
   return 0;
}

static int action_get_title_config_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "CONFIG DIR %s", path);
   return 0;
}

static int action_get_title_input_remapping_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "INPUT REMAPPING DIR %s", path);
   return 0;
}

static int action_get_title_autoconfig_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "AUTOCONFIG DIR %s", path);
   return 0;
}

static int action_get_title_playlist_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "PLAYLIST DIR %s", path);
   return 0;
}

static int action_get_title_browser_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "BROWSER DIR %s", path);
   return 0;
}

static int action_get_title_content_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "CONTENT DIR %s", path);
   return 0;
}

static int action_get_title_screenshot_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "SCREENSHOT DIR %s", path);
   return 0;
}

static int action_get_title_cursor_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "CURSOR DIR %s", path);
   return 0;
}

static int action_get_title_onscreen_overlay_keyboard_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "OSK OVERLAY DIR %s", path);
   return 0;
}

static int action_get_title_recording_config_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "RECORDING CONFIG DIR %s", path);
   return 0;
}

static int action_get_title_recording_output_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "RECORDING OUTPUT DIR %s", path);
   return 0;
}

static int action_get_title_video_shader_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "SHADER DIR %s", path);
   return 0;
}

static int action_get_title_audio_filter_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "AUDIO FILTER DIR %s", path);
   return 0;
}

static int action_get_title_video_filter_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "VIDEO FILTER DIR %s", path);
   return 0;
}

static int action_get_title_savefile_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "SAVEFILE DIR %s", path);
   return 0;
}

static int action_get_title_overlay_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "OVERLAY DIR %s", path);
   return 0;
}

static int action_get_title_system_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "SYSTEM DIR %s", path);
   return 0;
}

static int action_get_title_assets_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "ASSETS DIR %s", path);
   return 0;
}

static int action_get_title_extraction_directory(const char *path, const char *label, 
      unsigned menu_type, char *s, size_t len)
{
   snprintf(s, len, "EXTRACTION DIR %s", path);
   return 0;
}

static int menu_entries_cbs_init_bind_title_compare_label(menu_file_list_cbs_t *cbs,
      const char *label, uint32_t label_hash, const char *elem1)
{
   rarch_setting_t *setting = menu_setting_find(label);

   if (setting)
   {
      if (!strcmp(setting->parent_group, "Main Menu") && setting->type == ST_GROUP)
      {
         if (elem1[0] != '\0')
         {
            cbs->action_get_title = action_get_title_group_settings;
            return 0;
         }
      }
   }

   switch (label_hash)
   {
      case MENU_LABEL_DEFERRED_DATABASE_MANAGER_LIST:
         cbs->action_get_title = action_get_title_deferred_database_manager_list;
         break;
      case MENU_LABEL_DEFERRED_CURSOR_MANAGER_LIST:
         cbs->action_get_title = action_get_title_deferred_cursor_manager_list;
         break;
      case MENU_LABEL_DEFERRED_CURSOR_MANAGER_LIST_RDB_ENTRY_DEVELOPER:
         cbs->action_get_title = action_get_title_list_rdb_entry_developer;
         break;
      case MENU_LABEL_DEFERRED_CURSOR_MANAGER_LIST_RDB_ENTRY_PUBLISHER:
         cbs->action_get_title = action_get_title_list_rdb_entry_publisher;
         break;
      case MENU_LABEL_DEFERRED_CURSOR_MANAGER_LIST_RDB_ENTRY_ORIGIN:
         cbs->action_get_title = action_get_title_list_rdb_entry_origin;
         break;
      case MENU_LABEL_DEFERRED_CURSOR_MANAGER_LIST_RDB_ENTRY_FRANCHISE:
         cbs->action_get_title = action_get_title_list_rdb_entry_franchise;
         break;
      case MENU_LABEL_DEFERRED_CURSOR_MANAGER_LIST_RDB_ENTRY_EDGE_MAGAZINE_RATING:
         cbs->action_get_title = action_get_title_list_rdb_entry_edge_magazine_rating;
         break;
      case MENU_LABEL_DEFERRED_CURSOR_MANAGER_LIST_RDB_ENTRY_EDGE_MAGAZINE_ISSUE:
         cbs->action_get_title = action_get_title_list_rdb_entry_edge_magazine_issue;
         break;
      case MENU_LABEL_DEFERRED_CURSOR_MANAGER_LIST_RDB_ENTRY_RELEASEMONTH:
         cbs->action_get_title = action_get_title_list_rdb_entry_releasedate_by_month;
         break;
      case MENU_LABEL_DEFERRED_CURSOR_MANAGER_LIST_RDB_ENTRY_RELEASEYEAR:
         cbs->action_get_title = action_get_title_list_rdb_entry_releasedate_by_year;
         break;
      case MENU_LABEL_DEFERRED_CURSOR_MANAGER_LIST_RDB_ENTRY_ESRB_RATING:
         cbs->action_get_title = action_get_title_list_rdb_entry_esrb_rating;
         break;
      case MENU_LABEL_DEFERRED_CURSOR_MANAGER_LIST_RDB_ENTRY_ELSPA_RATING:
         cbs->action_get_title = action_get_title_list_rdb_entry_elspa_rating;
         break;
      case MENU_LABEL_DEFERRED_CURSOR_MANAGER_LIST_RDB_ENTRY_PEGI_RATING:
         cbs->action_get_title = action_get_title_list_rdb_entry_pegi_rating;
         break;
      case MENU_LABEL_DEFERRED_CURSOR_MANAGER_LIST_RDB_ENTRY_CERO_RATING:
         cbs->action_get_title = action_get_title_list_rdb_entry_cero_rating;
         break;
      case MENU_LABEL_DEFERRED_CURSOR_MANAGER_LIST_RDB_ENTRY_BBFC_RATING:
         cbs->action_get_title = action_get_title_list_rdb_entry_bbfc_rating;
         break;
      case MENU_LABEL_DEFERRED_CURSOR_MANAGER_LIST_RDB_ENTRY_MAX_USERS:
         cbs->action_get_title = action_get_title_list_rdb_entry_max_users;
         break;
      case MENU_LABEL_DEFERRED_RDB_ENTRY_DETAIL:
         cbs->action_get_title = action_get_title_list_rdb_entry_database_info;
         break;
      case MENU_LABEL_DEFERRED_CORE_LIST:
         cbs->action_get_title = action_get_title_deferred_core_list;
         break;
      case MENU_LABEL_CONFIGURATIONS:
         cbs->action_get_title = action_get_title_configurations;
         break;
      case MENU_LABEL_JOYPAD_AUTOCONFIG_DIR:
         cbs->action_get_title = action_get_title_autoconfig_directory;
         break;
      case MENU_LABEL_EXTRACTION_DIRECTORY:
         cbs->action_get_title = action_get_title_extraction_directory;
         break;
      case MENU_LABEL_SYSTEM_DIRECTORY:
         cbs->action_get_title = action_get_title_system_directory;
         break;
      case MENU_LABEL_ASSETS_DIRECTORY:
         cbs->action_get_title = action_get_title_assets_directory;
         break;
      case MENU_LABEL_SAVEFILE_DIRECTORY:
         cbs->action_get_title = action_get_title_savefile_directory;
         break;
      case MENU_LABEL_OVERLAY_DIRECTORY:
         cbs->action_get_title = action_get_title_overlay_directory;
         break;
      case MENU_LABEL_RGUI_BROWSER_DIRECTORY:
         cbs->action_get_title = action_get_title_browser_directory;
         break;
      case MENU_LABEL_PLAYLIST_DIRECTORY:
         cbs->action_get_title = action_get_title_playlist_directory;
         break;
      case MENU_LABEL_CONTENT_DIRECTORY:
         cbs->action_get_title = action_get_title_content_directory;
         break;
      case MENU_LABEL_SCREENSHOT_DIRECTORY:
         cbs->action_get_title = action_get_title_screenshot_directory;
         break;
      case MENU_LABEL_VIDEO_SHADER_DIR:
         cbs->action_get_title = action_get_title_video_shader_directory;
         break;
      case MENU_LABEL_VIDEO_FILTER_DIR:
         cbs->action_get_title = action_get_title_video_filter_directory;
         break;
      case MENU_LABEL_AUDIO_FILTER_DIR:
         cbs->action_get_title = action_get_title_audio_filter_directory;
         break;
      case MENU_LABEL_CURSOR_DIRECTORY:
         cbs->action_get_title = action_get_title_cursor_directory;
         break;
      case MENU_LABEL_RECORDING_CONFIG_DIRECTORY:
         cbs->action_get_title = action_get_title_recording_config_directory;
         break;
      case MENU_LABEL_RECORDING_OUTPUT_DIRECTORY:
         cbs->action_get_title = action_get_title_recording_output_directory;
         break;
      case MENU_LABEL_OSK_OVERLAY_DIRECTORY:
         cbs->action_get_title = action_get_title_onscreen_overlay_keyboard_directory;
         break;
      case MENU_LABEL_INPUT_REMAPPING_DIRECTORY:
         cbs->action_get_title = action_get_title_input_remapping_directory;
         break;
      case MENU_LABEL_CONTENT_DATABASE_DIRECTORY:
         cbs->action_get_title = action_get_title_content_database_directory;
         break;
      case MENU_LABEL_SAVESTATE_DIRECTORY:
         cbs->action_get_title = action_get_title_savestate_directory;
         break;
      case MENU_LABEL_DYNAMIC_WALLPAPERS_DIRECTORY:
         cbs->action_get_title = action_get_title_dynamic_wallpapers_directory;
         break;
      case MENU_LABEL_CORE_ASSETS_DIRECTORY:
         cbs->action_get_title = action_get_title_core_assets_directory;
         break;
      case MENU_LABEL_RGUI_CONFIG_DIRECTORY:
         cbs->action_get_title = action_get_title_config_directory;
         break;
      case MENU_LABEL_PERFORMANCE_COUNTERS:
      case MENU_LABEL_CORE_LIST:
      case MENU_LABEL_MANAGEMENT:
      case MENU_LABEL_OPTIONS:
      case MENU_LABEL_SETTINGS:
      case MENU_LABEL_FRONTEND_COUNTERS:
      case MENU_LABEL_CORE_COUNTERS:
      case MENU_LABEL_HISTORY_LIST:
      case MENU_LABEL_INFO_SCREEN:
      case MENU_LABEL_SYSTEM_INFORMATION:
      case MENU_LABEL_CORE_INFORMATION:
      case MENU_LABEL_VIDEO_SHADER_PARAMETERS:
      case MENU_LABEL_VIDEO_SHADER_PRESET_PARAMETERS:
      case MENU_LABEL_DISK_OPTIONS:
      case MENU_LABEL_CORE_OPTIONS:
      case MENU_LABEL_SHADER_OPTIONS:
      case MENU_LABEL_VIDEO_OPTIONS:
      case MENU_LABEL_CORE_CHEAT_OPTIONS:
      case MENU_LABEL_CORE_INPUT_REMAPPING_OPTIONS:
      case MENU_LABEL_DATABASE_MANAGER_LIST:
      case MENU_LABEL_CURSOR_MANAGER_LIST:
      case MENU_LABEL_DEFERRED_CORE_UPDATER_LIST:
         cbs->action_get_title = action_get_title_action_generic;
         break;
      default:
         return -1;
   }

   return 0;
}

int menu_entries_cbs_init_bind_title(menu_file_list_cbs_t *cbs,
      const char *path, const char *label, unsigned type, size_t idx,
      const char *elem0, const char *elem1,
      uint32_t label_hash, uint32_t menu_label_hash)
{
   driver_t               *driver = driver_get_ptr();

   if (!cbs)
      return -1;

   cbs->action_get_title = action_get_title_default;

#if 0
   RARCH_LOG("label %s, elem0 %s, elem1 %s\n", label, elem0, elem1);
#endif

   if (menu_entries_cbs_init_bind_title_compare_label(cbs, label, label_hash, elem1) == 0)
      return 0;

   return -1;
}

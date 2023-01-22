#ifndef _MSC_VER
#include <stdbool.h>
#include <sched.h>
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#include <string/stdstring.h>
#include <file/file_path.h>
#include <streams/file_stream.h>

#include <libretro.h>
#include "libretro_core_options.h"
#include "a5200_osk.h"

#include "altirra_5200_os.h"
#include "atari.h"
#include "cartridge.h"
#include "input.h"
#include "pia.h"
#include "pokeysnd.h"
#include "statesav.h"

#include "chd.h"

#ifdef _3DS
extern void* linearMemAlign(size_t size, size_t alignment);
extern void linearFree(void* mem);
#endif

#ifdef WRC
#include <emscripten.h>
#endif

static retro_log_printf_t log_cb;
static retro_video_refresh_t video_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static retro_environment_t environ_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;

#ifndef PATH_MAX_LENGTH
#if defined(_XBOX1) || defined(_3DS) || defined(PSP) || defined(PS2) || defined(GEKKO)|| defined(WIIU) || defined(ORBIS) || defined(__PSL1GHT__) || defined(__PS3__)
#define PATH_MAX_LENGTH 512
#else
#define PATH_MAX_LENGTH 4096
#endif
#endif

#ifndef PI
#define PI 3.141592653589793238
#endif

#define A5200_BIOS_FILE_NAME "5200.rom"
#define A5200_BIOS_SIZE 0x800
#define A5200_SAVE_STATE_SIZE 131361

#define A5200_PALETTE_SIZE 256
#define A5200_SCREEN_BUFFER_WIDTH 512
#define A5200_SCREEN_BUFFER_HEIGHT 512
/* Note: Maximum Atari 5200 resolution is 320x192,
 * but outputting at this resolution causes severe
 * vertical cropping in most games. A vertical
 * display resolution of 224 seems to be the best
 * value for 'safely' cropping overscan */
#define A5200_VIDEO_WIDTH 320
#define A5200_VIDEO_HEIGHT 224
#define A5200_VIDEO_OFFSET_X ((ATARI_WIDTH - A5200_VIDEO_WIDTH) >> 1)
#define A5200_VIDEO_OFFSET_Y ((ATARI_HEIGHT - A5200_VIDEO_HEIGHT) >> 1)

#define A5200_FPS 60
#define A5200_AUDIO_BUFFER_SIZE (SOUND_SAMPLE_RATE / A5200_FPS)

#define LIBRETRO_ANALOG_RANGE 0x8000
#define A5200_JOY_MIN 6
#define A5200_JOY_MAX 220
#define A5200_JOY_CENTER 114
#define A5200_VIRTUAL_NUMPAD_THRESHOLD 0.7
#define A5200_NUM_PADS 2

/* Delay (in frames) between OSK cursor
 * movements when holding a direction */
#define A5200_OSK_CURSOR_REPEAT_DELAY 10

static const uint32_t a5200_palette_ntsc[A5200_PALETTE_SIZE] = {
   0x000000, 0x252525, 0x343434, 0x4F4F4F,
   0x5B5B5B, 0x696969, 0x7B7B7B, 0x8A8A8A,
   0xA7A7A7, 0xB9B9B9, 0xC5C5C5, 0xD0D0D0,
   0xD7D7D7, 0xE1E1E1, 0xF4F4F4, 0xFFFFFF,
   0x4C3200, 0x623A00, 0x7B4A00, 0x9A6000,
   0xB57400, 0xCC8500, 0xE79E08, 0xF7AF10,
   0xFFC318, 0xFFD020, 0xFFD828, 0xFFDF30,
   0xFFE63B, 0xFFF440, 0xFFFA4B, 0xFFFF50,
   0x992500, 0xAA2500, 0xB42500, 0xD33000,
   0xDD4802, 0xE25009, 0xF46700, 0xF47510,
   0xFF9E10, 0xFFAC20, 0xFFBA3A, 0xFFBF50,
   0xFFC66D, 0xFFD580, 0xFFE490, 0xFFE699,
   0x980C0C, 0x990C0C, 0xC21300, 0xD31300,
   0xE23500, 0xE34000, 0xE44020, 0xE55230,
   0xFD7854, 0xFF8A6A, 0xFF987C, 0xFFA48B,
   0xFFB39E, 0xFFC2B2, 0xFFD0BA, 0xFFD7C0,
   0x990000, 0xA90000, 0xC20400, 0xD30400,
   0xDA0400, 0xDB0800, 0xE42020, 0xF64040,
   0xFB7070, 0xFB7E7E, 0xFB8F8F, 0xFF9F9F,
   0xFFABAB, 0xFFB9B9, 0xFFC9C9, 0xFFCFCF,
   0x7E0050, 0x800050, 0x80005F, 0x950B74,
   0xAA2288, 0xBB2F9A, 0xCE3FAD, 0xD75AB6,
   0xE467C3, 0xEF72CE, 0xFB7EDA, 0xFF8DE1,
   0xFF9DE5, 0xFFA5E7, 0xFFAFEA, 0xFFB8EC,
   0x48006C, 0x5C0488, 0x650D90, 0x7B23A7,
   0x933BBF, 0x9D45C9, 0xA74FD3, 0xB25ADE,
   0xBD65E9, 0xC56DF1, 0xCE76FA, 0xD583FF,
   0xDA90FF, 0xDE9CFF, 0xE2A9FF, 0xE6B6FF,
   0x1B0070, 0x221B8D, 0x3730A2, 0x4841B3,
   0x5952C4, 0x635CCE, 0x6F68DA, 0x7D76E8,
   0x8780F8, 0x938CFF, 0x9D97FF, 0xA8A3FF,
   0xB3AFFF, 0xBCB8FF, 0xC4C1FF, 0xDAD1FF,
   0x000D7F, 0x0012A7, 0x0018C0, 0x0A2BD1,
   0x1B4AE3, 0x2F58F0, 0x3768FF, 0x4979FF,
   0x5B85FF, 0x6D96FF, 0x7FA3FF, 0x8CADFF,
   0x96B4FF, 0xA8C0FF, 0xB7CBFF, 0xC6D6FF,
   0x00295A, 0x003876, 0x004892, 0x005CAC,
   0x0071C6, 0x0086D0, 0x0A9BDF, 0x1AA8EC,
   0x2BB6FF, 0x3FC2FF, 0x45CBFF, 0x59D3FF,
   0x7FDAFF, 0x8FDEFF, 0xA0E2FF, 0xB0EBFF,
   0x004A00, 0x004C00, 0x006A20, 0x508E79,
   0x409999, 0x009CAA, 0x00A1BB, 0x01A4CC,
   0x03A5D7, 0x05DAE2, 0x18E5FF, 0x34EAFF,
   0x49EFFF, 0x66F2FF, 0x84F4FF, 0x9EF9FF,
   0x004A00, 0x005D00, 0x007000, 0x008300,
   0x009500, 0x00AB00, 0x07BD07, 0x0AD00A,
   0x1AD540, 0x5AF177, 0x82EFA7, 0x84EDD1,
   0x89FFED, 0x7DFFFF, 0x93FFFF, 0x9BFFFF,
   0x224A03, 0x275304, 0x306405, 0x3C770C,
   0x458C11, 0x5AA513, 0x1BD209, 0x1FDD00,
   0x3DCD2D, 0x3DCD30, 0x58CC40, 0x60D350,
   0xA2EC55, 0xB3F24A, 0xBBF65D, 0xC4F870,
   0x2E3F0C, 0x364A0F, 0x405615, 0x465F17,
   0x57771A, 0x65851C, 0x74931D, 0x8FA525,
   0xADB72C, 0xBCC730, 0xC9D533, 0xD4E03B,
   0xE0EC42, 0xEAF645, 0xF0FD47, 0xF4FF6F,
   0x552400, 0x5A2C00, 0x6C3B00, 0x794B00,
   0xB97500, 0xBB8500, 0xC1A120, 0xD0B02F,
   0xDEBE3F, 0xE6C645, 0xEDCD57, 0xF5DB62,
   0xFBE569, 0xFCEE6F, 0xFDF377, 0xFDF37F,
   0x5C2700, 0x5C2F00, 0x713B00, 0x7B4800,
   0xB96820, 0xBB7220, 0xC58629, 0xD79633,
   0xE6A440, 0xF4B14B, 0xFDC158, 0xFFCC55,
   0xFFD461, 0xFFDD69, 0xFFE679, 0xFFEA98
};
static uint16_t a5200_palette_rgb565[A5200_PALETTE_SIZE] = {0};

static const unsigned input_analog_numpad_map[8] = {
   AKEY_5200_7,
   AKEY_5200_8,
   AKEY_5200_9,
   AKEY_5200_6,
   AKEY_5200_3,
   AKEY_5200_2,
   AKEY_5200_1,
   AKEY_5200_4,
};

static struct retro_input_descriptor input_descriptors[] = {
   { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,   "Joystick Left (Digital)" },
   { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,     "Joystick Up (Digital)" },
   { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,   "Joystick Down (Digital)" },
   { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT,  "Joystick Right (Digital)" },
   { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,      "Fire 1 / OSK Select" },
   { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,      "Fire 2" },
   { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,      "NumPad #" },
   { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,      "NumPad *" },
   { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,      "NumPad 0" },
   { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3,     "NumPad 5" },
   { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,      "Show/Hide OSK" },
   { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Pause" },
   { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,  "Start" },
   { 0, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,  RETRO_DEVICE_ID_ANALOG_X, "Joystick X (Analog)" },
   { 0, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,  RETRO_DEVICE_ID_ANALOG_Y, "Joystick Y (Analog)" },
   { 0, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_X, "NumPad [1-9]" },
   { 0, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_Y, "NumPad [1-9]" },

   { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "Joystick Left (Digital)" },
   { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "Joystick Up (Digital)" },
   { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "Joystick Down (Digital)" },
   { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Joystick Right (Digital)" },
   { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "Fire 1" },
   { 1, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_X,  "Joystick X (Analog)" },
   { 1, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_Y,  "Joystick Y (Analog)" },

   { 0 },
};

uint8_t *a5200_screen_buffer         = NULL;
static uint16_t *video_buffer        = NULL;
static uint16_t *video_buffer_prev   = NULL;
static uint8_t *audio_samples_buffer = NULL;
static int16_t *audio_out_buffer     = NULL;

static bool audio_low_pass_enabled  = false;
static int32_t audio_low_pass_range = (60 * 0x10000) / 100;
static int32_t audio_low_pass_prev  = 0;

enum input_hack_type
{
   INPUT_HACK_NONE = 0,
   INPUT_HACK_DUAL_STICK,
   INPUT_HACK_SWAP_PORTS
};

static unsigned input_shift_ctrl       = 0;
static enum input_hack_type input_hack = INPUT_HACK_NONE;
static int input_analog_deadzone       = (int)(0.15f * (float)LIBRETRO_ANALOG_RANGE);
static bool input_analog_quadratic     = false;

static bool input_show_osk             = false;
static bool input_osk_toggle_lock      = false;
static uint16_t input_osk_cursor_latch = 0;

static uint8_t *rom_buf        = NULL;
static const uint8_t *rom_data = NULL;
static size_t rom_size         = 0;

static bool a5200_use_official_bios = true;

static bool libretro_supports_bitmasks = false;

extern UBYTE PCPOT_input[8];

/************************************
 * Interframe blending
 ************************************/

enum frame_blend_method
{
   FRAME_BLEND_NONE = 0,
   FRAME_BLEND_MIX,
   FRAME_BLEND_GHOST_65,
   FRAME_BLEND_GHOST_75,
   FRAME_BLEND_GHOST_85,
   FRAME_BLEND_GHOST_95
};

/* It would be more flexible to have 'persistence'
 * as a core option, but using a variable parameter
 * reduces performance by ~15%. We therefore offer
 * fixed values, and use macros to avoid excessive
 * duplication of code...
 * Note: persistence fraction is (persistence/128),
 * using a power of 2 like this further increases
 * performance by ~15% */
#define BLEND_FRAMES_GHOST(persistence)                                                               \
{                                                                                                     \
   uint16_t *curr = video_buffer;                                                                     \
   uint16_t *prev = video_buffer_prev;                                                                \
   size_t x, y;                                                                                       \
                                                                                                      \
   for (y = 0; y < A5200_VIDEO_HEIGHT; y++)                                                           \
   {                                                                                                  \
      for (x = 0; x < A5200_VIDEO_WIDTH; x++)                                                         \
      {                                                                                               \
         /* Get colours from current + previous frames */                                             \
         uint16_t color_curr = *(curr);                                                               \
         uint16_t color_prev = *(prev);                                                               \
                                                                                                      \
         /* Unpack colours */                                                                         \
         uint16_t r_curr     = (color_curr >> 11) & 0x1F;                                             \
         uint16_t g_curr     = (color_curr >>  6) & 0x1F;                                             \
         uint16_t b_curr     = (color_curr      ) & 0x1F;                                             \
                                                                                                      \
         uint16_t r_prev     = (color_prev >> 11) & 0x1F;                                             \
         uint16_t g_prev     = (color_prev >>  6) & 0x1F;                                             \
         uint16_t b_prev     = (color_prev      ) & 0x1F;                                             \
                                                                                                      \
         /* Mix colors */                                                                             \
         uint16_t r_mix      = ((r_curr * (128 - persistence)) >> 7) + ((r_prev * persistence) >> 7); \
         uint16_t g_mix      = ((g_curr * (128 - persistence)) >> 7) + ((g_prev * persistence) >> 7); \
         uint16_t b_mix      = ((b_curr * (128 - persistence)) >> 7) + ((b_prev * persistence) >> 7); \
                                                                                                      \
         /* Output colour is the maximum of the input                                                 \
          * and decayed values */                                                                     \
         uint16_t r_out      = (r_mix > r_curr) ? r_mix : r_curr;                                     \
         uint16_t g_out      = (g_mix > g_curr) ? g_mix : g_curr;                                     \
         uint16_t b_out      = (b_mix > b_curr) ? b_mix : b_curr;                                     \
         uint16_t color_out  = r_out << 11 | g_out << 6 | b_out;                                      \
                                                                                                      \
         /* Assign colour and store for next frame */                                                 \
         *(prev++)           = color_out;                                                             \
         *(curr++)           = color_out;                                                             \
      }                                                                                               \
   }                                                                                                  \
}

static void blend_frames_mix(void)
{
   uint16_t *curr = video_buffer;
   uint16_t *prev = video_buffer_prev;
   size_t x, y;

   for (y = 0; y < A5200_VIDEO_HEIGHT; y++)
   {
      for (x = 0; x < A5200_VIDEO_WIDTH; x++)
      {
         /* Get colours from current + previous frames */
         uint16_t color_curr = *(curr);
         uint16_t color_prev = *(prev);

         /* Store colours for next frame */
         *(prev++) = color_curr;

         /* Mix colours */
         *(curr++) = (color_curr + color_prev + ((color_curr ^ color_prev) & 0x821)) >> 1;
      }
   }
}

static void blend_frames_ghost65(void)
{
   /* 65% = 83 / 128 */
   BLEND_FRAMES_GHOST(83);
}

static void blend_frames_ghost75(void)
{
   /* 75% = 95 / 128 */
   BLEND_FRAMES_GHOST(95);
}

static void blend_frames_ghost85(void)
{
   /* 85% ~= 109 / 128 */
   BLEND_FRAMES_GHOST(109);
}

static void blend_frames_ghost95(void)
{
   /* 95% ~= 122 / 128 */
   BLEND_FRAMES_GHOST(122);
}

static void (*blend_frames)(void) = NULL;

static void init_frame_blending(enum frame_blend_method blend_method)
{
   /* Allocate/zero out buffer, if required */
   if (blend_method != FRAME_BLEND_NONE)
   {
      if (!video_buffer_prev)
         video_buffer_prev = (uint16_t*)malloc(A5200_VIDEO_WIDTH *
               A5200_VIDEO_HEIGHT * sizeof(uint16_t));

      memset(video_buffer_prev, 0, A5200_VIDEO_WIDTH *
            A5200_VIDEO_HEIGHT * sizeof(uint16_t));
   }

   /* Assign function pointer */
   switch (blend_method)
   {
      case FRAME_BLEND_MIX:
         blend_frames = blend_frames_mix;
         break;
      case FRAME_BLEND_GHOST_65:
         blend_frames = blend_frames_ghost65;
         break;
      case FRAME_BLEND_GHOST_75:
         blend_frames = blend_frames_ghost75;
         break;
      case FRAME_BLEND_GHOST_85:
         blend_frames = blend_frames_ghost85;
         break;
      case FRAME_BLEND_GHOST_95:
         blend_frames = blend_frames_ghost95;
         break;
      default:
         blend_frames = NULL;
         break;
   }
}

/************************************
 * Auxiliary functions
 ************************************/

void a5200_log(enum retro_log_level level, const char *format, ...)
{
   char msg[512];
   va_list ap;

   msg[0] = '\0';

   if (string_is_empty(format))
      return;

   va_start(ap, format);
   vsprintf(msg, format, ap);
   va_end(ap);

   if (log_cb)
      log_cb(level, "[a5200] %s", msg);
   else
      fprintf((level == RETRO_LOG_ERROR) ? stderr : stdout,
            "[a5200] %s", msg);
}

static void load_bios(void)
{
   const char *system_dir = NULL;
   RFILE *bios_file       = NULL;
   int64_t bytes_read     = 0;
   char bios_path[PATH_MAX_LENGTH];

   bios_path[0] = '\0';

   /* If user has selected the internal bios,
    * skip all file access */
   if (!a5200_use_official_bios)
      goto fallback;

   /* Get system directory */
   if (!environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &system_dir) ||
       !system_dir)
   {
      a5200_log(RETRO_LOG_ERROR,
            "No system directory defined, unable to look for bios.\n");
      goto fallback;
   }

   /* Get BIOS path */
   fill_pathname_join(bios_path, system_dir,
         A5200_BIOS_FILE_NAME, sizeof(bios_path));

   /* Read BIOS file */
   bios_file = filestream_open(bios_path,
         RETRO_VFS_FILE_ACCESS_READ,
         RETRO_VFS_FILE_ACCESS_HINT_NONE);

   if (!bios_file)
   {
      a5200_log(RETRO_LOG_ERROR,
            "Failed to open bios file: %s\n", bios_path);
      goto fallback;
   }

   bytes_read = filestream_read(bios_file,
         atari_os, A5200_BIOS_SIZE);
   filestream_close(bios_file);

   if (bytes_read != A5200_BIOS_SIZE)
   {
      a5200_log(RETRO_LOG_ERROR,
            "Failed to read bios file: %s\n", bios_path);
      goto fallback;
   }

   a5200_log(RETRO_LOG_INFO, "Read bios: %s\n", bios_path);
   return;

fallback:
   /* Official bios missing or disabled - use
    * internal open-source Altirra OS fallback
    * (reduced compatibility) */
   memcpy(atari_os, ROM_altirra_5200_os, A5200_BIOS_SIZE);

   /* If official bios was selected, display
    * a notification error message */
   if (a5200_use_official_bios)
   {
      unsigned msg_interface_version = 0;
      environ_cb(RETRO_ENVIRONMENT_GET_MESSAGE_INTERFACE_VERSION,
            &msg_interface_version);

      if (msg_interface_version >= 1)
      {
         struct retro_message_ext msg = {
            "Missing BIOS file (" A5200_BIOS_FILE_NAME
                  ") - using internal",
            2000,
            3,
            RETRO_LOG_ERROR,
            RETRO_MESSAGE_TARGET_ALL,
            RETRO_MESSAGE_TYPE_NOTIFICATION,
            -1
         };
         environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE_EXT, &msg);
      }
      else
      {
         struct retro_message msg = {
            "Missing BIOS file (" A5200_BIOS_FILE_NAME
                  ") - using internal",
            120
         };
         environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, &msg);
      }
   }
}

static void initialise_palette(void)
{
   uint8_t r, g, b;
   size_t i;

   for (i = 0; i < A5200_PALETTE_SIZE; i++)
   {
      r = (uint8_t)((a5200_palette_ntsc[i] & 0xFF0000) >> 16);
      g = (uint8_t)((a5200_palette_ntsc[i] & 0x00FF00) >>  8);
      b = (uint8_t)((a5200_palette_ntsc[i] & 0x0000FF)      );

      a5200_palette_rgb565[i] = (r >> 3) << 11 | (g >> 3) << 6 | (b >> 3);
   }
}

/* BIOS core option must be checked once
 * (and only once) before content is loaded */
static void check_bios_variable(void)
{
   struct retro_variable var = {0};

   /* BIOS */
   var.key                 = "a5200_bios";
   var.value               = NULL;
   a5200_use_official_bios = true;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) &&
       !string_is_empty(var.value) &&
       string_is_equal(var.value, "internal"))
      a5200_use_official_bios = false;
}

static void check_variables(void)
{
   struct retro_variable var = {0};
   enum frame_blend_method blend_method;

   /* Interframe Blending */
   var.key      = "a5200_mix_frames";
   var.value    = NULL;
   blend_method = FRAME_BLEND_NONE;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) &&
       !string_is_empty(var.value))
   {
      if (string_is_equal(var.value, "mix"))
         blend_method = FRAME_BLEND_MIX;
      else if (string_is_equal(var.value, "ghost_65"))
         blend_method = FRAME_BLEND_GHOST_65;
      else if (string_is_equal(var.value, "ghost_75"))
         blend_method = FRAME_BLEND_GHOST_75;
      else if (string_is_equal(var.value, "ghost_85"))
         blend_method = FRAME_BLEND_GHOST_85;
      else if (string_is_equal(var.value, "ghost_95"))
         blend_method = FRAME_BLEND_GHOST_95;
   }

   init_frame_blending(blend_method);

   /* Audio Filter */
   var.key                = "a5200_low_pass_filter";
   var.value              = NULL;
   audio_low_pass_enabled = false;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) &&
       !string_is_empty(var.value) &&
       string_is_equal(var.value, "enabled"))
      audio_low_pass_enabled = true;

   /* Audio Filter Level */
   var.key              = "a5200_low_pass_range";
   var.value            = NULL;
   audio_low_pass_range = (60 * 0x10000) / 100;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) &&
       !string_is_empty(var.value))
   {
      uint32_t filter_level = string_to_unsigned(var.value);
      audio_low_pass_range  = (filter_level * 0x10000) / 100;
   }

   /* Controller Hacks */
   var.key    = "a5200_input_hack";
   var.value  = NULL;
   input_hack = INPUT_HACK_NONE;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) &&
       !string_is_empty(var.value))
   {
      if (string_is_equal(var.value, "dual_stick"))
         input_hack = INPUT_HACK_DUAL_STICK;
      else if (string_is_equal(var.value, "swap_ports"))
         input_hack = INPUT_HACK_SWAP_PORTS;
   }

   /* Digital Joystick Sensitivity */
   var.key              = "a5200_digital_sensitivity";
   var.value            = NULL;
   joy_5200_digital_min = JOY_5200_MIN;
   joy_5200_digital_max = JOY_5200_MAX;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) &&
       !string_is_empty(var.value))
   {
      float range;

      if (string_is_equal(var.value, "auto"))
         range = cart_info.joy_digital_range;
      else
         range = (float)string_to_unsigned(var.value) / 100.0f;

      joy_5200_digital_min = JOY_5200_CENTER -
            (unsigned int)(((float)(JOY_5200_CENTER - JOY_5200_MIN) *
                  range) + 0.5f);
      joy_5200_digital_max = JOY_5200_CENTER +
            (unsigned int)(((float)(JOY_5200_MAX - JOY_5200_CENTER) *
                  range) + 0.5f);
   }

   /* Analog Joystick Sensitivity */
   var.key             = "a5200_analog_sensitivity";
   var.value           = NULL;
   joy_5200_analog_min = JOY_5200_MIN;
   joy_5200_analog_max = JOY_5200_MAX;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) &&
       !string_is_empty(var.value))
   {
      float range;

      if (string_is_equal(var.value, "auto"))
         range = cart_info.joy_analog_range;
      else
         range = (float)string_to_unsigned(var.value) / 100.0f;

      joy_5200_analog_min = JOY_5200_CENTER -
            (unsigned int)(((float)(JOY_5200_CENTER - JOY_5200_MIN) *
                  range) + 0.5f);
      joy_5200_analog_max = JOY_5200_CENTER +
            (unsigned int)(((float)(JOY_5200_MAX - JOY_5200_CENTER) *
                  range) + 0.5f);
   }

   /* Analog Joystick Response */
   var.key                = "a5200_analog_response";
   var.value              = NULL;
   input_analog_quadratic = false;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) &&
       !string_is_empty(var.value) &&
       string_is_equal(var.value, "enabled"))
      input_analog_quadratic = true;

   /* Analog Joystick Deadzone */
   var.key               = "a5200_analog_deadzone";
   var.value             = NULL;
   input_analog_deadzone = (int)(0.15f * (float)LIBRETRO_ANALOG_RANGE);

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) &&
       !string_is_empty(var.value))
   {
      unsigned deadzone     = string_to_unsigned(var.value);
      input_analog_deadzone = (int)((float)deadzone * 0.01f *
            (float)LIBRETRO_ANALOG_RANGE);
   }
}

static INLINE unsigned int a5200_get_analog_pot(int input)
{
   /* Convert to amplitude */
   float amplitude = (float)((input > input_analog_deadzone) ?
         (input - input_analog_deadzone) :
               (input + input_analog_deadzone)) /
                     (float)(LIBRETRO_ANALOG_RANGE - input_analog_deadzone);

   /* Check whether analog response is quadratic */
   if (input_analog_quadratic)
   {
      if (amplitude < 0.0)
         amplitude = -(amplitude * amplitude);
      else
         amplitude = amplitude * amplitude;
   }

   /* Map to Atari 5200 values */
   if (amplitude >= 0.0f)
      return JOY_5200_CENTER +
            (unsigned int)(((float)(joy_5200_analog_max - JOY_5200_CENTER) * amplitude) + 0.5f);

   return JOY_5200_CENTER -
         (unsigned int)(((float)(JOY_5200_CENTER - joy_5200_analog_min) * -amplitude) + 0.5f);
}

static unsigned a5200_get_analog_numpad_key(int input_x, int input_y)
{
   /* Right analog stick maps to the following
    * number pad keys:
    *    1 2 3
    *    4   6
    *    7 8 9 */
   double input_radius;
   double input_angle;
   unsigned numpad_index;

   /* Convert to amplitude */
   float amplitude_x = (float)((input_x > input_analog_deadzone) ?
         (input_x - input_analog_deadzone) :
               (input_x + input_analog_deadzone)) /
                     (float)(LIBRETRO_ANALOG_RANGE - input_analog_deadzone);
   float amplitude_y = (float)((input_y > input_analog_deadzone) ?
         (input_y - input_analog_deadzone) :
               (input_y + input_analog_deadzone)) /
                     (float)(LIBRETRO_ANALOG_RANGE - input_analog_deadzone);

   /* Convert to polar coordinates: part 1 */
   input_radius = sqrt((double)(amplitude_x * amplitude_x) +
         (double)(amplitude_y * amplitude_y));

   /* Check if radius is above threshold */
   if (input_radius > A5200_VIRTUAL_NUMPAD_THRESHOLD)
   {
      /* Convert to polar coordinates: part 2 */
      input_angle = atan2((double)amplitude_y, (double)amplitude_x);

      /* Adjust rotation offset... */
      input_angle = (2.0 * PI) - (input_angle + PI);
      input_angle = fmod(input_angle - (0.125 * PI), 2.0 * PI);
      if (input_angle < 0)
         input_angle += 2.0 * PI;

      /* Convert angle into numpad key index */
      numpad_index = (unsigned)((input_angle / (2.0 * PI)) * 8.0);
      /* Unnecessary safety check... */
      numpad_index = (numpad_index > 7) ? 7 : numpad_index;

      return input_analog_numpad_map[numpad_index];
   }

   return 0;
}

static void update_input(void)
{
   size_t pad_idx;

   key_code   = 0;
   key_shift  = 0;
   key_consol = CONSOL_NONE;

   /* pad_idx: Physical RetroPad
    * pad:     Emulated controller */
   for (pad_idx = 0; pad_idx < A5200_NUM_PADS; pad_idx++)
   {
      unsigned joypad_bits = 0;
      size_t pad           = (input_hack == INPUT_HACK_SWAP_PORTS) ?
            ((A5200_NUM_PADS - 1) - pad_idx) : pad_idx;
      int left_analog_x;
      int left_analog_y;
      int right_analog_x;
      int right_analog_y;

      /* Set a well-defined initial state */
      joy_5200_stick[pad]          = STICK_CENTRE;
      joy_5200_trig[pad]           = 1;
      atari_analog[pad]            = 0;
      joy_5200_pot[(pad << 1) + 0] = JOY_5200_CENTER;
      joy_5200_pot[(pad << 1) + 1] = JOY_5200_CENTER;

      /* Read input */
      if (libretro_supports_bitmasks)
         joypad_bits = input_state_cb(pad_idx,
               RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_MASK);
      else
      {
         size_t i;
         for (i = 0; i < (RETRO_DEVICE_ID_JOYPAD_R3+1); i++)
            joypad_bits |= input_state_cb(pad_idx,
                  RETRO_DEVICE_JOYPAD, 0, i) ? (1 << i) : 0;
      }

      /* D-Pad */
      if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_UP))
      {
         if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_LEFT))
            joy_5200_stick[pad] = STICK_UL;
         else if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_RIGHT))
            joy_5200_stick[pad] = STICK_UR;
         else
            joy_5200_stick[pad] = STICK_FORWARD;
      }
      else if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_DOWN))
      {
         if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_LEFT))
            joy_5200_stick[pad] = STICK_LL;
         else if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_RIGHT))
            joy_5200_stick[pad] = STICK_LR;
         else
            joy_5200_stick[pad] = STICK_BACK;
      }
      else if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_LEFT))
         joy_5200_stick[pad] = STICK_LEFT;
      else if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_RIGHT))
         joy_5200_stick[pad] = STICK_RIGHT;

      /* Analog joystick */
      left_analog_x = input_state_cb(pad_idx, RETRO_DEVICE_ANALOG,
            RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_X);
      left_analog_y = input_state_cb(pad_idx, RETRO_DEVICE_ANALOG,
            RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_Y);

      if ((left_analog_x < -input_analog_deadzone) ||
          (left_analog_x > input_analog_deadzone))
      {
         joy_5200_pot[(pad << 1) + 0] = a5200_get_analog_pot(left_analog_x);
         atari_analog[pad]            = 1;
      }

      if ((left_analog_y < -input_analog_deadzone) ||
          (left_analog_y > input_analog_deadzone))
      {
         joy_5200_pot[(pad << 1) + 1] = a5200_get_analog_pot(left_analog_y);
         atari_analog[pad]            = 1;
      }

      /* 1st trigger button */
      joy_5200_trig[pad] = joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_A) ? 0 : 1;

      /* Internal emulation code only supports
       * 2nd trigger/start/pause/number pad input
       * from player 1 (i.e. activating these
       * inputs generates an identical signal
       * from *all* connected controllers) */
      if (pad_idx > 0)
         break;

      /* 2nd trigger button */
      if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_B))
      {
         input_shift_ctrl ^= AKEY_SHFT;
         key_shift         = 1;
      }
      key_code += input_shift_ctrl ? 0x40 : 0x00;

      /* Start/pause */
      if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_SELECT))
         key_code += AKEY_5200_PAUSE;
      else if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_START))
         key_code += AKEY_5200_START;
      /* Number pad */
      else if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_Y))
         key_code += AKEY_5200_ASTERISK;
      else if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_X))
         key_code += AKEY_5200_HASH;
      else if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_R))
         key_code += AKEY_5200_0;
      else if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_R3))
         key_code += AKEY_5200_5;
      else if (input_hack != INPUT_HACK_DUAL_STICK)
      {
         /* If dual stick hack is disabled, right analog
          * stick is used as a virtual number pad, with
          * directions mapping to the physical key layout */
         right_analog_x = input_state_cb(pad_idx, RETRO_DEVICE_ANALOG,
               RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_X);
         right_analog_y = input_state_cb(pad_idx, RETRO_DEVICE_ANALOG,
               RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_Y);

         if ((right_analog_x < -input_analog_deadzone) ||
             (right_analog_x > input_analog_deadzone)  ||
             (right_analog_y < -input_analog_deadzone) ||
             (right_analog_y > input_analog_deadzone))
            key_code += a5200_get_analog_numpad_key(right_analog_x, right_analog_y);
      }

      /* OSK toggle */
      if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_L))
      {
         if (!input_osk_toggle_lock)
            input_show_osk = true;

         input_osk_toggle_lock = true;
      }
      else
         input_osk_toggle_lock = false;

      /* Dual stick hack
       * > Maps player 2's joystick to the right
       *   analog stick of player 1's RetroPad
       * > Note that we can safely use pad_idx
       *   when indexing arrays here, since
       *   port swapping cannot be enabled when
       *   dual stick hack is active */
      if (input_hack == INPUT_HACK_DUAL_STICK)
      {
         right_analog_x = input_state_cb(pad_idx, RETRO_DEVICE_ANALOG,
               RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_X);
         right_analog_y = input_state_cb(pad_idx, RETRO_DEVICE_ANALOG,
               RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_Y);

         if ((right_analog_x < -input_analog_deadzone) ||
             (right_analog_x > input_analog_deadzone))
         {
            joy_5200_pot[((pad_idx + 1) << 1) + 0] = a5200_get_analog_pot(right_analog_x);
            atari_analog[pad_idx + 1]              = 1;
         }

         if ((right_analog_y < -input_analog_deadzone) ||
             (right_analog_y > input_analog_deadzone))
         {
            joy_5200_pot[((pad_idx + 1) << 1) + 1] = a5200_get_analog_pot(right_analog_y);
            atari_analog[pad_idx + 1]              = 1;
         }

         /* When dual stick hack is enabled, all
          * other player 2 input is disabled */
         joy_5200_stick[pad_idx + 1] = STICK_CENTRE;
         joy_5200_trig[pad_idx + 1]  = 1;
         break;
      }
   }
}

static void update_input_osk(void)
{
   size_t pad_idx;
   unsigned joypad_bits = 0;

   key_code   = 0;
   key_shift  = 0;
   key_consol = CONSOL_NONE;

   /* Disable input from all emulated controllers */
   for (pad_idx = 0; pad_idx < A5200_NUM_PADS; pad_idx++)
   {
      joy_5200_stick[pad_idx]          = STICK_CENTRE;
      joy_5200_trig[pad_idx]           = 1;
      atari_analog[pad_idx]            = 0;
      joy_5200_pot[(pad_idx << 1) + 0] = JOY_5200_CENTER;
      joy_5200_pot[(pad_idx << 1) + 1] = JOY_5200_CENTER;
   }

   /* Read input from RetroPad 0 */
   if (libretro_supports_bitmasks)
      joypad_bits = input_state_cb(0,
            RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_MASK);
   else
   {
      size_t i;
      for (i = 0; i < (RETRO_DEVICE_ID_JOYPAD_R3+1); i++)
         joypad_bits |= input_state_cb(0,
               RETRO_DEVICE_JOYPAD, 0, i) ? (1 << i) : 0;
   }

   /* Handle key press */
   if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_A))
      key_code += a5200_osk_get_key();

   /* Move cursor */
   if ((joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_LEFT)) ||
       (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_RIGHT)))
   {
      if (input_osk_cursor_latch > 0)
         input_osk_cursor_latch--;

      if (input_osk_cursor_latch == 0)
      {
         a5200_osk_move_cursor(joypad_bits &
               (1 << RETRO_DEVICE_ID_JOYPAD_LEFT) ?
                     -1 : 1);
         input_osk_cursor_latch = A5200_OSK_CURSOR_REPEAT_DELAY;
      }
   }
   else
      input_osk_cursor_latch = 0;

   /* OSK toggle */
   if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_L))
   {
      if (!input_osk_toggle_lock)
         input_show_osk = false;

      input_osk_toggle_lock = true;
   }
   else
      input_osk_toggle_lock = false;
}

static void update_video(void)
{
   uint16_t *video_buffer_ptr = video_buffer;
   size_t x, y;

   for (y = 0; y < A5200_VIDEO_HEIGHT; y++)
   {
      uint8_t *screen_buffer_ptr = a5200_screen_buffer +
            ((y + A5200_VIDEO_OFFSET_Y) * A5200_SCREEN_BUFFER_WIDTH) +
            A5200_VIDEO_OFFSET_X;

      for (x = 0; x < A5200_VIDEO_WIDTH; x++)
         *(video_buffer_ptr++) = a5200_palette_rgb565[*(screen_buffer_ptr + x)];
   }

   if (blend_frames)
      blend_frames();

   if (input_show_osk)
      a5200_osk_draw(video_buffer, A5200_VIDEO_WIDTH,
            A5200_VIDEO_HEIGHT);

   video_cb(video_buffer, A5200_VIDEO_WIDTH, A5200_VIDEO_HEIGHT,
         A5200_VIDEO_WIDTH << 1);
}

static void update_audio(void)
{
   uint8_t *samples_ptr = audio_samples_buffer;
   int16_t *out_ptr   = audio_out_buffer;
   size_t i;

   Pokey_process(samples_ptr, A5200_AUDIO_BUFFER_SIZE);

   if (audio_low_pass_enabled)
   {
      /* Restore previous sample */
      int32_t low_pass = audio_low_pass_prev;
      /* Single-pole low-pass filter (6 dB/octave) */
      int32_t factor_a = audio_low_pass_range;
      int32_t factor_b = 0x10000 - factor_a;

      for(i = 0; i < A5200_AUDIO_BUFFER_SIZE; i++)
      {
         /* Convert 8u mono to 16s stereo */
         int16_t sample_16 = (int16_t)((*(samples_ptr++) - 128) << 8);

         /* Apply low-pass filter */
         low_pass = (low_pass * factor_a) + (sample_16 * factor_b);

         /* 16.16 fixed point */
         low_pass >>= 16;

         /* Update output buffer */
         *(out_ptr++) = (int16_t)low_pass;
         *(out_ptr++) = (int16_t)low_pass;
      }

      /* Save last sample for next frame */
      audio_low_pass_prev = low_pass;
   }
   else
   {
      for(i = 0; i < A5200_AUDIO_BUFFER_SIZE; i++)
      {
         /* Convert 8u mono to 16s stereo */
         int16_t sample_16 = (int16_t)((*(samples_ptr++) - 128) << 8);

         /* Update output buffer */
         *(out_ptr++) = sample_16;
         *(out_ptr++) = sample_16;
      }
   }

   audio_batch_cb(audio_out_buffer, A5200_AUDIO_BUFFER_SIZE);
}

/************************************
 * libretro implementation
 ************************************/

void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t cb) { audio_cb = cb; }
void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }

void retro_set_environment(retro_environment_t cb)
{
   bool option_cats_supported;
   struct retro_vfs_interface_info vfs_iface_info;
   static const struct retro_system_content_info_override content_overrides[] = {
      {
         "a52|bin", /* extensions */
         false,     /* need_fullpath */
         true       /* persistent_data */
      },
      { NULL, false, false }
   };

   environ_cb = cb;

   /* Initialise core options */
   libretro_set_core_options(environ_cb, &option_cats_supported);

   /* Initialise VFS */
   vfs_iface_info.required_interface_version = 1;
   vfs_iface_info.iface                      = NULL;
   if (environ_cb(RETRO_ENVIRONMENT_GET_VFS_INTERFACE, &vfs_iface_info))
      filestream_vfs_init(&vfs_iface_info);

   /* Request a persistent content data buffer */
   environ_cb(RETRO_ENVIRONMENT_SET_CONTENT_INFO_OVERRIDE,
         (void*)content_overrides);
}

void retro_get_system_info(struct retro_system_info *info)
{
   memset(info, 0, sizeof(*info));
   info->library_name = "a5200";
#ifndef GIT_VERSION
#define GIT_VERSION ""
#endif
   info->library_version = "2.0.2 " GIT_VERSION;
   info->need_fullpath = false;
   info->valid_extensions = "a52|bin";
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   memset(info, 0, sizeof(*info));
   info->timing.fps            = (double)A5200_FPS;
   info->timing.sample_rate    = (double)SOUND_SAMPLE_RATE;
   info->geometry.base_width   = A5200_VIDEO_WIDTH;
   info->geometry.base_height  = A5200_VIDEO_HEIGHT;
   info->geometry.max_width    = A5200_VIDEO_WIDTH;
   info->geometry.max_height   = A5200_VIDEO_HEIGHT;
   info->geometry.aspect_ratio = 4.0f / 3.0f;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
   (void)port;
   (void)device;
}

size_t retro_serialize_size(void)
{
   return A5200_SAVE_STATE_SIZE;
}

bool retro_serialize(void *data, size_t size)
{
   return SaveAtariState(data, size, 0);
}

bool retro_unserialize(const void *data, size_t size)
{
   return ReadAtariState(data, size);
}

void retro_cheat_reset(void)
{}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
   (void)index;
   (void)enabled;
   (void)code;
}

bool retro_load_game(const struct retro_game_info *info)
{
   const struct retro_game_info_ext *info_ext = NULL;
   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;

   /* a5200 requires a persistent ROM data buffer */
   rom_buf  = NULL;
   rom_data = NULL;
   rom_size = 0;

   if (environ_cb(RETRO_ENVIRONMENT_GET_GAME_INFO_EXT, &info_ext) &&
       info_ext->persistent_data)
   {
      rom_data = (const uint8_t*)info_ext->data;
      rom_size = info_ext->size;
   }

   /* If frontend does not support persistent
    * content data, must create a copy */
   if (!rom_data)
   {
      if (!info)
         goto error;

      rom_size = info->size;
      rom_buf  = (uint8_t*)malloc(rom_size);

      if (!rom_buf)
      {
         a5200_log(RETRO_LOG_INFO, "Failed to allocate ROM buffer.\n");
         goto error;
      }

      memcpy(rom_buf, (const uint8_t*)info->data, rom_size);
      rom_data = (const uint8_t*)rom_buf;
   }

   /* Set colour depth */
   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   {
      a5200_log(RETRO_LOG_INFO, "RGB565 is not supported.\n");
      goto error;
   }

   /* Set input descriptors */
   environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS,
         input_descriptors);

   /* Load bios */
   check_bios_variable();
   load_bios();

   /* Load game */
   if (!Atari800_OpenFile(rom_data, rom_size))
   {
      a5200_log(RETRO_LOG_INFO, "Failed to load content: %s\n", info->path);
      goto error;
   }

   Atari800_Initialise();

   /* Apply initial core options */
   check_variables();

   return true;

error:
   if (rom_buf)
      free(rom_buf);
   rom_buf = NULL;
   return false;
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info)
{
   (void)game_type;
   (void)info;
   (void)num_info;
   return false;
}

void retro_unload_game(void)
{
   CART_Remove();
   Atari800_Exit();

   if (rom_buf)
      free(rom_buf);

   rom_buf               = NULL;
   rom_data              = NULL;
   rom_size              = 0;
}

unsigned retro_get_region(void)
{
   return RETRO_REGION_NTSC;
}

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

void *retro_get_memory_data(unsigned id)
{
   return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
   return 0;
}

void retro_init(void)
{
   struct retro_log_callback log;

   if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
      log_cb = log.log;
   else
      log_cb = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_INPUT_BITMASKS, NULL))
      libretro_supports_bitmasks = true;

   a5200_screen_buffer = (uint8_t*)malloc(A5200_SCREEN_BUFFER_WIDTH *
         A5200_SCREEN_BUFFER_HEIGHT * sizeof(uint8_t));
#ifdef _3DS
   video_buffer = (uint16_t*)linearMemAlign(A5200_VIDEO_WIDTH *
         A5200_VIDEO_HEIGHT * sizeof(uint16_t), 128);
#else
   video_buffer = (uint16_t*)malloc(A5200_VIDEO_WIDTH *
         A5200_VIDEO_HEIGHT * sizeof(uint16_t));
#endif

   memset(a5200_screen_buffer, 0, A5200_SCREEN_BUFFER_WIDTH *
         A5200_SCREEN_BUFFER_HEIGHT * sizeof(uint8_t));
   memset(video_buffer, 0, A5200_VIDEO_WIDTH *
         A5200_VIDEO_HEIGHT * sizeof(uint16_t));

   /* Mono */
   audio_samples_buffer = (uint8_t*)malloc(A5200_AUDIO_BUFFER_SIZE *
         sizeof(uint8_t));
   /* Stereo */
   audio_out_buffer     = (int16_t*)malloc((A5200_AUDIO_BUFFER_SIZE << 1) *
         sizeof(int16_t));

   input_shift_ctrl        = 0;
   input_hack              = INPUT_HACK_NONE;
   input_analog_quadratic  = false;
   input_show_osk          = false;
   input_osk_toggle_lock   = false;
   input_osk_cursor_latch  = 0;
   audio_low_pass_prev     = 0;
   a5200_use_official_bios = true;

   initialise_palette();
   a5200_osk_init();
}

void retro_deinit(void)
{
   libretro_supports_bitmasks = false;
   input_shift_ctrl           = 0;
   input_hack                 = INPUT_HACK_NONE;
   input_analog_quadratic     = false;
   input_show_osk             = false;
   input_osk_toggle_lock      = false;
   input_osk_cursor_latch     = 0;
   audio_low_pass_prev        = 0;
   a5200_use_official_bios    = true;

   if (a5200_screen_buffer)
   {
      free(a5200_screen_buffer);
      a5200_screen_buffer = NULL;
   }

   if (video_buffer)
   {
#ifdef _3DS
      linearFree(video_buffer);
#else
      free(video_buffer);
#endif
      video_buffer = NULL;
   }

   if (video_buffer_prev)
   {
      free(video_buffer_prev);
      video_buffer_prev = NULL;
   }

   if (audio_samples_buffer)
   {
      free(audio_samples_buffer);
      audio_samples_buffer = NULL;
   }

   if (audio_out_buffer)
   {
      free(audio_out_buffer);
      audio_out_buffer = NULL;
   }

   a5200_osk_deinit();
}

void retro_reset(void)
{
   Warmstart();
}

void retro_run(void)
{
   bool options_updated = false;

   /* Core options */
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &options_updated) &&
       options_updated)
      check_variables();

   /* Update input */
   input_poll_cb();
   if (input_show_osk)
      update_input_osk();
   else
      update_input();

   /* Run emulator */
   Atari800_Frame();

   /* Output video */
   update_video();

   /* Output audio */
   update_audio();
}

#ifdef WRC
void em_cmd_savefiles() {}
void wrc_on_set_options(int opts) {}

const chd_header *chd_get_header(chd_file *chd) { return 0; }
chd_error chd_get_metadata(chd_file *chd, UINT32 searchtag, UINT32 searchindex, void *output, UINT32 outputlen, UINT32 *resultlen, UINT32 *resulttag, UINT8 *resultflags) { return 0; }
chd_error chd_open(const char *filename, int mode, chd_file *parent, chd_file **chd) { return 0; }
void chd_close(chd_file *chd) {}
chd_error chd_read(chd_file *chd, UINT32 hunknum, void *buffer) { return 0; }

#define SAMPLE_RATE 48000
#define FPS 60
#define SAMPLES (SAMPLE_RATE / FPS)
#define CHUNKSIZE 2048

static unsigned char* audio_samples = NULL;
static unsigned char vidBuf[512 * 512];

//a5200_screen_buffer

int wrc_start(char* arg) {
   char* filename = arg;
   unsigned long filesize, done = 0;
   unsigned char* buffer;

   int bios_read = 0;
   RFILE* bios_file = NULL;
   int64_t bytes_read = 0;

   bios_file = filestream_open("/bios.bin", RETRO_VFS_FILE_ACCESS_READ,
                              RETRO_VFS_FILE_ACCESS_HINT_NONE);
   if (bios_file) {
      bytes_read = filestream_read(bios_file, atari_os, A5200_BIOS_SIZE);
      filestream_close(bios_file);
      bios_read = (bytes_read == A5200_BIOS_SIZE);
   }

   if(!bios_read) {
      memcpy(atari_os, ROM_altirra_5200_os, A5200_BIOS_SIZE);
   }

   /* Open file */
   FILE* fp = fopen(filename, "rb");
   if (!fp) {
         return 0;
   }

   /* Get file size */
   fseek(fp, 0, SEEK_END);
   filesize = ftell(fp);
   fseek(fp, 0, SEEK_SET);

   /* allocate buffer */
   buffer = (unsigned char*)malloc(filesize);
   if (!buffer) {
         fclose(fp);
         return 0;
   }

   /* Read into buffer (2k blocks) */
   while (filesize > CHUNKSIZE) {
         fread(buffer + done, CHUNKSIZE, 1, fp);
         done += CHUNKSIZE;
         filesize -= CHUNKSIZE;
   }

   /* Read remaining bytes */
   fread(buffer + done, filesize, 1, fp);
   done += filesize;

   /* Close file */
   fclose(fp);

   // load card game if ok
   if (Atari800_OpenFile(buffer, done) == AFILE_ERROR) {
         return 0;
   }

   a5200_screen_buffer = vidBuf;

   Atari800_Initialise();
   audio_samples = malloc(SAMPLES);

   printf("Init succeeded.\n");

   return 1;
   }

#define JST_UP 0x0100
#define JST_RIGHT 0x0200
#define JST_DOWN 0x0400
#define JST_LEFT 0x0800
#define JST_FIRER 0x0040
#define JST_FIREL 0x4000
#define JST_0 0x0001
#define JST_1 0x0002
#define JST_2 0x0003
#define JST_3 0x0004
#define JST_4 0x0005
#define JST_5 0x0006
#define JST_6 0x0007
#define JST_7 0x0008
#define JST_8 0x0009
#define JST_9 0x000A
#define JST_STAR 0x000B
#define JST_POUND 0x000C
#define JST_START 0x000D
#define JST_PAUSE 0x000E
#define JST_RESET 0x000F

int wrcInput[2] = {0, 0};
float wrcAnalog[2][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}};

static void wrc_update_input()
{
   size_t pad_idx;

   key_code   = 0;
   key_shift  = 0;
   key_consol = CONSOL_NONE;

   /* pad_idx: Physical RetroPad
    * pad:     Emulated controller */
   for (pad_idx = 0; pad_idx < A5200_NUM_PADS; pad_idx++)
   {
      unsigned joypad_bits = 0;
      size_t pad           = (input_hack == INPUT_HACK_SWAP_PORTS) ?
            ((A5200_NUM_PADS - 1) - pad_idx) : pad_idx;
      int left_analog_x;
      int left_analog_y;
      int right_analog_x;
      int right_analog_y;

      /* Set a well-defined initial state */
      joy_5200_stick[pad]          = STICK_CENTRE;
      joy_5200_trig[pad]           = 1;
      atari_analog[pad]            = 0;
      joy_5200_pot[(pad << 1) + 0] = JOY_5200_CENTER;
      joy_5200_pot[(pad << 1) + 1] = JOY_5200_CENTER;

      /* Read input */
      // if (libretro_supports_bitmasks)
      //    joypad_bits = input_state_cb(pad_idx,
      //          RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_MASK);
      // else
      // {
      //    size_t i;
      //    for (i = 0; i < (RETRO_DEVICE_ID_JOYPAD_R3+1); i++)
      //       joypad_bits |= input_state_cb(pad_idx,
      //             RETRO_DEVICE_JOYPAD, 0, i) ? (1 << i) : 0;
      // }

      joypad_bits = wrcInput[pad_idx];

      /* D-Pad */
      if (joypad_bits & JST_UP)
      {
         if (joypad_bits & JST_LEFT)
            joy_5200_stick[pad] = STICK_UL;
         else if (joypad_bits & JST_RIGHT)
            joy_5200_stick[pad] = STICK_UR;
         else
            joy_5200_stick[pad] = STICK_FORWARD;
      }
      else if (joypad_bits & JST_DOWN)
      {
         if (joypad_bits & JST_LEFT)
            joy_5200_stick[pad] = STICK_LL;
         else if (joypad_bits & JST_RIGHT)
            joy_5200_stick[pad] = STICK_LR;
         else
            joy_5200_stick[pad] = STICK_BACK;
      }
      else if (joypad_bits & JST_LEFT)
         joy_5200_stick[pad] = STICK_LEFT;
      else if (joypad_bits & JST_RIGHT)
         joy_5200_stick[pad] = STICK_RIGHT;

      /* Analog joystick */
      // left_analog_x = input_state_cb(pad_idx, RETRO_DEVICE_ANALOG,
      //       RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_X);
      // left_analog_y = input_state_cb(pad_idx, RETRO_DEVICE_ANALOG,
      //       RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_Y);
      left_analog_x = wrcAnalog[pad_idx][0];
      left_analog_y = wrcAnalog[pad_idx][1];

      if ((left_analog_x < -input_analog_deadzone) ||
          (left_analog_x > input_analog_deadzone))
      {
         joy_5200_pot[(pad << 1) + 0] = a5200_get_analog_pot(left_analog_x);
         atari_analog[pad]            = 1;
      }

      if ((left_analog_y < -input_analog_deadzone) ||
          (left_analog_y > input_analog_deadzone))
      {
         joy_5200_pot[(pad << 1) + 1] = a5200_get_analog_pot(left_analog_y);
         atari_analog[pad]            = 1;
      }

      /* 1st trigger button */
      joy_5200_trig[pad] = joypad_bits & JST_FIRER ? 0 : 1;

      /* Internal emulation code only supports
       * 2nd trigger/start/pause/number pad input
       * from player 1 (i.e. activating these
       * inputs generates an identical signal
       * from *all* connected controllers) */
      if (pad_idx > 0)
         break;

      /* 2nd trigger button */
      if (joypad_bits & JST_FIREL)
      {
         input_shift_ctrl ^= AKEY_SHFT;
         key_shift         = 1;
      }
      key_code += input_shift_ctrl ? 0x40 : 0x00;

      switch (joypad_bits & 0x000F) {
         case JST_START:
            key_code += AKEY_5200_START;
            break;
         case JST_RESET:
            key_code += AKEY_5200_RESET;
            break;
         case JST_PAUSE:
            key_code += AKEY_5200_PAUSE;
            break;
         case JST_1:
            key_code += AKEY_5200_1;
            break;
         case JST_2:
            key_code += AKEY_5200_2;
            break;
         case JST_3:
            key_code += AKEY_5200_3;
            break;
         case JST_4:
            key_code += AKEY_5200_4;
            break;
         case JST_5:
            key_code += AKEY_5200_5;
            break;
         case JST_6:
            key_code += AKEY_5200_6;
            break;
         case JST_7:
            key_code += AKEY_5200_7;
            break;
         case JST_8:
            key_code += AKEY_5200_8;
            break;
         case JST_9:
            key_code += AKEY_5200_9;
            break;
         case JST_0:
            key_code += AKEY_5200_0;
            break;
         case JST_STAR:
            key_code += AKEY_5200_ASTERISK;
            break;
         case JST_POUND:
            key_code += AKEY_5200_HASH;
            break;
      }

      /* Start/pause */
      // if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_SELECT))
      //    key_code += AKEY_5200_PAUSE;
      // else if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_START))
      //    key_code += AKEY_5200_START;
      // /* Number pad */
      // else if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_Y))
      //    key_code += AKEY_5200_ASTERISK;
      // else if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_X))
      //    key_code += AKEY_5200_HASH;
      // else if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_R))
      //    key_code += AKEY_5200_0;
      // else if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_R3))
      //    key_code += AKEY_5200_5;
      // else if (input_hack != INPUT_HACK_DUAL_STICK)
      // {
      //    /* If dual stick hack is disabled, right analog
      //     * stick is used as a virtual number pad, with
      //     * directions mapping to the physical key layout */
      //    right_analog_x = input_state_cb(pad_idx, RETRO_DEVICE_ANALOG,
      //          RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_X);
      //    right_analog_y = input_state_cb(pad_idx, RETRO_DEVICE_ANALOG,
      //          RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_Y);

      //    if ((right_analog_x < -input_analog_deadzone) ||
      //        (right_analog_x > input_analog_deadzone)  ||
      //        (right_analog_y < -input_analog_deadzone) ||
      //        (right_analog_y > input_analog_deadzone))
      //       key_code += a5200_get_analog_numpad_key(right_analog_x, right_analog_y);
      // }

      /* OSK toggle */
      // if (joypad_bits & (1 << RETRO_DEVICE_ID_JOYPAD_L))
      // {
      //    if (!input_osk_toggle_lock)
      //       input_show_osk = true;

      //    input_osk_toggle_lock = true;
      // }
      // else
      //    input_osk_toggle_lock = false;

      /* Dual stick hack
       * > Maps player 2's joystick to the right
       *   analog stick of player 1's RetroPad
       * > Note that we can safely use pad_idx
       *   when indexing arrays here, since
       *   port swapping cannot be enabled when
       *   dual stick hack is active */
      if (input_hack == INPUT_HACK_DUAL_STICK)
      {
         // right_analog_x = input_state_cb(pad_idx, RETRO_DEVICE_ANALOG,
         //       RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_X);
         // right_analog_y = input_state_cb(pad_idx, RETRO_DEVICE_ANALOG,
         //       RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_Y);
         right_analog_x = wrcAnalog[pad_idx][2];
         right_analog_y = wrcAnalog[pad_idx][3];


         if ((right_analog_x < -input_analog_deadzone) ||
             (right_analog_x > input_analog_deadzone))
         {
            joy_5200_pot[((pad_idx + 1) << 1) + 0] = a5200_get_analog_pot(right_analog_x);
            atari_analog[pad_idx + 1]              = 1;
         }

         if ((right_analog_y < -input_analog_deadzone) ||
             (right_analog_y > input_analog_deadzone))
         {
            joy_5200_pot[((pad_idx + 1) << 1) + 1] = a5200_get_analog_pot(right_analog_y);
            atari_analog[pad_idx + 1]              = 1;
         }

         /* When dual stick hack is enabled, all
          * other player 2 input is disabled */
         joy_5200_stick[pad_idx + 1] = STICK_CENTRE;
         joy_5200_trig[pad_idx + 1]  = 1;
         break;
      }
   }
}

void wrc_step() {
    for (int i = 0; i < 2; i++) {
      wrcInput[i] = EM_ASM_INT({return window.emulator.getInput($0)}, i);
      wrcAnalog[i][0] = EM_ASM_DOUBLE({return window.emulator.getAnalog($0, 0, true)}, i);
      wrcAnalog[i][1] = EM_ASM_DOUBLE({return window.emulator.getAnalog($0, 0, false)}, i);
      wrcAnalog[i][2] = EM_ASM_DOUBLE({return window.emulator.getAnalog($0, 1, true)}, i);
      wrcAnalog[i][3] = EM_ASM_DOUBLE({return window.emulator.getAnalog($0, 1, false)}, i);

      for (int x = 0; x < 4; x++) {
         wrcAnalog[i][x] *= LIBRETRO_ANALOG_RANGE;
         if (wrcAnalog[i][x] > LIBRETRO_ANALOG_RANGE) {
            wrcAnalog[i][x] = LIBRETRO_ANALOG_RANGE;
         } else if ((wrcAnalog[i][x] < -LIBRETRO_ANALOG_RANGE)) {
            wrcAnalog[i][x] = -LIBRETRO_ANALOG_RANGE;
         }
      }
    }

   /* Digital Joystick Sensitivity */
   joy_5200_digital_min = JOY_5200_MIN;
   joy_5200_digital_max = JOY_5200_MAX;
   float range;
   range = cart_info.joy_digital_range;
   joy_5200_digital_min = JOY_5200_CENTER -
         (unsigned int)(((float)(JOY_5200_CENTER - JOY_5200_MIN) *
               range) + 0.5f);
   joy_5200_digital_max = JOY_5200_CENTER +
         (unsigned int)(((float)(JOY_5200_MAX - JOY_5200_CENTER) *
               range) + 0.5f);

   /* Analog Joystick Sensitivity */
   joy_5200_analog_min = JOY_5200_MIN;
   joy_5200_analog_max = JOY_5200_MAX;
   range = cart_info.joy_analog_range;
   joy_5200_analog_min = JOY_5200_CENTER -
         (unsigned int)(((float)(JOY_5200_CENTER - JOY_5200_MIN) *
               range) + 0.5f);
   joy_5200_analog_max = JOY_5200_CENTER +
         (unsigned int)(((float)(JOY_5200_MAX - JOY_5200_CENTER) *
               range) + 0.5f);
   input_analog_quadratic = false;
   input_analog_deadzone = (int)(0.15f * (float)LIBRETRO_ANALOG_RANGE);

    wrc_update_input();

    Atari800_Frame();

    int W = 320;
    int H = 240;

    EM_ASM({ window.emulator.drawScreen($0, $1, $2); }, vidBuf, W, H);

    Pokey_process(audio_samples, SAMPLES);

    EM_ASM({ window.emulator.audioCallback($0, $1); }, audio_samples, SAMPLES);
}

void wrc_save_state(char* file) {
    unsigned char* state;
    state = (unsigned char*)malloc(A5200_SAVE_STATE_SIZE * sizeof(unsigned char));

    SaveAtariState(state, A5200_SAVE_STATE_SIZE, 0);

    FILE* f = fopen(file, "wb");
    fwrite(state, sizeof(unsigned char), A5200_SAVE_STATE_SIZE, f);
    fclose(f);

    free(state);
}

void wrc_load_state(char* file) {
    FILE* f = fopen(file, "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        unsigned len = ftell(f);
        rewind(f);

        unsigned char* state = (unsigned char*)malloc(len * sizeof(unsigned char));
        fread(state, len, 1, f); // Read in the entire file
        fclose(f);

        ReadAtariState(state, len);
        free(state);
    }
}

#endif



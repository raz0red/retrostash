/*
    This file is part of Flycast.

    Flycast is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Flycast is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Flycast.  If not, see <https://www.gnu.org/licenses/>.
 */
// FLY_RELEASE_BUILD: public-release artifact switch (build-baked via
// -DFLY_RELEASE_BUILD=1). HUD becomes opt-in (?hud=1) and the hudlog POST
// machinery is compiled out. Default 0 = dev/prod behavior unchanged.
#ifndef FLY_RELEASE_BUILD
#define FLY_RELEASE_BUILD 0
#endif

#include <cstdio>
#include <cstdarg>
#include <math.h>
#include "types.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include "rec-wasm/fly_instrument.h"
#endif
#ifndef _WIN32
#include <sys/time.h>
#endif
#include <mutex>

#ifdef __SWITCH__
#include <stdlib.h>
#include <string.h>
#include "nswitch.h"
#elif defined(__linux__) || defined(__FreeBSD__)
#include <stdlib.h>
#endif

#include <sys/stat.h>
#include <file/file_path.h>

#include <libretro.h>

#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES)
#include <glsm/glsm.h>
#include "wsi/gl_context.h"
#endif
#ifdef HAVE_VULKAN
#include "rend/vulkan/vulkan_context.h"
#include <libretro_vulkan.h>
#endif
#ifdef HAVE_D3D11
#include <libretro_d3d.h>
#include "rend/dx11/dx11context_lr.h"
#endif
#include "emulator.h"
#include "hw/sh4/sh4_mem.h"
#include "hw/sh4/sh4_sched.h"
#include "hw/sh4/sh4_core.h"  // SH4ThrownException for escape probe
#include "keyboard_map.h"
#include "hw/maple/maple_cfg.h"
#include "hw/maple/maple_if.h"
#include "hw/pvr/pvr_regs.h"
#include "hw/pvr/Renderer_if.h"
#include "hw/naomi/naomi_cart.h"
#include "hw/naomi/card_reader.h"
#include "LogManager.h"
#include "cheats.h"
#include "rend/osd.h"
#include "cfg/option.h"
#include "version.h"
#include "oslib/oslib.h"
#include "rend/CustomTexture.h"
#include "oslib/i18n.h"
#include "input/dreampotato.h"

constexpr char slash = path_default_slash_c();

#if defined(__EMSCRIPTEN__)
// WRC (2026-09-08): live-apply bridge for JS-side pause-menu settings that
// need to change while the core is already running, no reload required -
// same mechanism webrcade-app-beetle-psx uses for disk eject/analog mode/
// etc (see frontend/drivers/platform_emscripten.c's wrc_set_options(),
// which stores the bitmask into wrc_options and calls this extern hook).
// OPT1 is a pure "something changed, go read it" signal, not a value
// encoding - reicast_frame_skipping accepts 0-6 (see
// libretro_core_options.h), too wide a range to pack into a couple of
// wrc_options bits alongside whatever else might use that bitmask later.
// Setting OPT1 just flips a pending flag; retro_run() below (called every
// frame) checks it and, only when set, calls back into JS via EM_ASM_INT
// to fetch the actual current value and assign it straight into
// config::SkipFrame (an Option<int>, checked live by the renderer
// wherever frame skipping is consulted - no restart/re-init needed,
// unlike the .opt file which is only read once at core boot).
static bool wrc_frameskip_pending = false;

extern "C" void wrc_on_set_options(int opts) {
	if (opts & 1)
		wrc_frameskip_pending = true;
}
#endif

#define RETRO_DEVICE_TWINSTICK				RETRO_DEVICE_SUBCLASS( RETRO_DEVICE_JOYPAD, 1 )
#define RETRO_DEVICE_TWINSTICK_SATURN		RETRO_DEVICE_SUBCLASS( RETRO_DEVICE_JOYPAD, 2 )
#define RETRO_DEVICE_ASCIISTICK				RETRO_DEVICE_SUBCLASS( RETRO_DEVICE_JOYPAD, 3 )
#define RETRO_DEVICE_MARACAS				RETRO_DEVICE_SUBCLASS( RETRO_DEVICE_JOYPAD, 4 )
#define RETRO_DEVICE_FISHING				RETRO_DEVICE_SUBCLASS( RETRO_DEVICE_JOYPAD, 5 )
#define RETRO_DEVICE_POPNMUSIC				RETRO_DEVICE_SUBCLASS( RETRO_DEVICE_JOYPAD, 6 )
#define RETRO_DEVICE_RACING					RETRO_DEVICE_SUBCLASS( RETRO_DEVICE_JOYPAD, 7 )
#define RETRO_DEVICE_DENSHA					RETRO_DEVICE_SUBCLASS( RETRO_DEVICE_JOYPAD, 8 )
#define RETRO_DEVICE_FULL_CONTROLLER		RETRO_DEVICE_SUBCLASS( RETRO_DEVICE_JOYPAD, 9 )

#define RETRO_ENVIRONMENT_RETROARCH_START_BLOCK 0x800000

#define RETRO_ENVIRONMENT_SET_SAVE_STATE_IN_BACKGROUND (2 | RETRO_ENVIRONMENT_RETROARCH_START_BLOCK)
                                            /* bool * --
                                            * Boolean value that tells the front end to save states in the
                                            * background or not.
                                            */

#define RETRO_ENVIRONMENT_POLL_TYPE_OVERRIDE (4 | RETRO_ENVIRONMENT_RETROARCH_START_BLOCK)
                                            /* unsigned * --
                                            * Tells the frontend to override the poll type behavior. 
                                            * Allows the frontend to influence the polling behavior of the
                                            * frontend.
                                            *
                                            * Will be unset when retro_unload_game is called.
                                            *
                                            * 0 - Don't Care, no changes, frontend still determines polling type behavior.
                                            * 1 - Early
                                            * 2 - Normal
                                            * 3 - Late
                                            */

#include "libretro_core_option_defines.h"
#include "libretro_core_options.h"
#include "vmu_xhair.h"

extern void retro_audio_init(void);
extern void retro_audio_deinit(void);
extern void retro_audio_flush_buffer(void);
extern void retro_audio_upload(void);
extern size_t retro_audio_buffer_fill(void);
extern size_t retro_audio_buffer_capacity(void);
// Audio-loss telemetry (audiostream.cpp) — HUD surfacing
extern u32 g_aud_overflow_events;
extern u32 g_aud_shortfall_frames;
extern u32 g_aud_produced_frames;

// Retro-frontend diagnostic logs (video_cb per-frame dupe tracking).
// Noisy — only useful for presentation-layer / BLANK-cluster investigation.
#ifndef FLY_RETRO_DIAG
#define FLY_RETRO_DIAG 0
#endif

#if defined(__EMSCRIPTEN__) && !defined(JIT_PROD_BUILD)
// Diagnostic hooks into rec_wasm.cpp + Renderer_if.cpp
extern u32 g_wasm_block_count;
extern "C" void fly_dump_pc_trace();
extern "C" u32 g_fly_rend_start_render_calls;
// Exception / RTE diagnostics (sh4_interrupts.cpp, sh4_opcodes.cpp)
extern "C" u32 g_fly_exc_total;
extern "C" u32 g_fly_exc_count[1024];
extern "C" u32 g_fly_exc_trace_epc[32];
extern "C" u32 g_fly_exc_trace_evn[32];
extern "C" u32 g_fly_exc_trace_head;
extern "C" u32 g_fly_rte_count;
// Correct BL-transition counters (sh4_core_regs.cpp)
extern "C" u32 g_fly_bl_clear_count;
extern "C" u32 g_fly_bl_set_count;
// First-dispatch ring dumper (rec_wasm.cpp)
extern "C" void fly_dump_new_pcs(u32 target_pc);
// First-exception PC-trace snapshot dumper (rec_wasm.cpp)
extern "C" void fly_dump_first_exc_trace();
// ASIC/Holly interrupt counters (holly_intc.cpp)
extern "C" u32 g_fly_asic_total;
extern "C" u32 g_fly_asic_vblank_in;
extern "C" u32 g_fly_asic_vblank_out;
extern "C" u32 g_fly_asic_hblank;
extern "C" u32 g_fly_asic_maple;
#endif

std::string arcadeFlashPath;

static bool devices_need_refresh = false;
static int device_type[4] = {-1,-1,-1,-1};
static int astick_deadzone = 0;
static int trigger_deadzone = 0;
static bool digital_triggers = false;
static bool allow_service_buttons = false;
static bool haveCardReader;

static bool libretro_supports_bitmasks = false;

static bool categoriesSupported = false;
static bool platformIsDreamcast = true;
static bool platformIsArcade = false;
static bool threadedRenderingEnabled = true;
static bool oitEnabled = false;
#if defined(HAVE_OIT) || defined(HAVE_VULKAN) || defined(HAVE_D3D11)
static bool perPixelChecked = false;
#endif
static bool autoSkipFrameEnabled = false;
#ifdef _OPENMP
static bool textureUpscaleEnabled = false;
#endif
static bool vmuScreenSettingsShown = true;
static bool lightgunSettingsShown = true;

u32 kcode[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
u16 rt[4];
u16 lt[4];
u16 lt2[4];
u16 rt2[4];
u32 vks[4];
s16 joyx[4], joyy[4];
s16 joyrx[4], joyry[4];
s16 joy3x[4], joy3y[4];
// Mouse buttons
// bit 0: Button C
// bit 1: Right button (B)
// bit 2: Left button (A)
// bit 3: Wheel button
u8 mo_buttons[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
// Relative mouse coordinates [-512:511]
float mo_x_delta[4];
float mo_y_delta[4];
float mo_wheel_delta[4];
std::mutex relPosMutex;
// Absolute mouse coordinates
// Range [0:639] [0:479]
// but may be outside this range if the pointer is offscreen or outside the 4:3 window.
s32 mo_x_abs[4];
s32 mo_y_abs[4];

static u32 vib_stop_time[4];
static double vib_strength[4];
static double vib_delta[4];

unsigned per_content_vmus = 0;

static bool first_run = true;
static bool rotate_screen;
static bool rotate_game;
static bool is_pal;
static int framebufferWidth;
static int framebufferHeight;
static int maxFramebufferWidth;
static int maxFramebufferHeight;
static float framebufferAspectRatio = 4.f / 3.f;
static double fps_current;

float libretro_expected_audio_samples_per_run;
unsigned libretro_vsync_swap_interval = 1;
bool libretro_detect_vsync_swap_interval = false;

static retro_perf_callback perf_cb;
static retro_get_cpu_features_t perf_get_cpu_features_cb;

// Callbacks
static retro_log_printf_t         log_cb;
static retro_video_refresh_t      video_cb;
static retro_input_poll_t         poll_cb;
static retro_input_state_t        input_cb;
retro_audio_sample_batch_t        audio_batch_cb;
retro_environment_t               environ_cb;

static retro_rumble_interface rumble;

static void refresh_devices(bool first_startup);
static void init_disk_control_interface();
static bool read_m3u(const char *file);
static void updateVibration(u32 port, float power, float inclination, u32 durationMs);

static std::string game_data;
static char g_base_name[128];
static char game_dir[1024];
char game_dir_no_slash[1024];
char vmu_dir_no_slash[PATH_MAX];
char content_name[PATH_MAX];
char g_roms_dir[PATH_MAX];
static std::mutex mtx_serialization;
static bool gl_ctx_resetting = false;
static bool is_dupe;
static u64 startTime;

// Disk swapping
static struct retro_disk_control_callback retro_disk_control_cb;
static struct retro_disk_control_ext_callback retro_disk_control_ext_cb;
static unsigned disk_initial_index = 0;
static std::string disk_initial_path;
static unsigned disk_index = 0;
static std::vector<std::string> disk_paths;
static std::vector<std::string> disk_labels;
static bool disc_tray_open = false;

static bool set_variable_visibility(void);

void retro_set_video_refresh(retro_video_refresh_t cb)
{
	video_cb = cb;
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
	// Nothing to do here
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
	audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
	poll_cb = cb;
}

#ifdef FLYCAST_WRC_INPUT
// WRC - webrcade bypasses RetroArch's normal input driver stack entirely:
// JS calls wrc_set_input() (frontend/drivers/platform_emscripten.c in
// retrostash/RetroArch) which writes straight into the globals below, and
// RetroArch's own retro_input_state_t callback would just report nothing
// pressed in this build. Rather than touching every call site in
// UpdateInputState() (getBitmask()/get_analog_stick()/get_analog_trigger()
// all funnel through the single input_cb function pointer already), this
// substitutes what input_cb itself points to, so the existing
// MDT_SegaController path works unmodified. wrc_input_state's bit layout is
// the standard RETRO_DEVICE_ID_JOYPAD_* positions (1<<id) - see
// webrcade-app-retro-flycast/src/emulator/index.js's JOYPAD_* constants,
// written deliberately to match this rather than inventing a one-off
// scheme (same convention as mupen64plus-libretro-nx's WRC glue).
extern "C" {
extern unsigned int wrc_input_state[];
extern float wrc_input_state_analog[][4];
}

static int16_t wrc_libretro_input_state(unsigned port, unsigned device, unsigned index, unsigned id)
{
	if (port >= 4)
		return 0;

	if (device == RETRO_DEVICE_JOYPAD)
	{
		if (id == RETRO_DEVICE_ID_JOYPAD_MASK)
			return (int16_t)(wrc_input_state[port] & 0xFFFF);
		return (wrc_input_state[port] & (1u << id)) ? 1 : 0;
	}

	if (device == RETRO_DEVICE_ANALOG)
	{
		// WRC (2026-09-11): real Dreamcast L/R triggers are analog on the
		// actual hardware - flycast's own controller code (get_analog_
		// trigger(), further down this file) already queries this exact
		// device/index/id combo for L2/R2 before falling back to a plain
		// digital read, so this was previously a silent dead end (fell
		// through to the `return 0` below, forcing every call back onto
		// the digital fallback). wrc_input_state_analog[port][2]/[3] are
		// repurposed here rather than plumbing new WASM-exported state:
		// this app has no second analog stick to put there (Dreamcast has
		// only one), so index 2/3 carry trigger pressure instead - the JS
		// side (pollControls() in webrcade-app-retro-flycast) sends it
		// through those same two argument slots. Digital LBUMP/RBUMP
		// (still sent too, for a keyboard or a pad without analog trigger
		// support) wins with the full value when pressed; otherwise fall
		// through to whatever analog pressure was sent.
		if (index == RETRO_DEVICE_INDEX_ANALOG_BUTTON)
		{
			if (id == RETRO_DEVICE_ID_JOYPAD_L2)
			{
				if (wrc_input_state[port] & (1u << RETRO_DEVICE_ID_JOYPAD_L2))
					return 32767;
				return (int16_t)(wrc_input_state_analog[port][2] * 32767.0f);
			}
			if (id == RETRO_DEVICE_ID_JOYPAD_R2)
			{
				if (wrc_input_state[port] & (1u << RETRO_DEVICE_ID_JOYPAD_R2))
					return 32767;
				return (int16_t)(wrc_input_state_analog[port][3] * 32767.0f);
			}
			return 0;
		}

		float v = 0.f;
		if (index == RETRO_DEVICE_INDEX_ANALOG_LEFT)
			v = id == RETRO_DEVICE_ID_ANALOG_X ? wrc_input_state_analog[port][0]
			  : id == RETRO_DEVICE_ID_ANALOG_Y ? wrc_input_state_analog[port][1] : 0.f;
		else if (index == RETRO_DEVICE_INDEX_ANALOG_RIGHT)
			v = id == RETRO_DEVICE_ID_ANALOG_X ? wrc_input_state_analog[port][2]
			  : id == RETRO_DEVICE_ID_ANALOG_Y ? wrc_input_state_analog[port][3] : 0.f;
		return (int16_t)(v * 32767.0f);
	}

	return 0;
}
#endif

void retro_set_input_state(retro_input_state_t cb)
{
#ifdef FLYCAST_WRC_INPUT
	(void)cb;
	input_cb = wrc_libretro_input_state;
#else
	input_cb = cb;
#endif
}

static void input_set_deadzone_stick(int percent)
{
	if (percent >= 0 && percent <= 100)
		astick_deadzone = (int)(percent * 0.01f * 0x8000);
}

static void input_set_deadzone_trigger(int percent)
{
	if (percent >= 0 && percent <= 100)
		trigger_deadzone = (int)(percent * 0.01f * 0x8000);
}

void retro_set_environment(retro_environment_t cb)
{
	environ_cb = cb;

	// An annoyance: retro_set_environment() can be called
	// multiple times, and depending upon the current frontend
	// state various environment callbacks may be disabled.
	// This means the reported 'categories_supported' status
	// may change on subsequent iterations. We therefore have
	// to record whether 'categories_supported' is true on any
	// iteration, and latch the result
	bool optionCategoriesSupported = false;
	libretro_set_core_options(environ_cb, &optionCategoriesSupported);
	categoriesSupported |= optionCategoriesSupported;

	struct retro_core_options_update_display_callback update_display_cb;
	update_display_cb.callback = set_variable_visibility;
	environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_UPDATE_DISPLAY_CALLBACK, &update_display_cb);

	static const struct retro_controller_description ports_default[] =
	{
			{ "Controller",			RETRO_DEVICE_JOYPAD },
			{ "Arcade Stick",		RETRO_DEVICE_ASCIISTICK },
			{ "Keyboard",			RETRO_DEVICE_KEYBOARD },
			{ "Mouse",				RETRO_DEVICE_MOUSE },
			{ "Light Gun",			RETRO_DEVICE_LIGHTGUN },
			{ "Twin Stick",			RETRO_DEVICE_TWINSTICK },
			{ "Saturn Twin-Stick",	RETRO_DEVICE_TWINSTICK_SATURN },
			{ "Pointer",			RETRO_DEVICE_POINTER },
			{ "Maracas",			RETRO_DEVICE_MARACAS },
			{ "Fishing Controller",	RETRO_DEVICE_FISHING },
			{ "Pop'n Music",		RETRO_DEVICE_POPNMUSIC },
			{ "Race Controller",	RETRO_DEVICE_RACING },
			{ "Densha de Go!",		RETRO_DEVICE_DENSHA },
			{ "Full Controller",	RETRO_DEVICE_FULL_CONTROLLER },
	};
	static const struct retro_controller_info ports[] = {
			{ ports_default,  std::size(ports_default) },
			{ ports_default,  std::size(ports_default) },
			{ ports_default,  std::size(ports_default) },
			{ ports_default,  std::size(ports_default) },
			{ 0 },
	};
	environ_cb(RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports);
	const bool b = true;
	environ_cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, (void *)&b);
}

static void retro_keyboard_event(bool down, unsigned keycode, uint32_t character, uint16_t key_modifiers);

// Now comes the interesting stuff
void retro_init()
{
	first_run = true;
	memset(device_type, -1, sizeof(device_type));
	
	static bool emuInited;

	// Logging
	struct retro_log_callback log;
	if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
		log_cb = log.log;
	else
		log_cb = NULL;
	LogManager::Init((void *)log_cb);
	NOTICE_LOG(BOOT, "retro_init");

	if (environ_cb(RETRO_ENVIRONMENT_GET_PERF_INTERFACE, &perf_cb))
		perf_get_cpu_features_cb = perf_cb.get_cpu_features;
	else
		perf_get_cpu_features_cb = NULL;

	// Set color mode
	unsigned color_mode = RETRO_PIXEL_FORMAT_XRGB8888;
	environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &color_mode);

	init_kb_map();
	struct retro_keyboard_callback kb_callback = { &retro_keyboard_event };
	environ_cb(RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK, &kb_callback);

	if (environ_cb(RETRO_ENVIRONMENT_GET_INPUT_BITMASKS, NULL))
		libretro_supports_bitmasks = true;

	init_disk_control_interface();
	retro_audio_init();

#if defined(__APPLE__)
    char *data_dir = NULL;
    if (environ_cb(RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY, &data_dir) && data_dir)
        set_user_data_dir(std::string(data_dir) + "/");
#endif

	if (!addrspace::reserve())
		ERROR_LOG(VMEM, "Cannot reserve memory space");

#if defined(__linux__) || defined(__FreeBSD__)
	// SDL evdev keyboard driver installs a SIGSEGV signal handler by default, which replaces flycast's one.
	// Make sure to avoid this if SDL is initialized after the core (which happens).
	setenv("SDL_NO_SIGNAL_HANDLERS", "1", 1);
#endif
	os_InstallFaultHandler();
	MapleConfigMap::UpdateVibration = updateVibration;
	i18n::init();

#if defined(__APPLE__) || (defined(__GNUC__) && defined(__linux__) && !defined(__ANDROID__))
	if (!emuInited)
#else
	(void)emuInited;
#endif
		emu.init();
	emuInited = true;
}

void retro_deinit()
{
	INFO_LOG(COMMON, "retro_deinit");
	first_run = true;
	memset(device_type, -1, sizeof(device_type));

	//When auto-save states are enabled this is needed to prevent the core from shutting down before
	//any save state actions are still running - which results in partial saves
	{
		std::lock_guard<std::mutex> lock(mtx_serialization);
	}
	os_UninstallFaultHandler();
	
#if defined(__APPLE__) || (defined(__GNUC__) && defined(__linux__) && !defined(__ANDROID__))
	addrspace::release();
#else
	emu.term();
#endif
	libretro_supports_bitmasks = false;
	categoriesSupported = false;
	platformIsDreamcast = true;
	platformIsArcade = false;
	threadedRenderingEnabled = true;
	oitEnabled = false;
	autoSkipFrameEnabled = false;
#ifdef _OPENMP
	textureUpscaleEnabled = false;
#endif
	vmuScreenSettingsShown = true;
	lightgunSettingsShown = true;
	libretro_vsync_swap_interval = 1;
	libretro_detect_vsync_swap_interval = false;
	LogManager::Shutdown();

	retro_audio_deinit();
}

static bool set_variable_visibility(void)
{
	struct retro_core_option_display option_display;
	struct retro_variable var;
	bool updated = false;

	bool platformWasDreamcast = platformIsDreamcast;
	bool platformWasArcade = platformIsArcade;

	platformIsDreamcast = settings.platform.isConsole();
	platformIsArcade = settings.platform.isArcade();

	// Show/hide platform-dependent options
	if (first_run || (platformIsDreamcast != platformWasDreamcast) || (platformIsArcade != platformWasArcade))
	{
		// Show/hide NAOMI/Atomiswave options
		option_display.visible = platformIsArcade;
		option_display.key = CORE_OPTION_NAME "_allow_service_buttons";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		option_display.visible = settings.platform.isNaomi();
		option_display.key = CORE_OPTION_NAME "_force_freeplay";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);

		// Show/hide Dreamcast options
		option_display.visible = platformIsDreamcast;
		option_display.key = CORE_OPTION_NAME "_hle_bios";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		option_display.key = CORE_OPTION_NAME "_gdrom_fast_loading";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		option_display.key = CORE_OPTION_NAME "_cable_type";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		option_display.key = CORE_OPTION_NAME "_broadcast";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		option_display.key = CORE_OPTION_NAME "_language";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		option_display.key = CORE_OPTION_NAME "_force_wince";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		option_display.key = CORE_OPTION_NAME "_per_content_vmus";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		option_display.key = CORE_OPTION_NAME "_dc_32mb_mod";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		option_display.key = CORE_OPTION_NAME "_vmu_sound";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		option_display.visible = platformIsDreamcast || settings.platform.isAtomiswave();
		option_display.key = CORE_OPTION_NAME "_emulate_bba";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		option_display.key = CORE_OPTION_NAME "_upnp";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);

		vmuScreenSettingsShown = option_display.visible;
		for (unsigned i = 0; i < 4; i++)
		{
			char key[256];
			option_display.key = key;

			snprintf(key, sizeof(key), "%s%u%s", CORE_OPTION_NAME "_vmu", i + 1, "_screen_display");
			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
			snprintf(key, sizeof(key), "%s%u%s", CORE_OPTION_NAME "_vmu", i + 1, "_screen_position");
			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
			snprintf(key, sizeof(key), "%s%u%s", CORE_OPTION_NAME "_vmu", i + 1, "_screen_size_mult");
			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
			snprintf(key, sizeof(key), "%s%u%s", CORE_OPTION_NAME "_vmu", i + 1, "_pixel_on_color");
			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
			snprintf(key, sizeof(key), "%s%u%s", CORE_OPTION_NAME "_vmu", i + 1, "_pixel_off_color");
			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
			snprintf(key, sizeof(key), "%s%u%s", CORE_OPTION_NAME "_vmu", i + 1, "_screen_opacity");
			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		}

		// Show/hide manual option visibility toggles
		// > Only show if categories are not supported
		option_display.visible = platformIsDreamcast && !categoriesSupported;
		option_display.key = CORE_OPTION_NAME "_show_vmu_screen_settings";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);

		updated = true;
	}

	// Show/hide additional manual option visibility toggles
	// > Only show if categories are not supported
	if (first_run)
	{
		option_display.visible = !categoriesSupported;
		option_display.key = CORE_OPTION_NAME "_show_lightgun_settings";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		updated = true;
	}

	// Show/hide settings-dependent options

	// Only for threaded renderer
	bool threadedRenderingWasEnabled = threadedRenderingEnabled;
	threadedRenderingEnabled = true;
	var.key = CORE_OPTION_NAME "_threaded_rendering";
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value && !strcmp(var.value, "disabled"))
		threadedRenderingEnabled = false;

	if (first_run || (threadedRenderingEnabled != threadedRenderingWasEnabled))
	{
		option_display.visible = threadedRenderingEnabled;
		option_display.key = CORE_OPTION_NAME "_auto_skip_frame";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		updated = true;
	}

#if defined(HAVE_OIT) || defined(HAVE_VULKAN) || defined(HAVE_D3D11)
	// Only for per-pixel renderers
	bool oitWasEnabled = oitEnabled;
	oitEnabled = false;
	var.key = CORE_OPTION_NAME "_alpha_sorting";
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value && !strcmp(var.value, "per-pixel (accurate)"))
		oitEnabled = true;

	if (first_run || (oitEnabled != oitWasEnabled))
	{
		option_display.visible = oitEnabled;
		option_display.key = CORE_OPTION_NAME "_oit_abuffer_size";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		option_display.key = CORE_OPTION_NAME "_oit_layers";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		updated = true;
	}
#endif

#ifdef _OPENMP
	// Only if texture upscaling is enabled
	bool textureUpscaleWasEnabled = textureUpscaleEnabled;
	textureUpscaleEnabled = false;
	var.key = CORE_OPTION_NAME "_texupscale";
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value && strcmp(var.value, "1"))
		textureUpscaleEnabled = true;

	if (first_run || (textureUpscaleEnabled != textureUpscaleWasEnabled))
	{
		option_display.visible = textureUpscaleEnabled;
		option_display.key = CORE_OPTION_NAME "_texupscale_max_filtered_texture_size";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		updated = true;
	}
#endif

	// Only if automatic frame skipping is disabled
	bool autoSkipFrameWasEnabled = autoSkipFrameEnabled;

	autoSkipFrameEnabled = false;
	var.key = CORE_OPTION_NAME "_auto_skip_frame";
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value && strcmp(var.value, "disabled"))
		autoSkipFrameEnabled = true;

	if (first_run ||
		 (autoSkipFrameEnabled != autoSkipFrameWasEnabled) ||
		 (threadedRenderingEnabled != threadedRenderingWasEnabled))
	{
		option_display.visible = (!autoSkipFrameEnabled || !threadedRenderingEnabled);
		option_display.key = CORE_OPTION_NAME "_detect_vsync_swap_interval";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		updated = true;
	}

	// Show/hide expansion slots options
	if (devices_need_refresh)
	{
		for (int i = 0; i < 4; i++)
		{
			char key[64] = {0};
			option_display.key = key;

			// Show expansion slot 1 options only for DC with these devices
			option_display.visible = platformIsDreamcast
					&& (config::MapleMainDevices[i] == MDT_SegaController
					|| config::MapleMainDevices[i] == MDT_LightGun
					|| config::MapleMainDevices[i] == MDT_TwinStick
					|| config::MapleMainDevices[i] == MDT_AsciiStick
					|| config::MapleMainDevices[i] == MDT_RacingController
					|| config::MapleMainDevices[i] == MDT_SegaControllerXL);

			snprintf(key, sizeof(key), CORE_OPTION_NAME "_device_port%d_slot1", i + 1);
			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);

			// Only the regular controller (and the XL version) has 2 expansion slots
			option_display.visible = platformIsDreamcast
					&& (config::MapleMainDevices[i] == MDT_SegaController || config::MapleMainDevices[i] == MDT_SegaControllerXL);

			snprintf(key, sizeof(key), CORE_OPTION_NAME "_device_port%d_slot2", i + 1);
			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		}

		updated = true;
	}

	// If categories are supported, no further action is required
	if (categoriesSupported)
		return updated;

	// Show/hide VMU screen options
	bool vmuScreenSettingsWereShown = vmuScreenSettingsShown;

	if (platformIsDreamcast)
	{
		vmuScreenSettingsShown = true;
		var.key = CORE_OPTION_NAME "_show_vmu_screen_settings";

		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value && !strcmp(var.value, "disabled"))
			vmuScreenSettingsShown = false;
	}
	else
		vmuScreenSettingsShown = false;

	if (first_run || (vmuScreenSettingsShown != vmuScreenSettingsWereShown))
	{
		option_display.visible = vmuScreenSettingsShown;

		for (unsigned i = 0; i < 4; i++)
		{
			char key[256];
			option_display.key = key;

			snprintf(key, sizeof(key), "%s%u%s", CORE_OPTION_NAME "_vmu", i + 1, "_screen_display");
			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
			snprintf(key, sizeof(key), "%s%u%s", CORE_OPTION_NAME "_vmu", i + 1, "_screen_position");
			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
			snprintf(key, sizeof(key), "%s%u%s", CORE_OPTION_NAME "_vmu", i + 1, "_screen_size_mult");
			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
			snprintf(key, sizeof(key), "%s%u%s", CORE_OPTION_NAME "_vmu", i + 1, "_pixel_on_color");
			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
			snprintf(key, sizeof(key), "%s%u%s", CORE_OPTION_NAME "_vmu", i + 1, "_pixel_off_color");
			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
			snprintf(key, sizeof(key), "%s%u%s", CORE_OPTION_NAME "_vmu", i + 1, "_screen_opacity");
			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		}

		updated = true;
	}

	// Show/hide light gun options
	bool lightgunSettingsWereShown = lightgunSettingsShown;
	lightgunSettingsShown = true;
	var.key = CORE_OPTION_NAME "_show_lightgun_settings";

	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value && !strcmp(var.value, "disabled"))
		lightgunSettingsShown = false;

	if (first_run || (lightgunSettingsShown != lightgunSettingsWereShown))
	{
		option_display.visible = lightgunSettingsShown;

		option_display.key = CORE_OPTION_NAME "_lightgun_crosshair_size_scaling";
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);

		for (unsigned i = 0; i < 4; i++)
		{
			char key[256];
			option_display.key = key;

			snprintf(key, sizeof(key), "%s%u%s", CORE_OPTION_NAME "_lightgun", i + 1, "_crosshair");
			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
		}

		updated = true;
	}

	return updated;
}

static void setGameGeometry(retro_game_geometry& geometry)
{
	geometry.aspect_ratio = framebufferAspectRatio;
	if (rotate_screen)
		geometry.aspect_ratio = 1 / geometry.aspect_ratio;

	// Use same height for rotation potential
	geometry.max_width = std::max(maxFramebufferWidth, framebufferWidth);
	geometry.max_height = geometry.max_width;

	// Avoid gigantic window size at startup
	geometry.base_width = 640;
	geometry.base_height = 480;
}

bool setAVInfo(retro_system_av_info& avinfo)
{
	double sample_rate = 44100.0;
	double fps = SPG_CONTROL.isPAL() ? 50.0 : 59.94;

	// 240p NTSC rate
	if (framebufferHeight == 240 && !SPG_CONTROL.isNTSC() && !SPG_CONTROL.isPAL())
		fps = 59.82366;

	setGameGeometry(avinfo.geometry);
	avinfo.timing.sample_rate = sample_rate;
	avinfo.timing.fps = fps / (double)libretro_vsync_swap_interval;

	libretro_expected_audio_samples_per_run = sample_rate / fps;

	// Avoid video reinit with same timings
	if (avinfo.timing.fps == fps_current)
		return false;

	fps_current = avinfo.timing.fps;
	return true;
}

static bool retro_refresh_av_info(void)
{
	retro_system_av_info avinfo;

	if (first_run || game_data.empty())
		return false;

	if (setAVInfo(avinfo))
	{
		environ_cb(RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO, &avinfo);
		return true;
	}

	return false;
}

void retro_resize_renderer(int w, int h, float aspectRatio)
{
	if (w == framebufferWidth && h == framebufferHeight && aspectRatio == framebufferAspectRatio)
		return;
	framebufferWidth = w;
	framebufferHeight = h;
	framebufferAspectRatio = aspectRatio;
	bool avinfoNeeded = framebufferHeight > maxFramebufferHeight || framebufferWidth > maxFramebufferWidth;
	maxFramebufferHeight = std::max(maxFramebufferHeight, framebufferHeight);
	maxFramebufferWidth = std::max(maxFramebufferWidth, framebufferWidth);

	if (avinfoNeeded)
	{
		retro_system_av_info avinfo;
		setAVInfo(avinfo);
		environ_cb(RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO, &avinfo);
	}
	else
	{
		// Check if timing change is needed instead
		if (retro_refresh_av_info())
			return;

		retro_game_geometry geometry;
		setGameGeometry(geometry);
		environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, &geometry);
	}
}

static void setRotation()
{
	int rotation = 0;
	if (rotate_game)
	{
		if (!rotate_screen)
			rotation = 1;
		rotate_screen = !rotate_screen;
	}
	else
	{
		if (rotate_screen)
			rotation = 3;
	}
	environ_cb(RETRO_ENVIRONMENT_SET_ROTATION, &rotation);
}

static void update_variables(bool first_startup)
{
	bool wasThreadedRendering = config::ThreadedRendering;
	bool prevRotateScreen = rotate_screen;
	bool prevDetectVsyncSwapInterval = libretro_detect_vsync_swap_interval;
	bool emulateBba = config::EmulateBBA;
	MapleDeviceType MapleExpansionDevicesPrev[2] = { MDT_None, MDT_None };
	config::Settings::instance().setRetroEnvironment(environ_cb);
	config::Settings::instance().setOptionDefinitions(option_defs_us);
	config::Settings::instance().load(false);

	retro_variable var;

	var.key = CORE_OPTION_NAME "_per_content_vmus";
	unsigned previous_per_content_vmus = per_content_vmus;
	per_content_vmus = 0;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (!strcmp("VMU A1", var.value))
			per_content_vmus = 1;
		else if (!strcmp("All VMUs", var.value))
			per_content_vmus = 2;
	}
	if (!first_startup && per_content_vmus != previous_per_content_vmus
			&& settings.platform.isConsole())
	{
		// Recreate the VMUs so that the save location is taken into account.
		// Don't do this at startup because we don't know the system type yet
		// and the VMUs haven't been created anyway
		maple_ReconnectDevices();
	}

	var.key = CORE_OPTION_NAME "_screen_rotation";
	rotate_screen = false;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value && !strcmp("vertical", var.value))
		rotate_screen = true;

	var.key = CORE_OPTION_NAME "_internal_resolution";
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		char str[100];
		snprintf(str, sizeof(str), "%s", var.value);

		char *pch = strtok(str, "x");
		pch = strtok(NULL, "x");
		if (pch != nullptr)
			config::RenderResolution = strtoul(pch, NULL, 0);

		DEBUG_LOG(COMMON, "Got height: %u", (int)config::RenderResolution);
	}

	var.key = CORE_OPTION_NAME "_alpha_sorting";
	var.value = nullptr;
	RenderType previous_renderer = config::RendererType;
	environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var);
	if (var.value != nullptr && !strcmp(var.value, "per-pixel (accurate)"))
	{
		switch (config::RendererType)
		{
		case RenderType::Vulkan:
			config::RendererType = RenderType::Vulkan_OIT;
			break;
		case RenderType::DirectX11:
			config::RendererType = RenderType::DirectX11_OIT;
			break;
		case RenderType::OpenGL:
			config::RendererType = RenderType::OpenGL_OIT;
			break;
		default:
			break;
		}
		config::PerStripSorting = false;	// Not used
	}
	else
	{
		switch (config::RendererType)
		{
		case RenderType::Vulkan_OIT:
			config::RendererType = RenderType::Vulkan;
			break;
		case RenderType::DirectX11_OIT:
			config::RendererType = RenderType::DirectX11;
			break;
		case RenderType::OpenGL_OIT:
			config::RendererType = RenderType::OpenGL;
			break;
		default:
			break;
		}
		config::PerStripSorting = var.value != nullptr && !strcmp(var.value, "per-strip (fast, least accurate)");
	}

	if (!first_startup && previous_renderer != config::RendererType) {
		rend_term_renderer();
		rend_init_renderer();
	}

#if defined(HAVE_OIT) || defined(HAVE_VULKAN) || defined(HAVE_D3D11)
	var.key = CORE_OPTION_NAME "_oit_abuffer_size";
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (!strcmp(var.value, "512MB"))
			config::PixelBufferSize = 0x20000000u;
		else if (!strcmp(var.value, "1GB"))
			config::PixelBufferSize = 0x40000000u;
		else if (!strcmp(var.value, "2GB"))
			config::PixelBufferSize = 0x7ff00000u;
		else if (!strcmp(var.value, "4GB"))
			config::PixelBufferSize = 0xFFFFFFFFu;
		else
			config::PixelBufferSize = 0x20000000u;
	}
	else
		config::PixelBufferSize = 0x20000000u;
#endif

	if ((config::AutoSkipFrame != 0) && config::ThreadedRendering)
		libretro_detect_vsync_swap_interval = false;
	else
	{
		var.key = CORE_OPTION_NAME "_detect_vsync_swap_interval";
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		{
			if (!strcmp(var.value, "enabled"))
				libretro_detect_vsync_swap_interval = true;
			else if (!strcmp(var.value, "disabled"))
				libretro_detect_vsync_swap_interval = false;
		}
		else
			libretro_detect_vsync_swap_interval = false;
	}

	if (first_startup)
	{
		if (config::ThreadedRendering)
		{
			bool save_state_in_background = false;
			unsigned poll_type_early      = 1; /* POLL_TYPE_EARLY */
			environ_cb(RETRO_ENVIRONMENT_SET_SAVE_STATE_IN_BACKGROUND, &save_state_in_background);
			environ_cb(RETRO_ENVIRONMENT_POLL_TYPE_OVERRIDE, &poll_type_early);
		}

		config::Cable = 3;
		var.key = CORE_OPTION_NAME "_cable_type";
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		{
			if (!strcmp("VGA", var.value))
				config::Cable = 0;
			else if (!strcmp("TV (RGB)", var.value))
				config::Cable = 2;
		}
	}

	var.key = CORE_OPTION_NAME "_analog_stick_deadzone";
	var.value = NULL;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		input_set_deadzone_stick( atoi( var.value ) );

	var.key = CORE_OPTION_NAME "_trigger_deadzone";
	var.value = NULL;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		input_set_deadzone_trigger( atoi( var.value ) );

	var.key = CORE_OPTION_NAME "_digital_triggers";
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (!strcmp("enabled", var.value))
			digital_triggers = true;
		else
			digital_triggers = false;
	}
	else
		digital_triggers = false;

	var.key = CORE_OPTION_NAME "_allow_service_buttons";
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (!strcmp("enabled", var.value))
			allow_service_buttons = true;
		else
			allow_service_buttons = false;
	}
	else
		allow_service_buttons = false;

	var.key = CORE_OPTION_NAME "_lightgun_crosshair_size_scaling";
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		lightgun_crosshair_size = (float)LIGHTGUN_CROSSHAIR_SIZE * std::stof(var.value) / 100.f;
	else
		lightgun_crosshair_size = (float)LIGHTGUN_CROSSHAIR_SIZE;

	char key[256];
	key[0] = '\0';

	var.key = key ;
	for (int i = 0 ; i < 4 ; i++)
	{
		if (!first_startup && settings.platform.isConsole())
		{
			MapleExpansionDevicesPrev[0] = config::MapleExpansionDevices[i][0];
			MapleExpansionDevicesPrev[1] = config::MapleExpansionDevices[i][1];

			// Check slot options for these devices only, anything else has no slot
			if (config::MapleMainDevices[i] == MDT_SegaController
					|| config::MapleMainDevices[i] == MDT_LightGun
					|| config::MapleMainDevices[i] == MDT_TwinStick
					|| config::MapleMainDevices[i] == MDT_AsciiStick
					|| config::MapleMainDevices[i] == MDT_RacingController
					|| config::MapleMainDevices[i] == MDT_SegaControllerXL)
			{
				for (int slot = 0; slot < 2; slot++)
				{
					// Only regular controller has a 2nd slot
					if (slot == 1 && config::MapleMainDevices[i] != MDT_SegaController && config::MapleMainDevices[i] != MDT_SegaControllerXL)
					{
						config::MapleExpansionDevices[i][1] = MDT_None;
						continue;
					}

					snprintf(key, sizeof(key), CORE_OPTION_NAME "_device_port%d_slot%d", i + 1, slot + 1);
					config::NetworkExpansionDevices[i][slot] = 0;

					if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
					{
						if (!strcmp("VMU", var.value))
							config::MapleExpansionDevices[i][slot] = MDT_SegaVMU;
						else if (!strcmp("Purupuru", var.value))
							config::MapleExpansionDevices[i][slot] = MDT_PurupuruPack;
						else if (!strcmp("None", var.value))
							config::MapleExpansionDevices[i][slot] = MDT_None;
						else if (!strcmp("DreamPotato", var.value)) {
							config::MapleExpansionDevices[i][slot] = MDT_SegaVMU;
							config::NetworkExpansionDevices[i][slot] = 1;
						}
					}
					else if (slot == 0) // Default to VMU in case the above is false somehow
						config::MapleExpansionDevices[i][0] = MDT_SegaVMU;
					else if (slot == 1) // Default to Purupuru
						config::MapleExpansionDevices[i][1] = MDT_PurupuruPack;
				}
			}
			else
			{
				config::MapleExpansionDevices[i][0] = MDT_None;
				config::MapleExpansionDevices[i][1] = MDT_None;
			}

			if (MapleExpansionDevicesPrev[0] != config::MapleExpansionDevices[i][0]
					|| MapleExpansionDevicesPrev[1] != config::MapleExpansionDevices[i][1])
				devices_need_refresh = true;
		}

		lightgun_params[i].offscreen = true;
		lightgun_params[i].x = 0;
		lightgun_params[i].y = 0;
		lightgun_params[i].dirty = true;
		lightgun_params[i].colour = LIGHTGUN_COLOR_OFF;

		snprintf(key, sizeof(key), CORE_OPTION_NAME "_lightgun%d_crosshair", i+1) ;

		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value  )
		{
			if (!strcmp("disabled", var.value))
				lightgun_params[i].colour = LIGHTGUN_COLOR_OFF;
			else if (!strcmp("White", var.value))
				lightgun_params[i].colour = LIGHTGUN_COLOR_WHITE;
			else if (!strcmp("Red", var.value))
				lightgun_params[i].colour = LIGHTGUN_COLOR_RED;
			else if (!strcmp("Green", var.value))
				lightgun_params[i].colour = LIGHTGUN_COLOR_GREEN;
			else if (!strcmp("Blue", var.value))
				lightgun_params[i].colour = LIGHTGUN_COLOR_BLUE;
		}
		if (lightgun_params[i].colour == LIGHTGUN_COLOR_OFF)
			config::CrosshairColor[i] = 0;
		else
			config::CrosshairColor[i] = lightgun_palette[lightgun_params[i].colour * 3]
										| (lightgun_palette[lightgun_params[i].colour * 3 + 1] << 8)
										| (lightgun_palette[lightgun_params[i].colour * 3 + 2] << 16)
										| 0xff000000;

		vmu_lcd_status[i * 2] = false;
		vmuLastChanged[i * 2] = getTimeMs();
		vmu_screen_params[i].vmu_screen_position = UPPER_LEFT;
		vmu_screen_params[i].vmu_screen_size_mult = 1;
		vmu_screen_params[i].vmu_pixel_on_R = VMU_SCREEN_COLOR_MAP[VMU_DEFAULT_ON].r;
		vmu_screen_params[i].vmu_pixel_on_G = VMU_SCREEN_COLOR_MAP[VMU_DEFAULT_ON].g;
		vmu_screen_params[i].vmu_pixel_on_B = VMU_SCREEN_COLOR_MAP[VMU_DEFAULT_ON].b;
		vmu_screen_params[i].vmu_pixel_off_R = VMU_SCREEN_COLOR_MAP[VMU_DEFAULT_OFF].r;
		vmu_screen_params[i].vmu_pixel_off_G = VMU_SCREEN_COLOR_MAP[VMU_DEFAULT_OFF].g;
		vmu_screen_params[i].vmu_pixel_off_B = VMU_SCREEN_COLOR_MAP[VMU_DEFAULT_OFF].b;
		vmu_screen_params[i].vmu_screen_opacity = 0xFF;

		snprintf(key, sizeof(key), CORE_OPTION_NAME "_vmu%d_screen_display", i+1);

		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value && !strcmp("enabled", var.value)
				&& config::MapleExpansionDevices[i][0] == MDT_SegaVMU)
			vmu_lcd_status[i * 2] = true;

		snprintf(key, sizeof(key), CORE_OPTION_NAME "_vmu%d_screen_position", i+1);

		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		{
			if (!strcmp("Upper Left", var.value))
				vmu_screen_params[i].vmu_screen_position = UPPER_LEFT;
			else if (!strcmp("Upper Right", var.value))
				vmu_screen_params[i].vmu_screen_position = UPPER_RIGHT;
			else if (!strcmp("Lower Left", var.value))
				vmu_screen_params[i].vmu_screen_position = LOWER_LEFT;
			else if (!strcmp("Lower Right", var.value))
				vmu_screen_params[i].vmu_screen_position = LOWER_RIGHT;
		}

		snprintf(key, sizeof(key), CORE_OPTION_NAME "_vmu%d_screen_size_mult", i+1);

		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		{
			if (!strcmp("1x", var.value))
				vmu_screen_params[i].vmu_screen_size_mult = 1;
			else if (!strcmp("2x", var.value))
				vmu_screen_params[i].vmu_screen_size_mult = 2;
			else if (!strcmp("3x", var.value))
				vmu_screen_params[i].vmu_screen_size_mult = 3;
			else if (!strcmp("4x", var.value))
				vmu_screen_params[i].vmu_screen_size_mult = 4;
			else if (!strcmp("5x", var.value))
				vmu_screen_params[i].vmu_screen_size_mult = 5;
		}

		snprintf(key, sizeof(key), CORE_OPTION_NAME "_vmu%d_screen_opacity", i + 1);

		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		{
			if (!strcmp("100%", var.value))
				vmu_screen_params[i].vmu_screen_opacity = 255;
			else if (!strcmp("90%", var.value))
				vmu_screen_params[i].vmu_screen_opacity = 9*25.5;
			else if (!strcmp("80%", var.value))
				vmu_screen_params[i].vmu_screen_opacity = 8*25.5;
			else if (!strcmp("70%", var.value))
				vmu_screen_params[i].vmu_screen_opacity = 7*25.5;
			else if (!strcmp("60%", var.value))
				vmu_screen_params[i].vmu_screen_opacity = 6*25.5;
			else if (!strcmp("50%", var.value))
				vmu_screen_params[i].vmu_screen_opacity = 5*25.5;
			else if (!strcmp("40%", var.value))
				vmu_screen_params[i].vmu_screen_opacity = 4*25.5;
			else if (!strcmp("30%", var.value))
				vmu_screen_params[i].vmu_screen_opacity = 3*25.5;
			else if (!strcmp("20%", var.value))
				vmu_screen_params[i].vmu_screen_opacity = 2*25.5;
			else if (!strcmp("10%", var.value))
				vmu_screen_params[i].vmu_screen_opacity = 1*25.5;
		}

		snprintf(key, sizeof(key), CORE_OPTION_NAME "_vmu%d_pixel_on_color", i + 1);

		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value && strlen(var.value)>1)
		{
			int color_idx = atoi(var.value+(strlen(var.value)-2));
			vmu_screen_params[i].vmu_pixel_on_R = VMU_SCREEN_COLOR_MAP[color_idx].r;
			vmu_screen_params[i].vmu_pixel_on_G = VMU_SCREEN_COLOR_MAP[color_idx].g;
			vmu_screen_params[i].vmu_pixel_on_B = VMU_SCREEN_COLOR_MAP[color_idx].b;
		}

		snprintf(key, sizeof(key), CORE_OPTION_NAME "_vmu%d_pixel_off_color", i+1);

		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value && strlen(var.value)>1)
		{
			int color_idx = atoi(var.value+(strlen(var.value)-2));
			vmu_screen_params[i].vmu_pixel_off_R = VMU_SCREEN_COLOR_MAP[color_idx].r;
			vmu_screen_params[i].vmu_pixel_off_G = VMU_SCREEN_COLOR_MAP[color_idx].g;
			vmu_screen_params[i].vmu_pixel_off_B = VMU_SCREEN_COLOR_MAP[color_idx].b;
		}
	}

	set_variable_visibility();

#ifdef __EMSCRIPTEN__
	// WASM without pthreads: force single-threaded rendering.
	// ThreadedRendering causes retro_run() to loop up to 5 emu frames
	// trying to produce a non-dupe, creating massive pacing variance.
	config::ThreadedRendering = false;
#endif

	if (!first_startup)
	{
		if (wasThreadedRendering != config::ThreadedRendering)
		{
			config::ThreadedRendering = wasThreadedRendering;
			try {
				emu.stop();
				config::ThreadedRendering = !wasThreadedRendering;
				emu.start();
			} catch (const FlycastException& e) {
				ERROR_LOG(COMMON, "%s", e.what());
			}
		}
		if (rotate_screen != (prevRotateScreen ^ rotate_game))
		{
			setRotation();
			retro_game_geometry geometry;
			setGameGeometry(geometry);
			environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, &geometry);
		}
		else
			rotate_screen ^= rotate_game;
		if (rotate_game)
			config::Widescreen.override(false);

		if ((libretro_detect_vsync_swap_interval != prevDetectVsyncSwapInterval) &&
			 !libretro_detect_vsync_swap_interval &&
			 (libretro_vsync_swap_interval != 1))
		{
			libretro_vsync_swap_interval = 1;
			retro_system_av_info avinfo;
			setAVInfo(avinfo);
			environ_cb(RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO, &avinfo);
		}
		// must *not* be changed once a game is started
		config::EmulateBBA.override(emulateBba);
		dreampotato::update();
	}
}

#ifdef __EMSCRIPTEN__
// Duration of the previous retro_run's emulation section — the wall-clock
// pacer's headroom gate (catch-up frames only when the machine can afford
// them).
static double fly_last_emu_ms = 0;
// Frame debt in vblank units + the (trim-adjusted) target rate. File scope:
// the pacer block accrues debt, the render loop repays it with measured
// guest time.
static double fly_owed = 0;
// ★ DRC REPAIR (2026-07-24): repayment must be denominated in UNTRIMMED
// native fps. The 2026-07-17 guest-time rework credited debt at the trimmed
// rate — with accrual also at the trimmed rate, the trim cancels out of the
// steady state (balance = exactly native vblanks/wall-sec, production pinned
// to 44100 no matter the trim; measured p=44142 median in locked-60 with the
// queue at half-full and the trim pinned at max). Repaying at native rate
// restores the designed ±0.5% game-speed steer: queue refills at ~220/s.
static double fly_repay_fps = 59.94;
static double fly_base_fps = 59.94;
#endif

void retro_run()
{
#if defined(__EMSCRIPTEN__)
	if (wrc_frameskip_pending) {
		wrc_frameskip_pending = false;
		config::SkipFrame = EM_ASM_INT({ return window.emulator.getPendingFrameSkip(); });
	}
#endif

#if defined(__EMSCRIPTEN__) && !defined(JIT_PROD_BUILD)
	// Auto-run SHIL op tests on first retro_run() call (dev builds only).
	// GATED OFF: these harnesses (esp. singlestep) compile + execute synthetic
	// blocks and run hundreds of programs on the FIRST frame; they do not fully
	// restore global JIT/dispatch state, which black-screens boot. Disabled by
	// default so games boot normally — run them deliberately via the exported
	// _run_*_tests() functions in a controlled capture instead.
	static bool g_autorun_boot_tests = false;
	{
		static bool shil_tests_ran = false;
		if (g_autorun_boot_tests && !shil_tests_ran) {
			shil_tests_ran = true;
			int failures = EM_ASM_INT({ return Module._run_shil_op_tests(); });
			EM_ASM({ console.log('[SHIL-TEST] Auto-run complete: ' + $0 + ' failure(s)'); }, failures);
			int disp_failures = EM_ASM_INT({ return Module._run_dispatch_tests(); });
			EM_ASM({ console.log('[DISP-TEST] Auto-run complete: ' + $0 + ' failure(s)'); }, disp_failures);
			int rte_failures = EM_ASM_INT({ return Module._run_rte_tests(); });
			EM_ASM({ console.log('[RTE-TEST] Auto-run complete: ' + $0 + ' failure(s)'); }, rte_failures);
			int ss_failures = EM_ASM_INT({ return Module._run_singlestep_tests(); });
			EM_ASM({ console.log('[SS-TEST] Auto-run complete: ' + $0 + ' failure(s)'); }, ss_failures);
		}
	}

	// Freeze watchdog + progress beacon.
	//
	// Primary signal: render_called (g_fly_rend_start_render_calls). If
	// this stops advancing for 3s+, the game stopped presenting frames —
	// that's the user-visible "frozen/halted" state, regardless of whether
	// SH4 block dispatch is still happening.
	//
	// Also emits a periodic progress beacon every 2s with delta stats
	// (blocks_rate, render_rate, cycle delta). Catches gradual slowdowns
	// that don't trip the hard freeze threshold.
	{
		static bool freeze_fired = false;
		static u32 last_renders = 0;
		static u32 last_blocks = 0;
		static int  last_cycle = 0;
		static double last_render_progress_ms = 0;
		static double beacon_last_ms = 0;
		static u32 beacon_blocks_base = 0;
		static u32 beacon_renders_base = 0;
		static double hidden_since_ms = 0;

		double now_ms = emscripten_get_now();
		int hidden = EM_ASM_INT({
			return (typeof document !== 'undefined' && document.hidden) ? 1 : 0;
		});
		if (hidden) {
			if (hidden_since_ms == 0) hidden_since_ms = now_ms;
		} else if (hidden_since_ms > 0) {
			double skew = now_ms - hidden_since_ms;
			last_render_progress_ms += skew;
			beacon_last_ms += skew;
			hidden_since_ms = 0;
		}

		Sh4Context& ctx = Sh4cntx;
		u32 cur_renders = g_fly_rend_start_render_calls;
		u32 cur_blocks  = g_wasm_block_count;
		int cur_cycle   = ctx.cycle_counter;

		// --- Progress beacon (every 2s, always emitted, even when rendering OK) ---
		// Writes to window._flyLog (JS array) instead of console.error, so the
		// user can retrieve clean output via `copy(window._flyLog.join('\n'))`
		// without drowning in error-level spam from the rest of the engine.
		if (!hidden && (beacon_last_ms == 0 || now_ms - beacon_last_ms >= 2000.0)) {
			static u32 beacon_asic_base = 0;
			static u32 beacon_vbin_base = 0;
			static u32 beacon_exc_base = 0;
			static u32 beacon_rte_base = 0;
			static u64 beacon_sched_base = 0;
			if (beacon_last_ms > 0) {
				double dt_ms = now_ms - beacon_last_ms;
				u32 dblocks = cur_blocks - beacon_blocks_base;
				u32 drend   = cur_renders - beacon_renders_base;
				u32 dasic   = g_fly_asic_total - beacon_asic_base;
				u32 dvbin   = g_fly_asic_vblank_in - beacon_vbin_base;
				u32 dexc    = g_fly_exc_total - beacon_exc_base;
				u32 drte    = g_fly_rte_count - beacon_rte_base;
				u64 sched_now = sh4_sched_now64();
				u32 dsched  = (u32)(sched_now - beacon_sched_base);
				u32 buf = (u32)retro_audio_buffer_fill();
				EM_ASM({
					if (!window._flyLog) window._flyLog = [];
					window._flyLog.push('[BEACON] dt=' + $0.toFixed(0) + 'ms'
						+ ' blk=' + $1 + '/s'
						+ ' rend=' + $2 + '/s'
						+ ' vbin=' + $3
						+ ' asic=' + $4
						+ ' exc=' + $5
						+ ' rte=' + $6
						+ ' sched+=' + $7
						+ ' pc=0x' + ($8>>>0).toString(16)
						+ ' cc=' + $9
						+ ' ipend=0x' + ($10>>>0).toString(16));
					if (window._flyLog.length > 500) window._flyLog.shift();
				}, dt_ms,
				   (u32)(dblocks * 1000.0 / dt_ms),
				   (u32)(drend   * 1000.0 / dt_ms),
				   dvbin, dasic, dexc, drte, dsched,
				   ctx.pc, cur_cycle, ctx.interrupt_pend);
				(void)buf;
			}
			beacon_last_ms = now_ms;
			beacon_blocks_base = cur_blocks;
			beacon_renders_base = cur_renders;
			beacon_asic_base = g_fly_asic_total;
			beacon_vbin_base = g_fly_asic_vblank_in;
			beacon_exc_base = g_fly_exc_total;
			beacon_rte_base = g_fly_rte_count;
			beacon_sched_base = sh4_sched_now64();
		}

		// --- Hard freeze watchdog: render_called stalled for 3s ---
		if (!freeze_fired) {
			if (cur_renders != last_renders) {
				last_renders = cur_renders;
				last_render_progress_ms = now_ms;
			} else if (last_render_progress_ms > 0 && !hidden
			           && (now_ms - last_render_progress_ms) > 3000.0) {
				freeze_fired = true;
				double stall_ms = now_ms - last_render_progress_ms;
				FLY_EVT(FLY_EVT_FREEZE_DETECTED,
					(u32)stall_ms, ctx.pc,
					*(u32*)&ctx.sr, ctx.interrupt_pend);
				EM_ASM({
					if (!window._flyLog) window._flyLog = [];
					var L = window._flyLog;
					L.push('[FREEZE] render_called stalled ' + $0.toFixed(0) + 'ms (blocks_still=' + $10 + ')');
					L.push('[FREEZE] ctx.pc=0x' + ($1>>>0).toString(16)
						+ ' sr=0x' + ($2>>>0).toString(16)
						+ ' interrupt_pend=0x' + ($3>>>0).toString(16));
					L.push('[FREEZE] cycle_counter=' + $4
						+ ' sh4_sched_next=' + $5
						+ ' jdyn=0x' + ($6>>>0).toString(16));
					L.push('[FREEZE] gbr=0x' + ($7>>>0).toString(16)
						+ ' vbr=0x' + ($8>>>0).toString(16)
						+ ' pr=0x' + ($9>>>0).toString(16));
					// Surface one console.error line so the user notices it fired.
					console.error('[FREEZE] Triggered. Run: copy(window._flyLog.join("\\n"))');
				}, stall_ms, ctx.pc, *(u32*)&ctx.sr, ctx.interrupt_pend,
				   ctx.cycle_counter, ctx.sh4_sched_next, ctx.jdyn,
				   ctx.gbr, ctx.vbr, ctx.pr,
				   (u32)(cur_blocks - last_blocks));
				EM_ASM({
					window._flyLog.push('[FREEZE] render_called=' + $0
						+ ' audio_buffer=' + $1);
				}, cur_renders, (u32)retro_audio_buffer_fill());

				// Interrupt counters
				EM_ASM({
					window._flyLog.push('[FREEZE] asic_total=' + $0
						+ ' vblank_in=' + $1
						+ ' vblank_out=' + $2
						+ ' hblank=' + $3
						+ ' maple=' + $4);
				}, g_fly_asic_total, g_fly_asic_vblank_in,
				   g_fly_asic_vblank_out, g_fly_asic_hblank, g_fly_asic_maple);
				// Scheduler state
				EM_ASM({
					window._flyLog.push('[FREEZE] sched_now=' + $0);
				}, (u32)sh4_sched_now64());

				// Exception histogram + last N (epc, expEvn) pairs.
				// rte_interp is only interpreter-path RTEs (usually wrong ~0).
				// bl_clear is the authoritative "successful exception recovery"
				// counter — catches both RTE and direct-SR-write paths.
				EM_ASM({
					window._flyLog.push('[FREEZE] Do_Exception total=' + $0
						+ ' rte_interp=' + $1
						+ ' bl_set=' + $2
						+ ' bl_clear=' + $3);
				}, g_fly_exc_total, g_fly_rte_count,
				   g_fly_bl_set_count, g_fly_bl_clear_count);
				// Walk the histogram for any non-zero buckets
				for (u32 i = 0; i < 1024; i++) {
					if (g_fly_exc_count[i] > 0) {
						EM_ASM({
							window._flyLog.push('[FREEZE]   exc expEvn=0x'
								+ ($0>>>0).toString(16) + ' count=' + $1);
						}, i, g_fly_exc_count[i]);
					}
				}
				// Dump last 32 (epc, expEvn) pairs + the SH4 opcode word at each epc.
				// Reading mem_b directly for RAM addrs (area 3, bit pattern 0x8C or 0xAC).
				u32 ehead = g_fly_exc_trace_head;
				u32 ecount = ehead < 32 ? ehead : 32;
				EM_ASM({
					window._flyLog.push('[FREEZE] exception trace (last ' + $0 + '):');
				}, ecount);
				for (u32 i = 0; i < ecount; i++) {
					u32 idx = (ehead - ecount + i) & 31;
					u32 epc = g_fly_exc_trace_epc[idx];
					u32 op = 0xFFFF;
					u32 phys = epc & 0x1FFFFFFF;
					if ((phys >> 26) == 3) { // main RAM (area 3)
						op = *(u16*)&mem_b[phys & 0x00FFFFFF];
					}
					EM_ASM({
						window._flyLog.push('[FREEZE]   exc#' + $0
							+ ' epc=0x' + ($1>>>0).toString(16)
							+ ' evn=0x' + ($2>>>0).toString(16)
							+ ' op=0x' + ($3>>>0).toString(16).padStart(4,'0'));
					}, (u32)(i - ecount), epc, g_fly_exc_trace_evn[idx], op);
				}

				// First-caller lookup for the last faulting PC (the one
				// that kicked off this exception loop).
				if (ehead > 0) {
					u32 last_epc = g_fly_exc_trace_epc[(ehead - 1) & 31];
					fly_dump_new_pcs(last_epc);
				}
				// PC trace snapshot taken at the very first illegal fault —
				// this is the ACTUAL caller chain, not the fault-loop.
				fly_dump_first_exc_trace();

				// RAM dumps around suspect addresses
				auto dump_ram_range = [](const char* label, u32 start, u32 words) {
					EM_ASM({
						window._flyLog.push('[FREEZE] RAM ' + UTF8ToString($0)
							+ ' at 0x' + ($1>>>0).toString(16) + ':');
					}, label, 0x8C000000 | start);
					for (u32 i = 0; i < words; i++) {
						u32 off = start + i * 2;
						u16 w = *(u16*)&mem_b[off];
						EM_ASM({
							window._flyLog.push('[FREEZE]   [0x' + ($0>>>0).toString(16)
								+ '] = 0x' + ($1>>>0).toString(16).padStart(4,'0'));
						}, 0x8C000000 | off, (u32)w);
					}
				};
				dump_ram_range("boot/bios vectors", 0x000000, 24);
				dump_ram_range("around faulting PC", 0x1b89a0, 16);

				fly_dump_pc_trace();
			}
			last_blocks = cur_blocks;
			(void)last_cycle; (void)cur_cycle;
		}
	}
#endif
#ifdef __EMSCRIPTEN__
	// ★ WALL-CLOCK PACER (2026-07-17): run DC frames according to REAL TIME
	// at base_fps, decoupled from display Hz. The previous 1-frame-per-rAF
	// pacer tied emulation speed to the display (overspeed on >60Hz screens,
	// permanent slowdown on janky/dropped rAFs — the latter starving audio).
	// Per retro_run: accumulate owed frames with fractional carry; run 0
	// (display faster than DC — present dupe), 1 (normal), or 2 (bounded
	// catch-up). Anti-spiral (the old unbounded catch-up death): catch-up
	// only when the previous run had headroom, and debt beyond 3 frames is
	// SHED (tab switch / hard jank) instead of chased.
	int fly_frames_to_run = 1;
	{
		static double prev_time = 0;
		static int frame_count = 0;
		static double hud_last_update = 0;
		static double frame_time_min = 999;
		static double frame_time_max = 0;
		static double frame_time_sum = 0;

		double base_fps = SPG_CONTROL.isPAL() ? 50.0 : 59.94;
		fly_repay_fps = base_fps;   // native, BEFORE the trim below
		// ★ DYNAMIC RATE TRIM (2026-07-17): the wall clock and the audio
		// hardware's crystal tick at slightly different rates, so a fixed-
		// rate producer drifts the worklet queue until it runs dry (skipped
		// beat fragments) or overfills. Steer emulation speed ±0.5%
		// (imperceptible) to hold the queue at ~4096 frames (~93ms).
		{
			int fly_wq = EM_ASM_INT({
				var A = Module._flyAud;
				return (A && A.ready) ? (A.stats.avail | 0) : -1;
			});
			if (fly_wq >= 0) {
				// ★ SATURATING DRC (2026-07-24): the proportional form
				// asymptotes — +88/s at half-empty but only ~+25/s by 3500,
				// so the queue never finishes the climb before the next dip
				// (eyes: "reaches mid-3000s, equally often in 2000s").
				// Below the near-band spend the full +-0.5% (the cap is the
				// inaudibility law and is unchanged); proportional only
				// inside the last 512 so it settles without hunting.
				double trim;
				if (fly_wq < 4096 - 512) {
					trim = 0.005;
				} else {
					double err = ((double)fly_wq - 4096.0) / 4096.0;
					trim = -err * 0.04;
				}
				if (trim > 0.005) trim = 0.005;
				if (trim < -0.005) trim = -0.005;
				base_fps *= 1.0 + trim;
			}
		}
		fly_base_fps = base_fps;   // render loop repays debt at this rate
		double period_ms = 1000.0 / base_fps;
		double now = emscripten_get_now();
		double elapsed = (prev_time > 0) ? (now - prev_time) : 0;
		prev_time = now;

		if (elapsed > 0)
			fly_owed += elapsed / period_ms;
		else
			fly_owed = 1;
		if (fly_owed > 3)
			fly_owed = 1;                       // shed unpayable debt
		fly_frames_to_run = (int)fly_owed;
		if (fly_frames_to_run > 2)
			fly_frames_to_run = 2;
		if (fly_frames_to_run == 2 && fly_last_emu_ms > period_ms)
			fly_frames_to_run = 1;              // no headroom — never spiral
		if (settings.input.fastForwardMode) {
			fly_frames_to_run = 1;              // FF: old rAF-paced behavior
			fly_owed = 0;
		}
		// ★ GUEST-TIME ACCOUNTING (2026-07-17): debt is repaid by the guest
		// time each render actually consumed (measured via sh4_sched_now64 in
		// the render loop below), NOT by render count. A 30fps-native game
		// (JGR) advances TWO vblanks per render — counting renders made the
		// guest run up to 2x wall clock (measured: audio overproduction
		// +3.4%, 707K frames dropped, beat skipping). BIOS/60fps content was
		// unaffected, which is why it sounded perfect.

		// Track frame-to-frame timing
		if (elapsed > 0) {
			frame_time_sum += elapsed;
			if (elapsed < frame_time_min) frame_time_min = elapsed;
			if (elapsed > frame_time_max) frame_time_max = elapsed;
		}
		frame_count += fly_frames_to_run;       // count EMULATED frames (true DC fps)
		// ★ CORE-TIME SPLIT (2026-07-21, render-campaign scoping): accumulate
		// the previous tick's core-section duration (emu + in-core GL draw,
		// measured as fly_last_emu_ms). HUD shows its avg as `core:` —
		// ft − core = browser/EJS/present share; core − emu(ring ~5ms) = GL
		// draw inside the core. Decomposes the frame on REAL hardware, which
		// headless timing is structurally blind to.
		static double fly_core_ms_sum = 0;
		static int fly_core_ticks = 0;
		if (fly_last_emu_ms > 0) {
			fly_core_ms_sum += fly_last_emu_ms;
			fly_core_ticks++;
		}
		// Render-phase accumulators from the renderer (gles.cpp/ta_util.cpp):
		// [0]=ta_parse total [1]=sort [2]=GL submit [3]=render count
		// [4]=emu.render() total [5]=AICA/ARM7.
		// Averaged per RENDER (not per vblank) at each HUD tick, then reset.
		extern double g_fly_rp[10];

		// HUD: FPS + frame timing (min/avg/max)
		if (hud_last_update == 0) hud_last_update = now;
		if (now - hud_last_update >= 1000.0) {
			double avg_ft = (frame_count > 0) ? (frame_time_sum / frame_count) : 0;
			size_t abuf = retro_audio_buffer_fill();
			// Audio telemetry: produced frames/s (44100 = perfectly fed),
			// cumulative overflow events (ov) and frontend-refused frames (sh).
			static u32 aud_prod_last = 0;
			u32 aud_prod_rate = g_aud_produced_frames - aud_prod_last;
			aud_prod_last = g_aud_produced_frames;
#if FLY_RELEASE_BUILD
			// RELEASE: the HUD is opt-in via ?hud=1 in the page URL (checked
			// once); the hudlog POST block below is compiled out entirely
			// (users' servers have no /hudlog endpoint).
			static const bool fly_hud_optin = EM_ASM_INT({
				return location.search.indexOf('hud=1') >= 0 ? 1 : 0;
			}) != 0;
			if (fly_hud_optin)
#endif
			{
			EM_ASM({
				var el = document.getElementById('fc-hud');
				if (!el) {
					el = document.createElement('div');
					el.id = 'fc-hud';
					el.style.cssText = 'position:fixed;top:8px;left:8px;z-index:99999;'
						+ 'background:rgba(0,0,0,0.7);color:#fff;font:12px monospace;'
						+ 'padding:4px 8px;pointer-events:none;border-radius:3px;';
					document.body.appendChild(el);
				}
				var A = Module._flyAud;
				var wq = A ? (A.stats.avail | 0) : -1;
				var un = A ? (A.stats.under | 0) : 0;
				var dr = A ? ((A.drop | 0) + (A.stats.drop | 0)) : 0;
				el.textContent = 'FPS:' + $0 + '/' + $1.toFixed(0)
					+ ' p:' + $6 + '/s wq:' + wq + ' un:' + un + ' dr:' + dr
					+ ' ov:' + $7
					+ ' ft:' + $2.toFixed(0) + '/' + $3.toFixed(0) + '/' + $4.toFixed(0)
					+ ' core:' + $9.toFixed(1)
					+ ' rp:' + $10.toFixed(1) + '/' + $11.toFixed(1) + '/' + $12.toFixed(1)
					+ ' er:' + $13.toFixed(1) + ' aica:' + $14.toFixed(1);
				// ★ HUD RECORDER (2026-07-21): every HUD update is also queued
				// as a JSON line and flushed to the demo server's /hudlog sink
				// every 10 samples — a played session leaves a machine-readable
				// timeline (upstream/logs/hudlog-<sess>.jsonl) with no user
				// effort. JSON built by string concat + XHR (EM_ASM cannot
				// carry JS object literals — preprocessor gotcha).
				var W = window;
				if (!W._flyHudSess) {
					W._flyHudSess = Date.now().toString(36);
					W._flyHudBuf = [];
				}
				// EM_ASM is capped at 16 args ($0-$15) — '$16 is not defined'
				// killed the page once. The sample line is built across TWO
				// EM_ASM calls: this one leaves the JSON open in _flyHudPart.
				W._flyHudPart = '{"t":' + Date.now()
					+ ',"fps":' + $0
					+ ',"tgt":' + (+$1.toFixed(1))
					+ ',"ft_min":' + (+$2.toFixed(1))
					+ ',"ft_avg":' + (+$3.toFixed(1))
					+ ',"ft_max":' + (+$4.toFixed(1))
					+ ',"core":' + (+$9.toFixed(2))
					+ ',"rp_parse":' + (+$10.toFixed(2))
					+ ',"rp_sort":' + (+$11.toFixed(2))
					+ ',"rp_draw":' + (+$12.toFixed(2))
					+ ',"rp_er":' + (+$13.toFixed(2))
					+ ',"rp_aica":' + (+$14.toFixed(2))
					+ ',"owed":' + (+$15.toFixed(2))
					+ ',"wq":' + wq + ',"un":' + un + ',"dr":' + dr;
			}, frame_count, base_fps, frame_time_min, avg_ft, frame_time_max,
			   (int)abuf, aud_prod_rate, g_aud_overflow_events, g_aud_shortfall_frames,
			   fly_core_ticks > 0 ? fly_core_ms_sum / fly_core_ticks : 0.0,
			   g_fly_rp[3] > 0 ? (g_fly_rp[0] - g_fly_rp[1]) / g_fly_rp[3] : 0.0,
			   g_fly_rp[3] > 0 ? g_fly_rp[1] / g_fly_rp[3] : 0.0,
			   g_fly_rp[3] > 0 ? g_fly_rp[2] / g_fly_rp[3] : 0.0,
			   g_fly_rp[3] > 0 ? g_fly_rp[4] / g_fly_rp[3] : 0.0,
			   g_fly_rp[3] > 0 ? g_fly_rp[5] / g_fly_rp[3] : 0.0,
			   fly_owed);
#if !FLY_RELEASE_BUILD
			EM_ASM({
				var W = window;
				if (!W._flyHudPart) return;
				W._flyHudBuf.push(W._flyHudPart
					+ ',"vbl":' + (+$0.toFixed(1))
					+ ',"mft":' + $1
					+ ',"rp_ml":' + (+$4.toFixed(2))
					+ ',"p":' + $2 + ',"ov":' + $3 + '}');
				W._flyHudPart = null;
				if (W._flyHudBuf.length >= 10) {
					var pl = '{"session":"' + W._flyHudSess + '","samples":['
						+ W._flyHudBuf.join(',') + ']}';
					W._flyHudBuf.length = 0;
					try {
						var x = new XMLHttpRequest();
						x.open('POST', '/hudlog', true);
						x.send(pl);
					} catch (e) {}
				}
			}, g_fly_rp[6], (int)g_fly_rp[7], aud_prod_rate, g_aud_overflow_events,
			   g_fly_rp[6] > 0 ? g_fly_rp[8] / g_fly_rp[6] : 0.0);
#endif  // !FLY_RELEASE_BUILD (hudlog POST)
			}
#ifndef JIT_PROD_BUILD
			EM_ASM({
				console.log('[PERF] ' + '{"type":"frame_timing","fps":' + $0
					+ ',"target_fps":' + (+$1.toFixed(0))
					+ ',"ft_min":' + (+$2.toFixed(1))
					+ ',"ft_avg":' + (+$3.toFixed(1))
					+ ',"ft_max":' + (+$4.toFixed(1))
					+ ',"aud_p":' + $5
					+ ',"aud_ov":' + $6
					+ ',"aud_sh":' + $7
					+ ',"frames":' + $0 + '}');
			}, frame_count, base_fps, frame_time_min, avg_ft, frame_time_max,
			   aud_prod_rate, g_aud_overflow_events, g_aud_shortfall_frames);
#endif
			frame_count = 0;
			frame_time_min = 999;
			frame_time_max = 0;
			frame_time_sum = 0;
			fly_core_ms_sum = 0;
			fly_core_ticks = 0;
			g_fly_rp[0] = g_fly_rp[1] = g_fly_rp[2] = g_fly_rp[3] = 0;
			g_fly_rp[4] = g_fly_rp[5] = g_fly_rp[6] = g_fly_rp[7] = 0;
			g_fly_rp[8] = 0;
			hud_last_update = now;
		}
	}
#endif

	bool updated = false;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
		update_variables(false);

	if (devices_need_refresh)
		refresh_devices(false);

	if (custom_texture.isPreloading())
	{
		int texLoaded, texTotal;
		size_t loaded_size;
		custom_texture.getPreloadProgress(texLoaded, texTotal, loaded_size);

		static char msg_buf[64];
		float loaded_size_mb = (float)loaded_size / (1024 * 1024);
		snprintf(msg_buf, sizeof(msg_buf), "Preloading custom textures: %d / %d (%.1f MB)", texLoaded, texTotal, loaded_size_mb);

		struct retro_message msg;
		msg.msg = msg_buf;
		msg.frames = 1;
		environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, &msg);

		video_cb(NULL, 0, 0, 0);
		poll_cb();
		return;
	}

#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES)
	if (isOpenGL(config::RendererType))
		glsm_ctl(GLSM_CTL_STATE_BIND, nullptr);
#endif

	// On the first call, we start the emulator
	if (first_run)
		emu.start();

	poll_cb();
	os_UpdateInputState();
	bool fastforward = false;
	if (environ_cb(RETRO_ENVIRONMENT_GET_FASTFORWARDING, &fastforward))
		settings.input.fastForwardMode = fastforward;

	is_dupe = true;
	try {
		if (config::ThreadedRendering)
		{
			// Render
			for (int i = 0; i < 5 && is_dupe; i++)
				is_dupe = !emu.render();
		}
		else
		{
#ifdef __EMSCRIPTEN__
			// Wall-clock pacer: 0 frames presents a dupe, 2 = bounded
			// catch-up. Debt is repaid with the GUEST time each render
			// consumed (30fps-native games advance 2 vblanks per render).
			// SH4 main clock = 200MHz.
			double fly_run_t0 = emscripten_get_now();
			{
				// Pacer-state telemetry [6]=vblanks emulated, [7]=multi-frame
				// ticks (the debt-spiral counters; HUD reports per second)
				extern double g_fly_rp[10];
				if (fly_frames_to_run > 1)
					g_fly_rp[7] += 1;
				u64 fly_vb_t0 = sh4_sched_now64();
				for (int fly_i = 0; fly_i < fly_frames_to_run && (fly_i == 0 || fly_owed >= 1.0); fly_i++) {
					// ★ PACER-SPREAD (2026-07-24): one guest-time budget for
					// the WHOLE host tick — without this, the catch-up
					// iteration can take a second timed-out gulp right after
					// the first, rebuilding the 3-vblank freeze the lowered
					// render timeout (emulator.cpp vblank) just split up.
					if (fly_i > 0 && sh4_sched_now64() - fly_vb_t0 >= 5000000)
						break;
					startTime = sh4_sched_now64();
					{
						double fly_er_t0 = emscripten_get_now();
						emu.render();
						g_fly_rp[4] += emscripten_get_now() - fly_er_t0;
					}
					fly_owed -= (double)(sh4_sched_now64() - startTime)
					            * fly_repay_fps / 200000000.0;
				}
				g_fly_rp[6] += (double)(sh4_sched_now64() - fly_vb_t0)
				               * fly_repay_fps / 200000000.0;
			}
			if (fly_owed < -2.0) fly_owed = -2.0;  // bounded credit
			fly_last_emu_ms = emscripten_get_now() - fly_run_t0;
#else
			startTime = sh4_sched_now64();
			emu.render();
#endif
		}
	} catch (const FlycastException& e) {
		ERROR_LOG(COMMON, "%s", e.what());
		os_notify(e.what(), 5000);
		environ_cb(RETRO_ENVIRONMENT_SHUTDOWN, NULL);
	}
#if defined(__EMSCRIPTEN__) && !defined(JIT_PROD_BUILD)
	// Exception escape probe — logs which exception type escaped the
	// FlycastException catch above. We catch, log diagnostic data, then
	// RE-THROW to preserve existing crash behavior (the surgical fix
	// session proved a broad catch breaks input handling). If something
	// escapes to Emscripten's onAbort, this tells us what it was.
	catch (const SH4ThrownException& ex) {
		FLY_EVT(FLY_EVT_ESCAPE_EXCEPTION, 0, ex.epc, (u32)ex.expEvn, 0);
		EM_ASM({
			if (!window._flyLog) window._flyLog = [];
			window._flyLog.push('[ESCAPE] SH4ThrownException epc=0x'
				+ ($0>>>0).toString(16) + ' expEvn=0x' + ($1>>>0).toString(16));
			console.error('[ESCAPE] SH4ThrownException — see window._flyLog');
		}, ex.epc, (u32)ex.expEvn);
		throw;
	} catch (const std::exception& ex) {
		FLY_EVT(FLY_EVT_ESCAPE_EXCEPTION, 1, 0, 0, 0);
		EM_ASM({
			if (!window._flyLog) window._flyLog = [];
			window._flyLog.push('[ESCAPE] std::exception what=' + UTF8ToString($0));
			console.error('[ESCAPE] std::exception — see window._flyLog');
		}, ex.what());
		throw;
	} catch (...) {
		FLY_EVT(FLY_EVT_ESCAPE_EXCEPTION, 2, 0, 0, 0);
		EM_ASM({
			if (!window._flyLog) window._flyLog = [];
			window._flyLog.push('[ESCAPE] unknown exception');
			console.error('[ESCAPE] unknown exception — see window._flyLog');
		});
		throw;
	}
#endif

#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES)
	if (isOpenGL(config::RendererType))
		glsm_ctl(GLSM_CTL_STATE_UNBIND, nullptr);
#endif

	// Unless VGA cable is selected, We need to update
	// the refresh rate for PAL games with a 60Hz mode
	bool pal_check = SPG_CONTROL.isPAL();
	if (is_pal != pal_check)
	{
		retro_system_av_info avinfo;
		is_pal = pal_check;
		setAVInfo(avinfo);
		environ_cb(RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO, &avinfo);
#if defined(__EMSCRIPTEN__)
		// WRC (2026-09-09): the auto frame-skip heuristic's FPS threshold
		// (see emulator/index.js's onFpsUpdate()) is relative to the
		// game's real target rate, not a fixed 60 - a native 50Hz PAL
		// game would otherwise always read as "too slow" against a
		// hardcoded NTSC threshold. is_pal here is the same
		// SPG_CONTROL.isPAL()-driven, hardware-register-backed detection
		// setAVInfo() itself uses, so this fires exactly when the real
		// target rate changes, in either direction.
		EM_ASM({ window.emulator.setPalMode(!!$0); }, is_pal ? 1 : 0);
#endif
	}

#if defined(__EMSCRIPTEN__) && !defined(JIT_PROD_BUILD) && FLY_RETRO_DIAG
	{
		static int videocb_count = 0;
		static int first_non_dupe = 0;
		videocb_count++;
		if (!is_dupe && first_non_dupe == 0) {
			first_non_dupe = videocb_count;
			EM_ASM({ console.log('[retro] FIRST REAL FRAME at video_cb #' + $0 + ' w=' + $1 + ' h=' + $2); },
				videocb_count, framebufferWidth, framebufferHeight);
		}
		if (videocb_count <= 5 || (videocb_count % 10) == 0) {
			EM_ASM({ console.log('[retro] video_cb #' + $0 + ': is_dupe=' + $1 + ' w=' + $2 + ' h=' + $3); },
				videocb_count, is_dupe ? 1 : 0, framebufferWidth, framebufferHeight);
		}
	}
#endif
	video_cb(is_dupe ? 0 : RETRO_HW_FRAME_BUFFER_VALID, framebufferWidth, framebufferHeight, 0);

	if (!config::ThreadedRendering || config::LimitFPS)
		retro_audio_upload();
	else
		retro_audio_flush_buffer();

	first_run = false;
}

static bool loadGame()
{
	try {
		emu.loadGame(game_data.c_str());
	} catch (const FlycastException& e) {
		ERROR_LOG(BOOT, "%s", e.what());
		os_notify(e.what(), 5000);
        retro_unload_game();
		return false;
	}

	return true;
}

void retro_reset()
{
	std::lock_guard<std::mutex> lock(mtx_serialization);

	emu.unloadGame();

	config::ScreenStretching = 100;
	loadGame();
	if (rotate_game)
		config::Widescreen.override(false);
	config::Rotate90 = false;

	retro_game_geometry geometry;
	setGameGeometry(geometry);
	environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, &geometry);
	blankVmus();
	retro_audio_flush_buffer();

	emu.start();
}

#if defined(HAVE_OIT) || defined(HAVE_VULKAN) || defined(HAVE_D3D11)
void check_per_pixel_opt(void)
{
	// Check if per-pixel is supported, if not we hide the option
	if (!GraphicsContext::Instance()->hasPerPixel())
	{
		for (unsigned i = 0; option_defs_us[i].key != NULL; i++)
		{
			// Looking for the alpha sorting core option...
			if (!strcmp(option_defs_us[i].key, CORE_OPTION_NAME "_alpha_sorting"))
			{
				for (unsigned j = 0; option_defs_us[i].values[j].value != NULL; j++)
				{
					// ... then for the per-pixel choice...
					if (!strcmp(option_defs_us[i].values[j].value, "per-pixel (accurate)"))
					{
						// ... null it out...
						option_defs_us[i].values[j] = { NULL, NULL };

						// ... and finally refresh core options.
						bool optionCategoriesSupported = false;
						libretro_set_core_options(environ_cb, &optionCategoriesSupported);
						categoriesSupported |= optionCategoriesSupported;

						break;
					}
				}
				break;
			}
		}
		NOTICE_LOG(RENDERER, "Current renderer does not support 'Per-Pixel' Alpha Sorting.");
	}
	perPixelChecked = true;
}
#endif

#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES)
static void context_reset()
{
	INFO_LOG(RENDERER, "GL context_reset");
	gl_ctx_resetting = false;
	glsm_ctl(GLSM_CTL_STATE_CONTEXT_RESET, NULL);
	glsm_ctl(GLSM_CTL_STATE_SETUP, NULL);
	rend_term_renderer();
	theGLContext.init();
	rend_init_renderer();
#ifdef HAVE_OIT
	if (!perPixelChecked)
		check_per_pixel_opt();
#endif
}

static void context_destroy()
{
	gl_ctx_resetting = true;
	rend_term_renderer();
	glsm_ctl(GLSM_CTL_STATE_CONTEXT_DESTROY, NULL);
}
#endif

static void extract_directory(char *buf, const char *path, size_t size)
{
	strncpy(buf, path, size - 1);
	buf[size - 1] = '\0';

	char *base = find_last_slash(buf);
	if (base)
		*base = '\0';
	else
		strncpy(buf, ".", size - 1);
}

static uint32_t map_gamepad_button(unsigned device, unsigned id)
{
	static const uint32_t dc_joymap[] =
	{
			/* JOYPAD_B      */ DC_BTN_A,
			/* JOYPAD_Y      */ DC_BTN_X,
			/* JOYPAD_SELECT */ DC_BTN_D,
			/* JOYPAD_START  */ DC_BTN_START,
			/* JOYPAD_UP     */ DC_DPAD_UP,
			/* JOYPAD_DOWN   */ DC_DPAD_DOWN,
			/* JOYPAD_LEFT   */ DC_DPAD_LEFT,
			/* JOYPAD_RIGHT  */ DC_DPAD_RIGHT,
			/* JOYPAD_A      */ DC_BTN_B,
			/* JOYPAD_X      */ DC_BTN_Y,
			/* JOYPAD_L      */ DC_BTN_C,
			/* JOYPAD_R      */ DC_BTN_Z,
	};

	static const uint32_t dc_lg_joymap[] =
	{
			/* deprecated */ 		0,
			/* deprecated */ 		0,
			/* LIGHTGUN_TRIGGER */	DC_BTN_A,
			/* LIGHTGUN_AUX_A */	DC_BTN_B,
			/* LIGHTGUN_AUX_B */ 	0,
			/* deprecated */ 		0,
			/* LIGHTGUN_START */	DC_BTN_START,
			/* LIGHTGUN_SELECT */ 	0,
			/* LIGHTGUN_AUX_C */	0,
			/* LIGHTGUN_UP   */ 	DC_DPAD_UP,
			/* LIGHTGUN_DOWN   */ 	DC_DPAD_DOWN,
			/* LIGHTGUN_LEFT   */ 	DC_DPAD_LEFT,
			/* LIGHTGUN_RIGHT  */ 	DC_DPAD_RIGHT,
	};

	static const uint32_t aw_joymap[] =
	{
			/* JOYPAD_B      */ AWAVE_BTN0_KEY, /* BTN1 */
			/* JOYPAD_Y      */ AWAVE_BTN2_KEY, /* BTN3 */
			/* JOYPAD_SELECT */ AWAVE_COIN_KEY,
			/* JOYPAD_START  */ AWAVE_START_KEY,
			/* JOYPAD_UP     */ AWAVE_UP_KEY,
			/* JOYPAD_DOWN   */ AWAVE_DOWN_KEY,
			/* JOYPAD_LEFT   */ AWAVE_LEFT_KEY,
			/* JOYPAD_RIGHT  */ AWAVE_RIGHT_KEY,
			/* JOYPAD_A      */ AWAVE_BTN1_KEY, /* BTN2 */
			/* JOYPAD_X      */ AWAVE_BTN3_KEY, /* BTN4 */
			/* JOYPAD_L      */ 0,
			/* JOYPAD_R      */ AWAVE_BTN4_KEY, /* BTN5 */
			/* JOYPAD_L2     */ 0,
			/* JOYPAD_R2     */ 0,
			/* JOYPAD_L3     */ AWAVE_TEST_KEY,
			/* JOYPAD_R3     */ AWAVE_SERVICE_KEY,
	};

	static const uint32_t aw_lg_joymap[] =
	{
			/* deprecated */ 		0,
			/* deprecated */ 		0,
			/* LIGHTGUN_TRIGGER */	AWAVE_TRIGGER_KEY,
			/* LIGHTGUN_AUX_A */	AWAVE_BTN0_KEY,
			/* LIGHTGUN_AUX_B */ 	AWAVE_BTN1_KEY,
			/* deprecated */ 		0,
			/* LIGHTGUN_START */	AWAVE_START_KEY,
			/* LIGHTGUN_SELECT */ 	AWAVE_COIN_KEY,
			/* LIGHTGUN_AUX_C */	AWAVE_BTN2_KEY,
			/* LIGHTGUN_UP   */ 	AWAVE_UP_KEY,
			/* LIGHTGUN_DOWN   */ 	AWAVE_DOWN_KEY,
			/* LIGHTGUN_LEFT   */ 	AWAVE_LEFT_KEY,
			/* LIGHTGUN_RIGHT  */ 	AWAVE_RIGHT_KEY,
	};

	static const uint32_t nao_joymap[] =
	{
			/* JOYPAD_B      */ NAOMI_BTN0_KEY, /* BTN1 */
			/* JOYPAD_Y      */ NAOMI_BTN2_KEY, /* BTN3 */
			/* JOYPAD_SELECT */ NAOMI_COIN_KEY,
			/* JOYPAD_START  */ NAOMI_START_KEY,
			/* JOYPAD_UP     */ NAOMI_UP_KEY,
			/* JOYPAD_DOWN   */ NAOMI_DOWN_KEY,
			/* JOYPAD_LEFT   */ NAOMI_LEFT_KEY,
			/* JOYPAD_RIGHT  */ NAOMI_RIGHT_KEY,
			/* JOYPAD_A      */ NAOMI_BTN1_KEY, /* BTN2 */
			/* JOYPAD_X      */ NAOMI_BTN3_KEY, /* BTN4 */
			/* JOYPAD_L      */ NAOMI_BTN5_KEY, /* BTN6 */
			/* JOYPAD_R      */ NAOMI_BTN4_KEY, /* BTN5 */
			/* JOYPAD_L2     */ NAOMI_BTN7_KEY, /* BTN8 */
			/* JOYPAD_R2     */ NAOMI_BTN6_KEY, /* BTN7 */
			/* JOYPAD_L3     */ NAOMI_TEST_KEY,
			/* JOYPAD_R3     */ NAOMI_SERVICE_KEY,
	};

	static const uint32_t nao_lg_joymap[] =
	{
			/* deprecated */ 		0,
			/* deprecated */ 		0,
			/* LIGHTGUN_TRIGGER */	NAOMI_BTN0_KEY,
			/* LIGHTGUN_AUX_A */	NAOMI_BTN1_KEY,
			/* LIGHTGUN_AUX_B */ 	NAOMI_BTN2_KEY,
			/* deprecated */ 		0,
			/* LIGHTGUN_START */	NAOMI_START_KEY,
			/* LIGHTGUN_SELECT */ 	NAOMI_COIN_KEY,
			/* LIGHTGUN_AUX_C */	NAOMI_BTN3_KEY,
			/* LIGHTGUN_UP   */ 	NAOMI_UP_KEY,
			/* LIGHTGUN_DOWN   */ 	NAOMI_DOWN_KEY,
			/* LIGHTGUN_LEFT   */ 	NAOMI_LEFT_KEY,
			/* LIGHTGUN_RIGHT  */ 	NAOMI_RIGHT_KEY,
	};

	static const uint32_t systemsp_joymap[] =
	{
			/* JOYPAD_B      */ DC_BTN_A,
			/* JOYPAD_Y      */ DC_BTN_C,
			/* JOYPAD_SELECT */ DC_BTN_D,		// coin
			/* JOYPAD_START  */ DC_BTN_START,
			/* JOYPAD_UP     */ DC_DPAD_UP,
			/* JOYPAD_DOWN   */ DC_DPAD_DOWN,
			/* JOYPAD_LEFT   */ DC_DPAD_LEFT,
			/* JOYPAD_RIGHT  */ DC_DPAD_RIGHT,
			/* JOYPAD_A      */ DC_BTN_B,
			/* JOYPAD_X      */ 0,
			/* JOYPAD_L      */ 0,
			/* JOYPAD_R      */ 0,
			/* JOYPAD_L2     */ 0,
			/* JOYPAD_R2     */ 0,
			/* JOYPAD_L3     */ DC_DPAD2_DOWN,	// test
			/* JOYPAD_R3     */ DC_DPAD2_UP,	// service
	};

	const uint32_t *joymap;
	size_t joymap_size;

	switch (settings.platform.system)
	{
	case DC_PLATFORM_DREAMCAST:
	case DC_PLATFORM_DEV_UNIT:
		switch (device)
		{
		case RETRO_DEVICE_JOYPAD:
		case RETRO_DEVICE_POINTER:
			joymap = dc_joymap;
			joymap_size = std::size(dc_joymap);
			break;
		case RETRO_DEVICE_LIGHTGUN:
			joymap = dc_lg_joymap;
			joymap_size = std::size(dc_lg_joymap);
			break;
		default:
			return 0;
		}
		break;

	case DC_PLATFORM_NAOMI:
	case DC_PLATFORM_NAOMI2:
		switch (device)
		{
		case RETRO_DEVICE_JOYPAD:
		case RETRO_DEVICE_POINTER:
			joymap = nao_joymap;
			joymap_size = std::size(nao_joymap);
			break;
		case RETRO_DEVICE_LIGHTGUN:
			joymap = nao_lg_joymap;
			joymap_size = std::size(nao_lg_joymap);
			break;
		default:
			return 0;
		}
		break;

	case DC_PLATFORM_ATOMISWAVE:
		switch (device)
		{
		case RETRO_DEVICE_JOYPAD:
		case RETRO_DEVICE_POINTER:
			joymap = aw_joymap;
			joymap_size = std::size(aw_joymap);
			break;
		case RETRO_DEVICE_LIGHTGUN:
			joymap = aw_lg_joymap;
			joymap_size = std::size(aw_lg_joymap);
			break;
		default:
			return 0;
		}
		break;

	case DC_PLATFORM_SYSTEMSP:
		joymap = systemsp_joymap;
		joymap_size = std::size(systemsp_joymap);
		break;

	default:
		return 0;
	}

	if (id >= joymap_size)
		return 0;
	uint32_t mapped = joymap[id];
	// Hack to bind Button 9 instead of Service when not used
	if (id == RETRO_DEVICE_ID_JOYPAD_R3 && device == RETRO_DEVICE_JOYPAD
			&& settings.platform.isNaomi()
			&& !allow_service_buttons)
		mapped = NAOMI_BTN8_KEY;
	return mapped;
}

static const char *get_button_name(unsigned device, unsigned id, const char *default_name)
{
	if (NaomiGameInputs == NULL)
		return default_name;
	uint32_t mask = map_gamepad_button(device, id);
	if (mask == 0)
		return NULL;
	for (int i = 0; NaomiGameInputs->buttons[i].source != 0; i++)
		if (NaomiGameInputs->buttons[i].source == mask)
		{
			if (NaomiGameInputs->buttons[i].name[0] != '\0')
				return NaomiGameInputs->buttons[i].name;
			else
				return default_name;
		}
	return NULL;
}

static const char *get_axis_name(unsigned index, const char *default_name)
{
	if (NaomiGameInputs == NULL)
		return default_name;
	for (int i = 0; NaomiGameInputs->axes[i].name != NULL; i++)
		if (NaomiGameInputs->axes[i].axis == index)
		{
			if (NaomiGameInputs->axes[i].name[0] != '\0')
				return NaomiGameInputs->axes[i].name;
			else
				return default_name;
		}

	return NULL;
}

static void set_input_descriptors()
{
	struct retro_input_descriptor desc[100];
	int descriptor_index = 0;
	if (settings.platform.isArcade())
	{
		const char *name;

		for (unsigned i = 0; i < MAPLE_PORTS; i++)
		{
			switch (config::MapleMainDevices[i])
			{
			case MDT_LightGun:
				name = get_button_name(RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_DPAD_LEFT, "D-Pad Left");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_DPAD_LEFT, name };
				name = get_button_name(RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_DPAD_UP, "D-Pad Up");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_DPAD_UP, name };
				name = get_button_name(RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_DPAD_DOWN, "D-Pad Down");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_DPAD_DOWN, name };
				name = get_button_name(RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_DPAD_RIGHT, "D-Pad Right") ;
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_DPAD_RIGHT, name };
				name = get_button_name(RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_TRIGGER, "Trigger");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_TRIGGER, name };
				name = get_button_name(RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_AUX_A, "Button 1");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_AUX_A, name };
				name = get_button_name(RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_AUX_B, "Button 2");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_AUX_B, name };
				name = get_button_name(RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_AUX_C, "Button 3");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_AUX_C, name };
				desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_RELOAD, "Reload" };
				name = get_button_name(RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_SELECT, "Coin");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_SELECT, name };
				name = get_button_name(RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_START, "Start");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_START, name };
				break;

			case MDT_SegaController:
				name = get_button_name(RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_LEFT, "D-Pad Left");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, name };
				name = get_button_name(RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_UP, "D-Pad Up");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, name };
				name = get_button_name(RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_DOWN, "D-Pad Down");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  name };
				name = get_button_name(RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, name };
				name = get_button_name(RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_B, "Button 1");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, name };
				name = get_button_name(RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_A, "Button 2");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, name };
				name = get_button_name(RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_Y, "Button 3");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, name };
				name = get_button_name(RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_X, "Button 4");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, name };
				name = get_button_name(RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_R, "Button 5");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, name };
				name = haveCardReader ? "Insert Card" : get_button_name(RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_L, "Button 6");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, name };
				name = get_button_name(RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_R2, "Button 7");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2, name };
				name = get_button_name(RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_L2, "Button 8");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2, name };
				name = get_button_name(RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_START, "Start");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, name };
				name = get_button_name(RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_SELECT, "Coin");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, name };
				name = get_button_name(RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_L3, "Test");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3, name };
				name = get_button_name(RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_R3, "Service");
				if (name != NULL)
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3, name };
				name = get_axis_name(0, "Axis 1");
				if (name != NULL && name[0] != '\0')
					desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_X, name };
				name = get_axis_name(1, "Axis 2");
				if (name != NULL && name[0] != '\0')
					desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_Y, name };
				name = get_axis_name(2, "Axis 3");
				if (name != NULL && name[0] != '\0')
					desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_X, name };
				name = get_axis_name(3, "Axis 4");
				if (name != NULL && name[0] != '\0')
					desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_Y, name };
				name = get_axis_name(4, NULL);
				if (name != NULL && name[0] != '\0')
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2, name };
				name = get_axis_name(5, NULL);
				if (name != NULL && name[0] != '\0')
					desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2, name };
				break;

			default:
				break;
			}
		}
	}
	else
	{
		for (unsigned i = 0; i < MAPLE_PORTS; i++)
		{
			switch (config::MapleMainDevices[i])
			{
			case MDT_SegaController:
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "A" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "B" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Y" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "X" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "L Trigger" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,    "R Trigger" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_X, "Analog X" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_Y, "Analog Y" };
				break;

			case MDT_TwinStick:
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,   "L-Stick Left" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,     "L-Stick Up" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,   "L-Stick Down" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT,  "L-Stick Right" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,      "R-Stick Down" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,      "R-Stick Right" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,      "R-Stick Up" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,      "R-Stick Left" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,      "L Turbo" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,      "R Turbo" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,     "L Trigger" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,     "R Trigger" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,  "Start" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Special" };
				break;

			case MDT_AsciiStick:
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "Stick Left" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "Stick Up" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "Stick Down" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Stick Right" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "A" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "B" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Y" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "X" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "C" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "Z" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" };
				break;

			case MDT_LightGun:
				desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_DPAD_LEFT,  "D-Pad Left" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_DPAD_UP,    "D-Pad Up" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_DPAD_DOWN,  "D-Pad Down" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_DPAD_RIGHT, "D-Pad Right" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_TRIGGER,	   "A" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_START,      "Start" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_AUX_A,      "B" };
				break;

			case MDT_MaracasController:
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "A (R-Shake)" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "B (L-Shake)" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "C (L-Button)" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "D (R-Lost)" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "Z (R-Lost)" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start (R-Button)" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,  RETRO_DEVICE_ID_ANALOG_X, "Maraca 1 X pos." };
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,  RETRO_DEVICE_ID_ANALOG_Y, "Maraca 1 Y pos." };
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_X, "Maraca 2 X pos." };
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_Y, "Maraca 2 Y pos." };
				break;

			case MDT_FishingController:
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "A" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "B" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "X" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Y" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,    "Reel handle output" };                       // A1: Analog lever
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "Acc. sensor Z" };                            // A2: Acc. sensor Z
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,  RETRO_DEVICE_ID_ANALOG_X, "Analog X (L-R)" }; // A3: Analog key
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,  RETRO_DEVICE_ID_ANALOG_Y, "Analog Y (U-D)" }; // A4: Analog key
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_X, "Acc. sensor X" };  // A5: Acc. sensor X
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_Y, "Acc. sensor Y" };  // A6: Acc. sensor Y
				break;

			case MDT_PopnMusicController:
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "A" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "B" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "C" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "E" }; // A
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "F" }; // X
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "G" }; // B
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "H" }; // Y
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "I" }; // C
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" };
				break;

			case MDT_RacingController:
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "+" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "-" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "A" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "B" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,    "R-Axis" };                                // A1: Analog lever
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "L-Axis" };                                // A2: Analog lever
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,  RETRO_DEVICE_ID_ANALOG_X, "Wheel (L-R)" }; // A3: Analog key, also La, Ra
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_X, "Accelerator" }; // A5: Accelerator?
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_Y, "Brake" };       // A6: Brake?
				break;

			case MDT_DenshaDeGoController:
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Brake bit 3" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "Brake bit 2" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "Brake bit 1" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "Brake bit 0" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Master Control bit 2" }; // X
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Master Control bit 1" }; // Y
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "Master Control bit 0" }; // Z
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "A" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "B" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "C" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "D" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" };
				break;

			case MDT_SegaControllerXL:
				// No DPad2
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "A" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "B" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Y" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "X" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "C" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "Z" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,"D" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "L Trigger" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,    "R Trigger" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_X, "Analog X" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_Y, "Analog Y" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_X, "R. Analog X" };
				desc[descriptor_index++] = { i, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_Y, "R. Analog Y" };
				break;

			default:
				break;
			}
		}
	}
	desc[descriptor_index++] = { 0 };

	environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);
}

static void extract_basename(char *buf, const char *path, size_t size)
{
	const char *base = find_last_slash(path);
	if (!base)
		base = path;
	else
		base++;

	strncpy(buf, base, size - 1);
	buf[size - 1] = '\0';
}

static void remove_extension(char *buf, const char *path, size_t size)
{
	char *base;
	strncpy(buf, path, size - 1);
	buf[size - 1] = '\0';

	base = strrchr(buf, '.');

	if (base)
		*base = '\0';
}

#ifdef HAVE_VULKAN
static VulkanContext theVulkanContext;

static void retro_vk_context_reset()
{
	NOTICE_LOG(RENDERER, "retro_vk_context_reset");
	retro_hw_render_interface* vulkan;
	if (!environ_cb(RETRO_ENVIRONMENT_GET_HW_RENDER_INTERFACE, (void**)&vulkan) || !vulkan)
	{
		ERROR_LOG(RENDERER, "Get Vulkan HW interface failed");
		return;
	}
	if (!theVulkanContext.init((retro_hw_render_interface_vulkan *)vulkan))
		return;
	rend_term_renderer();
	rend_init_renderer();
	if (!perPixelChecked)
		check_per_pixel_opt();
}

static void retro_vk_context_destroy()
{
	NOTICE_LOG(RENDERER, "retro_vk_context_destroy");
	rend_term_renderer();
	theVulkanContext.term();
}

static bool set_vulkan_hw_render()
{
	retro_hw_render_callback hw_render{};
	hw_render.context_type = RETRO_HW_CONTEXT_VULKAN;
	hw_render.version_major = VK_API_VERSION_1_0;
	hw_render.version_minor = 0;
	hw_render.context_reset = retro_vk_context_reset;
	hw_render.context_destroy = retro_vk_context_destroy;
	hw_render.debug_context = false;

	if (!environ_cb(RETRO_ENVIRONMENT_SET_HW_RENDER, &hw_render))
		return false;

	static const struct retro_hw_render_context_negotiation_interface_vulkan negotiation_interface = {
			RETRO_HW_RENDER_CONTEXT_NEGOTIATION_INTERFACE_VULKAN,
			RETRO_HW_RENDER_CONTEXT_NEGOTIATION_INTERFACE_VULKAN_VERSION,
			VkGetApplicationInfo,
			VkCreateDevice,
			nullptr,
	};
	environ_cb(RETRO_ENVIRONMENT_SET_HW_RENDER_CONTEXT_NEGOTIATION_INTERFACE, (void *)&negotiation_interface);

	if (config::RendererType == RenderType::OpenGL_OIT || config::RendererType == RenderType::DirectX11_OIT)
		config::RendererType = RenderType::Vulkan_OIT;
	else if (config::RendererType != RenderType::Vulkan_OIT)
		config::RendererType = RenderType::Vulkan;
	return true;
}
#else
static bool set_vulkan_hw_render()
{
	return false;
}
#endif

static bool set_opengl_hw_render(u32 preferred)
{
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES)
	glsm_ctx_params_t params = {0};

	params.context_reset         = context_reset;
	params.context_destroy       = context_destroy;
	params.environ_cb            = environ_cb;
#if defined(TARGET_NO_STENCIL)
	params.stencil               = false;
#else
	params.stencil               = true;
#endif
	params.imm_vbo_draw          = NULL;
	params.imm_vbo_disable       = NULL;
#if defined(__APPLE__) && defined(HAVE_OPENGL)
	preferred = RETRO_HW_CONTEXT_OPENGL_CORE;
#endif
#ifdef HAVE_OIT
	if (config::RendererType == RenderType::OpenGL_OIT || config::RendererType == RenderType::DirectX11_OIT || config::RendererType == RenderType::Vulkan_OIT)
	{
		config::RendererType = RenderType::OpenGL_OIT;
#ifndef HAVE_OPENGLES
		params.context_type = (retro_hw_context_type)preferred;
		if (preferred == RETRO_HW_CONTEXT_OPENGL)
		{
			// There are some weirdness with RA's gl context's versioning :
			// - any value above 3.0 won't provide a valid context, while the GLSM_CTL_STATE_CONTEXT_INIT call returns true...
			// - the only way to overwrite previously set version with zero values is to set them directly in hw_render, otherwise they are ignored (see glsm_state_ctx_init logic)
			// FIXME what's the point of this?
			//retro_hw_render_callback hw_render;
			//hw_render.version_major = 3;
			//hw_render.version_minor = 0;
		}
		else
		{
			params.major = 4;
			params.minor = 3;
		}
#endif
	}
	else
#endif
	{
		/* WRC - context_type was previously only set inside
		 * #ifndef HAVE_OPENGLES, meaning on any GLES-target build (required
		 * for WASM/browser targets) the "preferred" argument passed in from
		 * GET_PREFERRED_HW_RENDER was silently discarded and params.context_type
		 * stayed at its zero-initialized value - flycast could never actually
		 * request GLES3 regardless of what the frontend reported as
		 * preferred, confirmed via console log ("Requesting OpenGLES2
		 * context" even after fixing RetroArch's own GET_PREFERRED_HW_RENDER
		 * to correctly report RETRO_HW_CONTEXT_OPENGLES3). major/minor stay
		 * guarded since those specific desktop-GL version numbers don't
		 * apply to the GLES path - context_type (ES2 vs ES3) already
		 * conveys the distinction there.
		 */
		params.context_type          = (retro_hw_context_type)preferred;
#ifndef HAVE_OPENGLES
		params.major                 = 3;
		params.minor                 = preferred == RETRO_HW_CONTEXT_OPENGL_CORE ? 2 : 0;
#endif
		config::RendererType = RenderType::OpenGL;
	}

	if (glsm_ctl(GLSM_CTL_STATE_CONTEXT_INIT, &params))
		return true;

#if defined(HAVE_GL3)
	params.context_type       = (retro_hw_context_type)preferred;
	params.major              = 3;
	params.minor              = 0;
#else
	params.context_type       = (retro_hw_context_type)preferred;
	params.major              = 0;
	params.minor              = 0;
#endif
	config::RendererType = RenderType::OpenGL;
	return glsm_ctl(GLSM_CTL_STATE_CONTEXT_INIT, &params);
#else
	return false;
#endif
}

#ifdef HAVE_D3D11
static void dx11_context_reset()
{
	NOTICE_LOG(RENDERER, "DX11 context reset");
	retro_hw_render_interface_d3d11 *hw_render = nullptr;
	if (!environ_cb(RETRO_ENVIRONMENT_GET_HW_RENDER_INTERFACE, &hw_render) || hw_render == nullptr || hw_render->interface_type != RETRO_HW_RENDER_INTERFACE_D3D11)
		return;
	if (hw_render->interface_version != RETRO_HW_RENDER_INTERFACE_D3D11_VERSION)
	{
		WARN_LOG(RENDERER, "Unsupported interface version %d, expecting %d", hw_render->interface_version, RETRO_HW_RENDER_INTERFACE_D3D11_VERSION);
		return;
	}
	rend_term_renderer();
	theDX11Context.term();

	theDX11Context.init(hw_render->device, hw_render->context, hw_render->D3DCompile, hw_render->featureLevel);
	if (config::RendererType == RenderType::OpenGL_OIT || config::RendererType == RenderType::Vulkan_OIT)
		config::RendererType = RenderType::DirectX11_OIT;
	else if (config::RendererType != RenderType::DirectX11_OIT)
		config::RendererType = RenderType::DirectX11;
	rend_init_renderer();
	if (!perPixelChecked)
		check_per_pixel_opt();
}

static void dx11_context_destroy()
{
	NOTICE_LOG(RENDERER, "DX11 context destroyed");
	rend_term_renderer();
	theDX11Context.term();
}
#endif

static bool set_dx11_hw_render()
{
#ifdef HAVE_D3D11
	retro_hw_render_callback hw_render_{};
	hw_render_.context_type = RETRO_HW_CONTEXT_DIRECT3D;
	hw_render_.version_major = 11;
	hw_render_.version_minor = 0;
	hw_render_.context_reset = dx11_context_reset;
	hw_render_.context_destroy = dx11_context_destroy;

	if (!environ_cb(RETRO_ENVIRONMENT_SET_HW_RENDER, &hw_render_))
	{
		WARN_LOG(RENDERER, "DX11 hardware rendering not available");
		return false;
	}
	return true;
#else
	return false;
#endif
}

// Loading/unloading games
bool retro_load_game(const struct retro_game_info *game)
{
#if defined(IOS)
	bool can_jit;
	if (environ_cb(RETRO_ENVIRONMENT_GET_JIT_CAPABLE, &can_jit) && !can_jit) {
		// jit is required both for performance and for audio. trying to run
		// without the jit will cause a crash.
		os_notify(i18n::T("Cannot run without JIT"), 5000);
		return false;
	}
#endif

	bool boot_to_bios = false;
	if (game != nullptr && game->path != nullptr && game->path[0] != '\0')
	{
		NOTICE_LOG(BOOT, "retro_load_game: %s", game->path);

		extract_basename(g_base_name, game->path, sizeof(g_base_name));
		extract_directory(game_dir, game->path, sizeof(game_dir));

		// Storing rom dir for later use
		snprintf(g_roms_dir, sizeof(g_roms_dir), "%s%c", game_dir, slash);
	}
	else
	{
		NOTICE_LOG(BOOT, "retro_load_game: (no content)");
		g_base_name[0] = '\0';
		game_dir[0] = '\0';
		g_roms_dir[0] = '\0';
		settings.platform.system = DC_PLATFORM_DREAMCAST;
		boot_to_bios = true;
	}
	if (environ_cb(RETRO_ENVIRONMENT_GET_RUMBLE_INTERFACE, &rumble) && log_cb)
		log_cb(RETRO_LOG_DEBUG, "Rumble interface supported!\n");

	const char *dir = NULL;
	if (!(environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &dir) && dir))
		dir = game_dir;

	snprintf(game_dir, sizeof(game_dir), "%s%cdc%c", dir, slash, slash);
	snprintf(game_dir_no_slash, sizeof(game_dir_no_slash), "%s%cdc", dir, slash);

	// Per-content VMU additions START
	// > Get save directory
	const char *vmu_dir = NULL;
	if (!(environ_cb(RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY, &vmu_dir) && vmu_dir))
		vmu_dir = game_dir;

	snprintf(vmu_dir_no_slash, sizeof(vmu_dir_no_slash), "%s", vmu_dir);

	// > Get content name
	remove_extension(content_name, g_base_name, sizeof(content_name));

	if (content_name[0] == '\0')
		snprintf(content_name, sizeof(content_name), "vmu_save");
	// Per-content VMU additions END

	update_variables(true);

	char *ext = strrchr(g_base_name, '.');

	{
		/* Check for extension .lst, .bin, .dat or .zip. If found, we will set the system type
		 * automatically to Naomi or AtomisWave. */
		if (ext)
		{
			log_cb(RETRO_LOG_INFO, "File extension is: %s\n", ext);
			if (!strcmp(".lst", ext)
					|| !strcmp(".bin", ext) || !strcmp(".BIN", ext)
					|| !strcmp(".dat", ext) || !strcmp(".DAT", ext)
					|| !strcmp(".zip", ext) || !strcmp(".ZIP", ext)
					|| !strcmp(".7z", ext) || !strcmp(".7Z", ext))
			{
				settings.platform.system = naomi_cart_GetPlatform(game->path);
				// Users should use the superior format instead, let's warn them
				if (!strcmp(".lst", ext)
						|| !strcmp(".bin", ext) || !strcmp(".BIN", ext)
						|| !strcmp(".dat", ext) || !strcmp(".DAT", ext))
				{
					struct retro_message msg;
					// Sadly, this callback is only able to display short messages, so we can't give proper explanations...
					msg.msg = "Please upgrade to MAME romsets or expect issues";
					msg.frames = 1200;
					environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, &msg);
				}
			}
			// If m3u playlist found load the paths into array
			else if (!strcmp(".m3u", ext) || !strcmp(".M3U", ext))
			{
				if (!read_m3u(game->path))
				{
					if (log_cb)
						log_cb(RETRO_LOG_ERROR, "%s\n", "[libretro]: failed to read m3u file ...\n");
					return false;
				}
			}
		}
	}

	if (boot_to_bios) {
		game_data.clear();
	}
	// if an m3u file was loaded, disk_paths will already be populated so load the game from there
	else if (disk_paths.size() > 0)
	{
		disk_index = 0;

		// Attempt to set initial disk index
		if (disk_paths.size() > 1
				&& disk_initial_index > 0
				&& disk_initial_index < disk_paths.size()
				&& disk_paths[disk_initial_index].compare(disk_initial_path) == 0)
			disk_index = disk_initial_index;

		game_data = disk_paths[disk_index];
	}
	else
	{
		char disk_label[PATH_MAX];
		disk_label[0] = '\0';

		disk_paths.push_back(game->path);

		fill_short_pathname_representation(disk_label, game->path, sizeof(disk_label));
		disk_labels.push_back(disk_label);

		game_data = game->path;
	}

	{
		char data_dir[1024];

		snprintf(data_dir, sizeof(data_dir), "%s%s", game_dir, "data");

		INFO_LOG(COMMON, "Creating dir: %s", data_dir);
		struct stat buf;
		if (stat(data_dir, &buf) < 0)
		{
			path_mkdir(data_dir);
		}
	}

	u32 preferred;
	if (!environ_cb(RETRO_ENVIRONMENT_GET_PREFERRED_HW_RENDER, &preferred))
		preferred = RETRO_HW_CONTEXT_DUMMY;
	bool foundRenderApi = false;

	if (preferred == RETRO_HW_CONTEXT_OPENGL || preferred == RETRO_HW_CONTEXT_OPENGL_CORE
			|| preferred == RETRO_HW_CONTEXT_OPENGLES2 || preferred == RETRO_HW_CONTEXT_OPENGLES3
			|| preferred == RETRO_HW_CONTEXT_OPENGLES_VERSION)
	{
		foundRenderApi = set_opengl_hw_render(preferred);
	}
	else if (preferred == RETRO_HW_CONTEXT_VULKAN)
	{
		foundRenderApi = set_vulkan_hw_render();
	}
	else if (preferred == RETRO_HW_CONTEXT_DIRECT3D)
	{
		foundRenderApi = set_dx11_hw_render();
	}
	else
	{
		// fallback when not supported (or auto-switching disabled), let's try all supported drivers
		foundRenderApi = set_dx11_hw_render();
		if (!foundRenderApi)
			foundRenderApi = set_vulkan_hw_render();
#if defined(HAVE_OPENGLES)
		if (!foundRenderApi)
			foundRenderApi = set_opengl_hw_render(RETRO_HW_CONTEXT_OPENGLES3);
		if (!foundRenderApi)
			foundRenderApi = set_opengl_hw_render(RETRO_HW_CONTEXT_OPENGLES2);
#else
		if (!foundRenderApi)
			foundRenderApi = set_opengl_hw_render(RETRO_HW_CONTEXT_OPENGL_CORE);
		if (!foundRenderApi)
			foundRenderApi = set_opengl_hw_render(RETRO_HW_CONTEXT_OPENGL);
#endif
	}

	if (!foundRenderApi)
		return false;

	if (settings.platform.isArcade())
	{
		if (environ_cb(RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY, &dir)
				&& dir != nullptr
				&& strcmp(dir, g_roms_dir) != 0)
		{
			static char save_dir[PATH_MAX];
			snprintf(save_dir, sizeof(save_dir), "%s%creicast%c", dir, slash, slash);

			struct stat buf;
			if (stat(save_dir, &buf) < 0)
			{
				DEBUG_LOG(BOOT, "Creating dir: %s", save_dir);
				path_mkdir(save_dir);
			}
			arcadeFlashPath = std::string(save_dir) + g_base_name;
		} else {
			arcadeFlashPath = std::string(g_roms_dir) + g_base_name;
		}
		INFO_LOG(BOOT, "Setting flash base path to %s", arcadeFlashPath.c_str());
	}

	config::ScreenStretching = 100;
	if (!loadGame())
		return false;

	rotate_game = config::Rotate90;
	if (rotate_game)
		config::Widescreen.override(false);
	config::Rotate90 = false;	// actual framebuffer rotation is done by frontend

	setRotation();

	haveCardReader = card_reader::readerAvailable();
	dreampotato::update();
	refresh_devices(true);

	// System may have changed - have to update hidden core options
	set_variable_visibility();

	return true;
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info)
{
	return false;
}

void retro_unload_game()
{
	INFO_LOG(COMMON, "Flycast unloading game");
	emu.unloadGame();
	dreampotato::term();
	game_data.clear();
	disk_paths.clear();
	disk_labels.clear();
	blankVmus();
}


// Memory/Serialization
void *retro_get_memory_data(unsigned type)
{
   if (type == RETRO_MEMORY_SYSTEM_RAM)
      return &mem_b[0];
   return nullptr;
}

size_t retro_get_memory_size(unsigned type)
{
   if (type == RETRO_MEMORY_SYSTEM_RAM)
      return RAM_SIZE;
   return 0;
}

size_t retro_serialize_size()
{
	DEBUG_LOG(SAVESTATE, "retro_serialize_size");
	std::lock_guard<std::mutex> lock(mtx_serialization);

	if (!first_run)
		try {
			emu.stop();
		} catch (const FlycastException& e) {
			ERROR_LOG(COMMON, "%s", e.what());
			return 0;
		}

	Serializer ser;
	dc_serialize(ser);
	if (!first_run)
		emu.start();

	return ser.size();
}

bool retro_serialize(void *data, size_t size)
{
	DEBUG_LOG(SAVESTATE, "retro_serialize %d bytes", (int)size);
	std::lock_guard<std::mutex> lock(mtx_serialization);

	if (!first_run)
		try {
			emu.stop();
		} catch (const FlycastException& e) {
			ERROR_LOG(COMMON, "%s", e.what());
			return false;
		}
	bool result = false;
	try {
		Serializer ser(data, size);
		dc_serialize(ser);
		result = true;
	} catch (const Serializer::Exception& e) {
		ERROR_LOG(SAVESTATE, "Saving state failed: %s", e.what());
	} 

	if (!first_run)
		emu.start();

	return result;
}

bool retro_unserialize(const void * data, size_t size)
{
	DEBUG_LOG(SAVESTATE, "retro_unserialize");
	std::lock_guard<std::mutex> lock(mtx_serialization);

	if (!first_run)
		try {
			emu.stop();
		} catch (const FlycastException& e) {
			ERROR_LOG(COMMON, "%s", e.what());
			return false;
		}

	try {
		Deserializer deser(data, size);
		emu.loadstate(deser);
	    retro_audio_flush_buffer();
		if (!first_run)
			emu.start();

		return true;
	} catch (const Deserializer::Exception& e) {
		ERROR_LOG(SAVESTATE, "Loading state failed: %s", e.what());
		return false;
	}
}

// Cheats
void retro_cheat_reset()
{
   // Nothing to do here
}
void retro_cheat_set(unsigned unused, bool unused1, const char* unused2)
{
   // Nothing to do here
}


// Get info
const char* retro_get_system_directory()
{
   const char* dir;
   environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &dir);
   return dir ? dir : ".";
}

void retro_get_system_info(struct retro_system_info *info)
{
   info->library_name = "Flycast";
#ifndef GIT_VERSION
#define GIT_VERSION "undefined"
#endif
   info->library_version = GIT_VERSION;
   info->valid_extensions = "chd|cdi|elf|cue|gdi|lst|bin|dat|zip|7z|m3u";
   info->need_fullpath = true;
   info->block_extract = true;
}

void retro_get_system_av_info(retro_system_av_info *info)
{
	NOTICE_LOG(RENDERER, "retro_get_system_av_info: Res=%d", (int)config::RenderResolution);

	if (cheatManager.isWidescreen())
	{
		retro_message msg;
		msg.msg = "Widescreen cheat activated";
		msg.frames = 120;
		environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, &msg);
	}

	framebufferWidth = config::RenderResolution * 16 / 9;
	framebufferHeight = config::RenderResolution;
	maxFramebufferWidth = std::max(maxFramebufferWidth, framebufferWidth);
	maxFramebufferHeight = std::max(maxFramebufferHeight, framebufferHeight);
	setAVInfo(*info);
}

unsigned retro_get_region()
{
   return config::Broadcast == 0 ? RETRO_REGION_NTSC :  RETRO_REGION_PAL;
}

// Controller
void retro_set_controller_port_device(unsigned in_port, unsigned device)
{
	if (device_type[in_port] != (int)device && in_port < MAPLE_PORTS)
	{
		devices_need_refresh = true;
		device_type[in_port] = device;
		switch (device)
		{
			case RETRO_DEVICE_JOYPAD:
				config::MapleMainDevices[in_port] = MDT_SegaController;
				break;
			case RETRO_DEVICE_TWINSTICK:
			case RETRO_DEVICE_TWINSTICK_SATURN:
				config::MapleMainDevices[in_port] = MDT_TwinStick;
				break;
			case RETRO_DEVICE_ASCIISTICK:
				config::MapleMainDevices[in_port] = MDT_AsciiStick;
				break;
			case RETRO_DEVICE_KEYBOARD:
				config::MapleMainDevices[in_port] = MDT_Keyboard;
				break;
			case RETRO_DEVICE_MOUSE:
				config::MapleMainDevices[in_port] = MDT_Mouse;
				break;
			case RETRO_DEVICE_LIGHTGUN:
			case RETRO_DEVICE_POINTER:
				config::MapleMainDevices[in_port] = MDT_LightGun;
				break;
			case RETRO_DEVICE_MARACAS:
				config::MapleMainDevices[in_port] = MDT_MaracasController;
				break;
			case RETRO_DEVICE_FISHING:
				config::MapleMainDevices[in_port] = MDT_FishingController;
				break;
			case RETRO_DEVICE_POPNMUSIC:
				config::MapleMainDevices[in_port] = MDT_PopnMusicController;
				break;
			case RETRO_DEVICE_RACING:
				config::MapleMainDevices[in_port] = MDT_RacingController;
				break;
			case RETRO_DEVICE_DENSHA:
				config::MapleMainDevices[in_port] = MDT_DenshaDeGoController;
				break;
			case RETRO_DEVICE_FULL_CONTROLLER:
				config::MapleMainDevices[in_port] = MDT_SegaControllerXL;
				break;
			default:
				config::MapleMainDevices[in_port] = MDT_None;
				break;
		}

		// To avoid refreshing input descriptors and core options 4 times on boot,
		// let's do it only when all ports are initialized.
		if (first_run)
			for (int type : device_type)
				if (type == -1)
					return;

		set_input_descriptors();

		// To refresh the expansion slots options and their visibility
		if (settings.platform.isConsole())
			update_variables(false);
	}
}

static void refresh_devices(bool first_startup)
{
   devices_need_refresh = false;

   if (!first_startup)
   {
      if (settings.platform.isConsole())
         maple_ReconnectDevices();

      if (rumble.set_rumble_state)
      {
         for(int i = 0; i < MAPLE_PORTS; i++)
         {
            rumble.set_rumble_state(i, RETRO_RUMBLE_STRONG, 0);
            rumble.set_rumble_state(i, RETRO_RUMBLE_WEAK,   0);
         }
      }
   }
   else if (settings.platform.isConsole())
   {
      mcfg_DestroyDevices();
      mcfg_CreateDevices();
   }
}

// API version (to detect version mismatch)
unsigned retro_api_version()
{
   return RETRO_API_VERSION;
}

void retro_rend_present()
{
	if (!config::ThreadedRendering)
		is_dupe = false;
}

static void get_analog_stick( retro_input_state_t input_state_cb,
                       int player_index,
                       int stick,
                       s16* p_analog_x,
                       s16* p_analog_y )
{
   int analog_x, analog_y;
   analog_x = input_state_cb( player_index, RETRO_DEVICE_ANALOG, stick, RETRO_DEVICE_ID_ANALOG_X );
   analog_y = input_state_cb( player_index, RETRO_DEVICE_ANALOG, stick, RETRO_DEVICE_ID_ANALOG_Y );

   // Analog stick deadzone (borrowed code from parallel-n64 core)
   if ( astick_deadzone > 0 )
   {
      static const int ASTICK_MAX = 0x8000;

      // Convert cartesian coordinate analog stick to polar coordinates
      double radius = sqrt(analog_x * analog_x + analog_y * analog_y);
      double angle = atan2(analog_y, analog_x);

      if (radius > astick_deadzone)
      {
         // Re-scale analog stick range to negate deadzone (makes slow movements possible)
         radius = (radius - astick_deadzone)*((float)ASTICK_MAX/(ASTICK_MAX - astick_deadzone));

         // Convert back to cartesian coordinates
         analog_x = (int)round(radius * cos(angle));
         analog_y = (int)round(radius * sin(angle));

         // Clamp to correct range
         if (analog_x > +32767) analog_x = +32767;
         if (analog_x < -32767) analog_x = -32767;
         if (analog_y > +32767) analog_y = +32767;
         if (analog_y < -32767) analog_y = -32767;
      }
      else
      {
         analog_x = 0;
         analog_y = 0;
      }
   }

   // output
   *p_analog_x = analog_x;
   *p_analog_y = analog_y;
}

static uint16_t apply_trigger_deadzone( uint16_t input )
{
   if ( trigger_deadzone > 0 )
   {
      if ( input > trigger_deadzone )
      {
         // Re-scale analog range
         static const int TRIGGER_MAX = 0x8000;
         const float scale = ((float)TRIGGER_MAX/(float)(TRIGGER_MAX - trigger_deadzone));
         float scaled      = (input - trigger_deadzone)*scale;

         input = (int)round(scaled);
         if (input > +32767)
            input = +32767;
      }
      else
         input = 0;
   }

   return input;
}

static uint16_t get_analog_trigger(
      int16_t ret,
      retro_input_state_t input_state_cb,
      int player_index,
      int id )
{
   // NOTE: Analog triggers were added Nov 2017. Not all front-ends support this
   // feature (or pre-date it) so we need to handle this in a graceful way.

   // First, try and get an analog value using the new libretro API constant
   uint16_t trigger = input_state_cb( player_index,
                       RETRO_DEVICE_ANALOG,
                       RETRO_DEVICE_INDEX_ANALOG_BUTTON,
                       id );

   if ( trigger == 0 )
   {
      // If we got exactly zero, we're either not pressing the button, or the front-end
      // is not reporting analog values. We need to do a second check using the classic
      // digital API method, to at least get some response - better than nothing.

      // NOTE: If we're really just not holding the trigger, we're still going to get zero.

      trigger = (ret & (1 << id)) ? 0x7FFF : 0;
   }
   else
   {
      // We got something, which means the front-end can handle analog buttons.
      // So we apply a deadzone to the input and use it.

      trigger = apply_trigger_deadzone( trigger );
   }

   return trigger;
}

inline static void setDeviceButton(u32 port, uint32_t dc_key, bool is_down)
{
	if (is_down)
		kcode[port] &= ~dc_key;
	else
		kcode[port] |= dc_key;
}

static void setDeviceButtonState(u32 port, int deviceType, int btnId)
{
	uint32_t dc_key = map_gamepad_button(deviceType, btnId);
	bool is_down    = input_cb(port, deviceType, 0, btnId);
	setDeviceButton(port, dc_key, is_down);
}

static void setDeviceButtonStateFromBitmap(u32 bitmap, u32 port, int deviceType, int btnId)
{
	uint32_t dc_key = map_gamepad_button(deviceType, btnId);
	bool is_down    = bitmap & (1 << btnId);
	setDeviceButton(port, dc_key, is_down);
}

// Don't call map_gamepad_button, we supply the DC key directly.
static void setDeviceButtonStateDirect(u32 bitmap, u32 port, int btnId, uint32_t dc_key)
{
	bool is_down = bitmap & (1 << btnId);
	setDeviceButton(port, dc_key, is_down);
}

static void setDeviceButtonStateDirect2(u32 bitmap, u32 port, int btnId1, int btnId2, uint32_t dc_key)
{
	bool is_down = (bitmap & (1 << btnId1)) ||
	               (bitmap & (1 << btnId2));
	setDeviceButton(port, dc_key, is_down);
}

static void updateMouseState(u32 port)
{
	std::lock_guard<std::mutex> lock(relPosMutex);

   mo_x_delta[port] += input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X);
   mo_y_delta[port] += input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y);

   bool btn_state   = input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT);
   if (btn_state)
	  mo_buttons[port] &= ~(1 << 2);
   else
	  mo_buttons[port] |= 1 << 2;
   btn_state = input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_RIGHT);
   if (btn_state)
	  mo_buttons[port] &= ~(1 << 1);
   else
	  mo_buttons[port] |= 1 << 1;
   btn_state = input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_MIDDLE);
   if (btn_state)
	  mo_buttons[port] &= ~(1 << 3);
   else
	  mo_buttons[port] |= 1 << 3;
   if (input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_WHEELDOWN))
	  mo_wheel_delta[port] -= 10;
   else if (input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_WHEELUP))
	  mo_wheel_delta[port] += 10;
}

static void updateLightgunCoordinates(u32 port)
{
	int x;
	int y;
	if (device_type[port] == RETRO_DEVICE_LIGHTGUN)
	{
		x = input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_SCREEN_X);
		y = input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_SCREEN_Y);
	}
	else
	{
		x = input_cb(port, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X);
		y = input_cb(port, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y);
	}
	if (config::Widescreen && config::ScreenStretching == 100 && !config::EmulateFramebuffer)
		mo_x_abs[port] = 640.f * ((x + 0x8000) * 4.f / 3.f / 0x10000 - (4.f / 3.f - 1.f) / 2.f);
	else
		mo_x_abs[port] = (x + 0x8000) * 640.f / 0x10000;
	mo_y_abs[port] = (y + 0x8000) * 480.f / 0x10000;

	lightgun_params[port].offscreen = false;
	lightgun_params[port].x = mo_x_abs[port];
	lightgun_params[port].y = mo_y_abs[port];
}

void updateLightgunCoordinatesFromAnalogStick(int port)
{
	int x = input_cb(port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_X);
	mo_x_abs[port] = 320 + x * 320 / 32767;
	int y = input_cb(port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_Y);
	mo_y_abs[port] = 240 + y * 240 / 32767;

	lightgun_params[port].offscreen = false;
	lightgun_params[port].x = mo_x_abs[port];
	lightgun_params[port].y = mo_y_abs[port];
}

static void UpdateInputStateNaomi(u32 port)
{
	switch (config::MapleMainDevices[port])
	{
	case MDT_LightGun:
		if (device_type[port] == RETRO_DEVICE_LIGHTGUN)
		{
			//
			// -- buttons
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_TRIGGER);
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_AUX_A);
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_AUX_B);
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_AUX_C);
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_START);
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_SELECT);
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_DPAD_UP);
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_DPAD_DOWN);
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_DPAD_LEFT);
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_DPAD_RIGHT);

			bool force_offscreen = false;

			if (input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_RELOAD))
			{
				force_offscreen = true;
				if (settings.platform.isAtomiswave())
					kcode[port] &= ~AWAVE_TRIGGER_KEY;
				else
					kcode[port] &= ~NAOMI_BTN0_KEY;
			}

			if (force_offscreen || input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_IS_OFFSCREEN))
			{
				mo_x_abs[port] = 0;
				mo_y_abs[port] = 0;
				lightgun_params[port].offscreen = true;

				if (input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_TRIGGER) || input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_RELOAD))
				{
					if (settings.platform.isNaomi())
						kcode[port] &= ~NAOMI_BTN1_KEY;
				}
			}
			else
			{
				updateLightgunCoordinates(port);
			}
		}
		else
		{
			// RETRO_DEVICE_POINTER
			setDeviceButtonState(port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_B);
			setDeviceButtonState(port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_START);
			setDeviceButtonState(port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_UP);
			setDeviceButtonState(port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_DOWN);
			setDeviceButtonState(port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_LEFT);
			setDeviceButtonState(port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_RIGHT);

			int pressed = input_cb(port, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_PRESSED);
			int count = input_cb(port, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_COUNT);
			if (count > 1)
			{
				// reload
				mo_x_abs[port] = 0;
				mo_y_abs[port] = 0;
				lightgun_params[port].offscreen = true;
			}
			else if (count == 1)
			{
				updateLightgunCoordinates(port);
			}
			if (pressed)
			{
				if (settings.platform.isAtomiswave())
					kcode[port] &= ~AWAVE_TRIGGER_KEY;
				else
					kcode[port] &= ~NAOMI_BTN0_KEY;
			}
			else
			{
				if (settings.platform.isAtomiswave())
					kcode[port] |= AWAVE_TRIGGER_KEY;
				else
					kcode[port] |= NAOMI_BTN0_KEY;
			}
		}
		break;

	default:
		{
			//
			// -- buttons
			int16_t ret = 0;
			if (libretro_supports_bitmasks)
				ret = input_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_MASK);
			else
			{
				for (int id = RETRO_DEVICE_ID_JOYPAD_B; id <= RETRO_DEVICE_ID_JOYPAD_R3; ++id)
					if (input_cb(port, RETRO_DEVICE_JOYPAD, 0, id))
						ret |= (1 << id);
			}

			for (int id = RETRO_DEVICE_ID_JOYPAD_B; id <= RETRO_DEVICE_ID_JOYPAD_R3; ++id)
			{
				switch (id)
				{
				case RETRO_DEVICE_ID_JOYPAD_L3:
					if (allow_service_buttons)
						setDeviceButtonStateFromBitmap(ret, port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_L3);
					break;
				case RETRO_DEVICE_ID_JOYPAD_R3:
					if (settings.platform.isNaomi()
							|| allow_service_buttons)
						setDeviceButtonStateFromBitmap(ret, port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_R3);
					break;
				case RETRO_DEVICE_ID_JOYPAD_L:
					if (haveCardReader)
					{
						 if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_L))
							 card_reader::insertCard(port);
					}
					else
						setDeviceButtonStateFromBitmap(ret, port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_L);
					break;
				default:
					setDeviceButtonStateFromBitmap(ret, port, RETRO_DEVICE_JOYPAD, id);
					break;
				}
			}
			//
			// -- analog stick

			get_analog_stick(input_cb, port, RETRO_DEVICE_INDEX_ANALOG_LEFT, &joyx[port], &joyy[port] );
			get_analog_stick(input_cb, port, RETRO_DEVICE_INDEX_ANALOG_RIGHT, &joyrx[port], &joyry[port]);
			lt[port] = get_analog_trigger(ret, input_cb, port, RETRO_DEVICE_ID_JOYPAD_L2) * 2;
			rt[port] = get_analog_trigger(ret, input_cb, port, RETRO_DEVICE_ID_JOYPAD_R2) * 2;

			if (NaomiGameInputs != NULL)
			{
				for (int i = 0; NaomiGameInputs->axes[i].name != NULL; i++)
				{
					if (NaomiGameInputs->axes[i].type == Half)
					{
						/* Note:
						 * - Analog stick axes have a range of [-32768, 32767]
						 * - Analog triggers have a range of [0, 65535] */
						switch (NaomiGameInputs->axes[i].axis)
						{
						case 0:
							/* Left stick X: [-32768, 32767] */
							joyx[port] = std::max((int)joyx[port], 0) * 2;
							break;
						case 1:
							/* Left stick Y: [-32768, 32767] */
							joyy[port] = std::max((int)joyy[port], 0) * 2;
							break;
						case 2:
							/* Right stick X: [-32768, 32767] */
							joyrx[port] = std::max((int)joyrx[port], 0) * 2;
							break;
						case 3:
							/* Right stick Y: [-32768, 32767] */
							joyry[port] = std::max((int)joyry[port], 0) * 2;
						break;
							/* Case 4/5 correspond to right/left trigger.
							 * These inputs are always classified as 'Half',
							 * and already have the correct range - so no
							 * further action is required */
						}
					}
				}
			}

			// -- mouse, for rotary encoders
			updateMouseState(port);
			// lightgun with analog stick
			if (settings.input.lightgunGame)
			{
				updateLightgunCoordinatesFromAnalogStick(port);
				if (input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_RELOAD))
				{
					mo_x_abs[port] = 0;
					mo_y_abs[port] = 0;
					lightgun_params[port].offscreen = true;
					if (settings.platform.isAtomiswave())
						kcode[port] &= ~AWAVE_TRIGGER_KEY;
					else
						kcode[port] &= ~NAOMI_BTN0_KEY;
				}
				else if (settings.platform.isAtomiswave())
				{
					// map btn0 to trigger, btn1 to btn0, etc.
					u32 k = kcode[port] | (AWAVE_BTN0_KEY | AWAVE_BTN1_KEY | AWAVE_BTN2_KEY | AWAVE_BTN3_KEY | AWAVE_TRIGGER_KEY);
					if ((kcode[port] & AWAVE_BTN0_KEY) == 0)
						k &= ~AWAVE_TRIGGER_KEY;
					if ((kcode[port] & AWAVE_BTN1_KEY) == 0)
						k &= ~AWAVE_BTN0_KEY;
					if ((kcode[port] & AWAVE_BTN2_KEY) == 0)
						k &= ~AWAVE_BTN1_KEY;
					if ((kcode[port] & AWAVE_BTN3_KEY) == 0)
						k &= ~AWAVE_BTN2_KEY;
					kcode[port] = k;
				}
			}
		}
		break;
	}

	// Avoid Left+Right or Up+Down buttons being pressed together as this crashes some games
	if (settings.platform.isAtomiswave())
	{
		if ((kcode[port] & (AWAVE_UP_KEY|AWAVE_DOWN_KEY)) == 0)
			kcode[port] |= AWAVE_UP_KEY|AWAVE_DOWN_KEY;
		if ((kcode[port] & (AWAVE_LEFT_KEY|AWAVE_RIGHT_KEY)) == 0)
			kcode[port] |= AWAVE_LEFT_KEY|AWAVE_RIGHT_KEY;
	}
	else
	{
		if ((kcode[port] & (NAOMI_UP_KEY|NAOMI_DOWN_KEY)) == 0)
			kcode[port] |= NAOMI_UP_KEY|NAOMI_DOWN_KEY;
		if ((kcode[port] & (NAOMI_LEFT_KEY|NAOMI_RIGHT_KEY)) == 0)
			kcode[port] |= NAOMI_LEFT_KEY|NAOMI_RIGHT_KEY;
	}
}

static int16_t getBitmask(u32 port, int deviceType)
{
	int16_t ret = 0;
	if (libretro_supports_bitmasks)
		ret = input_cb(port, deviceType, 0, RETRO_DEVICE_ID_JOYPAD_MASK);
	else
	{
		for (int id = RETRO_DEVICE_ID_JOYPAD_B; id <= RETRO_DEVICE_ID_JOYPAD_R3; ++id)
			if (input_cb(port, deviceType, 0, id))
				ret |= (1 << id);
	}
	return ret;
}

static void UpdateInputState(u32 port)
{
	if (gl_ctx_resetting)
		return;

	if (settings.platform.isArcade())
	{
		UpdateInputStateNaomi(port);
		return;
	}
	if (rumble.set_rumble_state != NULL && vib_stop_time[port] > 0)
	{
		if (getTimeMs() >= vib_stop_time[port])
		{
			vib_stop_time[port] = 0;
			rumble.set_rumble_state(port, RETRO_RUMBLE_STRONG, 0);
		}
		else if (vib_delta[port] > 0.0)
		{
			u32 rem_time = vib_stop_time[port] - getTimeMs();
			rumble.set_rumble_state(port, RETRO_RUMBLE_STRONG, 65535 * vib_strength[port] * rem_time * vib_delta[port]);
		}
	}

	lightgun_params[port].offscreen = true;

	switch (config::MapleMainDevices[port])
	{
	case MDT_SegaController:
	case MDT_SegaControllerXL:
		{
			int16_t ret = getBitmask(port, RETRO_DEVICE_JOYPAD);

			// -- buttons
			for (int id = RETRO_DEVICE_ID_JOYPAD_B; id <= RETRO_DEVICE_ID_JOYPAD_R; ++id)
				setDeviceButtonStateFromBitmap(ret, port, RETRO_DEVICE_JOYPAD, id);

			// -- analog sticks
			get_analog_stick(input_cb, port, RETRO_DEVICE_INDEX_ANALOG_LEFT, &joyx[port], &joyy[port]);
			get_analog_stick(input_cb, port, RETRO_DEVICE_INDEX_ANALOG_RIGHT, &joyrx[port], &joyry[port]);

			// -- triggers
			if ( digital_triggers )
			{
				// -- digital left trigger
				if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_L2))
					lt[port] = 0xFFFF;
				else
					lt[port] = 0;
				// -- digital right trigger
				if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_R2))
					rt[port] = 0xFFFF;
				else
					rt[port] = 0;
			}
			else
			{
				// -- analog triggers
				lt[port] = get_analog_trigger(ret, input_cb, port, RETRO_DEVICE_ID_JOYPAD_L2 ) * 2;
				rt[port] = get_analog_trigger(ret, input_cb, port, RETRO_DEVICE_ID_JOYPAD_R2 ) * 2;
			}
		}
		break;

	case MDT_AsciiStick:
		{
			int16_t ret = getBitmask(port, RETRO_DEVICE_ASCIISTICK);
			kcode[port] = 0xFFFF; // active-low

			// stick
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_UP,    DC_DPAD_UP    );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_DOWN,  DC_DPAD_DOWN  );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_LEFT,  DC_DPAD_LEFT  );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_RIGHT, DC_DPAD_RIGHT );

			// buttons
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_B, DC_BTN_A );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_A, DC_BTN_B );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_Y, DC_BTN_X );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_X, DC_BTN_Y );
			setDeviceButtonStateDirect2(ret, port, RETRO_DEVICE_ID_JOYPAD_L, 
			                                       RETRO_DEVICE_ID_JOYPAD_L2, DC_BTN_Z );
			setDeviceButtonStateDirect2(ret, port, RETRO_DEVICE_ID_JOYPAD_R, 
			                                       RETRO_DEVICE_ID_JOYPAD_R2, DC_BTN_C );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_START, DC_BTN_START );

			// unused inputs
			lt[port]=0;
			rt[port]=0;
			joyx[port]=0;
			joyy[port]=0;
		}
		break;

	case MDT_TwinStick:
		{
			int16_t ret = 0;
			kcode[port] = 0xFFFF; // active-low
			
			if ( device_type[port] == RETRO_DEVICE_TWINSTICK_SATURN )
			{
				// NOTE: This is a remapping of the RetroPad layout in the block below to make using a real
				// Saturn Twin-Stick controller (via a USB adapter) less effort.

				// The Saturn Twin-Stick identifies as a regular Saturn controller internally but with its controls
				// wired to the two sticks without much rhyme or reason. The mapping below untangles that layout
				// into DC compatible inputs, without requiring a change for the Reicast and Beetle Saturn cores.

				// Hope that makes sense!!

				// NOTE: the dc_bits below are the same, only the retro id values have been rearranged.

				ret = getBitmask(port, RETRO_DEVICE_TWINSTICK_SATURN);

				// left-stick
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_UP,    DC_DPAD_UP    );
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_DOWN,  DC_DPAD_DOWN  );
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_LEFT,  DC_DPAD_LEFT  );
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_RIGHT, DC_DPAD_RIGHT );
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_L2,    DC_BTN_X      ); // left-trigger
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_R2,    DC_BTN_Y      ); // left-turbo

				// right-stick
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_X, DC_DPAD2_UP    );
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_L, DC_DPAD2_RIGHT );
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_A, DC_DPAD2_DOWN  );
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_Y, DC_DPAD2_LEFT  );
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_B, DC_BTN_A       ); // right-trigger
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_R, DC_BTN_B       ); // right-turbo

				// misc control
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_START,  DC_BTN_START );
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_SELECT, DC_BTN_D     );
			}
			else
			{
				int analog;

				const int thresh = 11000; // about 33%, allows for 8-way movement

				ret = getBitmask(port, RETRO_DEVICE_TWINSTICK);

				// LX
				analog = input_cb( port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_X );
				if ( analog < -thresh )
					kcode[port] &= ~DC_DPAD_LEFT;
				else if ( analog > thresh )
					kcode[port] &= ~DC_DPAD_RIGHT;
				else
				{
					// digital
					setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_LEFT,  DC_DPAD_LEFT  );
					setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_RIGHT, DC_DPAD_RIGHT );
				}

				// LY
				analog = input_cb( port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_Y );
				if ( analog < -thresh )
					kcode[port] &= ~DC_DPAD_UP;
				else if ( analog > thresh )
					kcode[port] &= ~DC_DPAD_DOWN;
				else
				{
					// digital
					setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_UP,   DC_DPAD_UP   );
					setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_DOWN, DC_DPAD_DOWN );
				}

				// RX
				analog = input_cb( port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_X );
				if ( analog < -thresh )
					kcode[port] &= ~DC_DPAD2_LEFT;
				else if ( analog > thresh )
					kcode[port] &= ~DC_DPAD2_RIGHT;
				else
				{
					// digital
					setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_Y, DC_DPAD2_LEFT  );
					setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_A, DC_DPAD2_RIGHT );
				}

				// RY
				analog = input_cb( port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_Y );
				if ( analog < -thresh )
					kcode[port] &= ~DC_DPAD2_UP;
				else if ( analog > thresh )
					kcode[port] &= ~DC_DPAD2_DOWN;
				else
				{
					// digital
					setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_X, DC_DPAD2_UP   );
					setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_B, DC_DPAD2_DOWN );
				}

				// left-stick buttons
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_L2, DC_BTN_X ); // left-trigger
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_L,  DC_BTN_Y ); // left-turbo

				// right-stick buttons
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_R2, DC_BTN_A ); // right-trigger
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_R,  DC_BTN_B ); // right-turbo

				// misc control
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_START,  DC_BTN_START );
				setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_SELECT, DC_BTN_D     );
			}

			// unused inputs
			lt[port]=0;
			rt[port]=0;
			joyx[port]=0;
			joyy[port]=0;
		}
		break;

	case MDT_LightGun:
		if (device_type[port] == RETRO_DEVICE_LIGHTGUN)
		{
			//
			// -- buttons
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_TRIGGER);
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_AUX_A);
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_START);
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_DPAD_UP);
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_DPAD_DOWN);
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_DPAD_LEFT);
			setDeviceButtonState(port, RETRO_DEVICE_LIGHTGUN, RETRO_DEVICE_ID_LIGHTGUN_DPAD_RIGHT);

			bool force_offscreen = false;

			if (input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_RELOAD))
			{
				force_offscreen = true;
				kcode[port] &= ~DC_BTN_A;
			}

			if (force_offscreen || input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_IS_OFFSCREEN))
			{
				mo_x_abs[port] = -1000;
				mo_y_abs[port] = -1000;
				lightgun_params[port].offscreen = true;

				lightgun_params[port].x = mo_x_abs[port];
				lightgun_params[port].y = mo_y_abs[port];
			}
			else
			{
				updateLightgunCoordinates(port);
			}
		}
		else
		{
			// RETRO_DEVICE_POINTER
			setDeviceButtonState(port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_B);
			setDeviceButtonState(port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_START);
			setDeviceButtonState(port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_UP);
			setDeviceButtonState(port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_DOWN);
			setDeviceButtonState(port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_LEFT);
			setDeviceButtonState(port, RETRO_DEVICE_JOYPAD, RETRO_DEVICE_ID_JOYPAD_RIGHT);

			int pressed = input_cb(port, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_PRESSED);
			int count = input_cb(port, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_COUNT);
			if (count > 1)
			{
				// reload
				mo_x_abs[port] = -1000;
				mo_y_abs[port] = -1000;
				lightgun_params[port].offscreen = true;

				lightgun_params[port].x = mo_x_abs[port];
				lightgun_params[port].y = mo_y_abs[port];
			}
			else if (count == 1)
			{
				updateLightgunCoordinates(port);
			}
			if (pressed)
				kcode[port] &= ~DC_BTN_A;
			else
				kcode[port] |= DC_BTN_A;
		}
		break;

	case MDT_Mouse:
		updateMouseState(port);
		break;

	case MDT_MaracasController:
		{
			int16_t ret = getBitmask(port, RETRO_DEVICE_MARACAS);
			kcode[port] = 0xFFFF; // active-low

			// buttons
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_B,     DC_BTN_A     ); // Right maraca shake switch
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_A,     DC_BTN_B     ); // Left  maraca shake switch
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_X,     DC_BTN_C     ); // Left  maraca button
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_L,     DC_BTN_D     ); // Left  maraca "lost" flag
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_R,     DC_BTN_Z     ); // Right maraca "lost" flag
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_START, DC_BTN_START ); // Right maraca button

			// If we wanted to apply deadzone (which we don't want for maracas), we could use:
			// get_analog_stick( input_cb, port, RETRO_DEVICE_INDEX_ANALOG_LEFT,  &(joyx [port]), &(joyy [port]) );
			// get_analog_stick( input_cb, port, RETRO_DEVICE_INDEX_ANALOG_RIGHT, &(joyrx[port]), &(joyry[port]) );
			joyx [port] = input_cb( port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,  RETRO_DEVICE_ID_ANALOG_X );
   			joyy [port] = input_cb( port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,  RETRO_DEVICE_ID_ANALOG_Y );
			joyrx[port] = input_cb( port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_X );
   			joyry[port] = input_cb( port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_Y );

			// unused inputs
			lt[port]=0;
			rt[port]=0;
		}
		break;

	case MDT_FishingController:
		{
			int16_t ret = getBitmask(port, RETRO_DEVICE_FISHING);
			kcode[port] = 0xFFFF; // active-low

			// buttons
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_B,     DC_BTN_A     );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_A,     DC_BTN_B     );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_Y,     DC_BTN_X     );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_X,     DC_BTN_Y     );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_START, DC_BTN_START );

			// analog axes
			get_analog_stick( input_cb, port, RETRO_DEVICE_INDEX_ANALOG_LEFT,  &(joyx [port]), &(joyy [port]) );                  // A3, A4: Analog keys (XY)
			joyrx[port] = input_cb( port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT,  RETRO_DEVICE_ID_ANALOG_X  );     // A5: Acc. sensor X
   			joyry[port] = input_cb( port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT,  RETRO_DEVICE_ID_ANALOG_Y  );     // A6: Acc. sensor Y
			lt   [port] = input_cb( port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_BUTTON, RETRO_DEVICE_ID_JOYPAD_L2 ) * 2; // A2: Acc. sensor Z
			rt   [port] = input_cb( port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_BUTTON, RETRO_DEVICE_ID_JOYPAD_R2 ) * 2; // A1: Analog lever
		}
		break;

	case MDT_PopnMusicController:
		{
			int16_t ret = getBitmask(port, RETRO_DEVICE_POPNMUSIC);
			kcode[port] = 0xFFFF; // active-low

			// buttons
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_LEFT,  DC_DPAD_LEFT  ); // Pop'n A
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_UP,    DC_DPAD_UP    ); // Pop'n B
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_DOWN,  DC_DPAD_DOWN  ); // Pop'n C
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_RIGHT, DC_DPAD_RIGHT ); // Pop'n D
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_B,     DC_BTN_A      ); // Pop'n E
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_Y,     DC_BTN_X      ); // Pop'n F
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_A,     DC_BTN_B      ); // Pop'n G
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_X,     DC_BTN_Y      ); // Pop'n H
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_R,     DC_BTN_C      ); // Pop'n I
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_START, DC_BTN_START  ); // Pop'n Start

			// unused inputs
			lt[port]=0;
			rt[port]=0;
			joyx[port]=0;
			joyy[port]=0;
		}
		break;

	case MDT_RacingController:
		{
			int16_t ret = getBitmask(port, RETRO_DEVICE_RACING);
			kcode[port] = 0xFFFF; // active-low

			// buttons
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_UP,    DC_DPAD_UP    );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_DOWN,  DC_DPAD_DOWN  );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_B,     DC_BTN_A      );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_A,     DC_BTN_B      );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_START, DC_BTN_START  );

			// analog axes
			get_analog_stick( input_cb, port, RETRO_DEVICE_INDEX_ANALOG_LEFT,  &(joyx [port]), &(joyy [port]) );                  // A3: Analog key;
			rt   [port] = input_cb( port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_BUTTON, RETRO_DEVICE_ID_JOYPAD_R2 ) * 2; // A1: Analog lever
			lt   [port] = input_cb( port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_BUTTON, RETRO_DEVICE_ID_JOYPAD_L2 ) * 2; // A2: Analog lever
			joyrx[port] = input_cb( port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT,  RETRO_DEVICE_ID_ANALOG_X  );     // A5: Accelerator?
   			joyry[port] = input_cb( port, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT,  RETRO_DEVICE_ID_ANALOG_Y  );     // A6: Brake?
			joyy [port] = 0;                                                                                                      // A4: unused
		}
		break;

	case MDT_DenshaDeGoController:
		{
			int16_t ret = getBitmask(port, RETRO_DEVICE_DENSHA);
			kcode[port] = 0xFFFF; // active-low

			// buttons
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_RIGHT, DC_DPAD_RIGHT );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_LEFT,  DC_DPAD_LEFT  );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_DOWN,  DC_DPAD_DOWN  );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_UP,    DC_DPAD_UP    );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_Y,     DC_BTN_X      );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_X,     DC_BTN_Y      );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_R,     DC_BTN_Z      );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_B,     DC_BTN_A      );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_A,     DC_BTN_B      );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_L,     DC_BTN_C      );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_L2,    DC_BTN_D      );
			setDeviceButtonStateDirect(ret, port, RETRO_DEVICE_ID_JOYPAD_START, DC_BTN_START  );
			// unused inputs - actually, A1, A2, A5, A6 == FFh, A3, A4 == 0, but that is already set in maple_densha_controller...
			//lt[port]=0;
			//rt[port]=0;
			//joyx[port]=0;
			//joyy[port]=0;
		}
		break;

	default:
		break;
	}
}

void os_UpdateInputState()
{
	UpdateInputState(0);
	UpdateInputState(1);
	UpdateInputState(2);
	UpdateInputState(3);
}

static void updateVibration(u32 port, float power, float inclination, u32 durationMs)
{
	if (!rumble.set_rumble_state)
		return;

	vib_strength[port] = power;

	rumble.set_rumble_state(port, RETRO_RUMBLE_STRONG, (u16)(65535 * power));
	vib_stop_time[port] = getTimeMs() + durationMs;
	vib_delta[port] = inclination;
}

u8 kb_key[4][6];	// normal keys pressed
u8 kb_shift[4];	// modifier keys pressed (bitmask)
static int kb_used;

static void release_key(unsigned dc_keycode)
{
	if (dc_keycode == 0)
		return;

	if (kb_used > 0)
	{
		for (int i = 0; i < 6; i++)
		{
			if (kb_key[0][i] == dc_keycode)
			{
				kb_used--;
				for (int j = i; j < 5; j++)
					kb_key[0][j] = kb_key[0][j + 1];
				kb_key[0][5] = 0;
			}
		}
	}
}

static void retro_keyboard_event(bool down, unsigned keycode, uint32_t character, uint16_t key_modifiers)
{
	// Dreamcast keyboard emulation
	if (keycode == RETROK_LSHIFT || keycode == RETROK_RSHIFT)
	{
		if (!down)
			kb_shift[0] &= ~(0x02 | 0x20);
		else
			kb_shift[0] |= (0x02 | 0x20);
	}
	if (keycode == RETROK_LCTRL || keycode == RETROK_RCTRL)
	{
		if (!down)
			kb_shift[0] &= ~(0x01 | 0x10);
		else
			kb_shift[0] |= (0x01 | 0x10);
	}
	// Make sure modifier keys are released
	if ((key_modifiers & RETROKMOD_SHIFT) == 0)
	{
		release_key(kb_map[RETROK_LSHIFT]);
		release_key(kb_map[RETROK_LSHIFT]);
	}
	if ((key_modifiers & RETROKMOD_CTRL) == 0)
	{
		release_key(kb_map[RETROK_LCTRL]);
		release_key(kb_map[RETROK_RCTRL]);
	}

	u8 dc_keycode = kb_map[keycode];
	if (dc_keycode != 0)
	{
		if (down)
		{
			if (kb_used < 6)
			{
				bool found = false;
				for (int i = 0; !found && i < 6; i++)
				{
					if (kb_key[0][i] == dc_keycode)
						found = true;
				}
				if (!found)
				{
					kb_key[0][kb_used] = dc_keycode;
					kb_used++;
				}
			}
		}
		else
		{
			release_key(dc_keycode);
		}
	}
}

void fatal_error(const char* text, ...)
{
	va_list args;
	char temp[2048];
	va_start(args, text);
	vsprintf(temp, text, args);
	va_end(args);
	strcat(temp, "\n");
	if (log_cb)
		log_cb(RETRO_LOG_ERROR, temp);
#ifdef __EMSCRIPTEN__
	EM_ASM({ console.error('[FATAL] ' + UTF8ToString($0)); }, temp);
#endif
}

[[noreturn]] void os_DebugBreak()
{
	ERROR_LOG(COMMON, "DEBUGBREAK!");
#ifdef __EMSCRIPTEN__
	EM_ASM({ console.error('[os_DebugBreak] called! Stack: ' + new Error().stack); });
#endif
	//exit(-1);
#ifdef __SWITCH__
	svcExitProcess();
#elif defined(__EMSCRIPTEN__)
	abort();
#else
	__builtin_trap();
#endif
}

static bool retro_set_eject_state(bool ejected)
{
	disc_tray_open = ejected;
	if (ejected)
	{
		emu.openGdrom();
		return true;
	}
	else
	{
		try {
			emu.insertGdrom(disk_paths[disk_index]);
			return true;
		} catch (const FlycastException& e) {
			ERROR_LOG(GDROM, "%s", e.what());
			return false;
		}
	}
}

static bool retro_get_eject_state()
{
	return disc_tray_open;
}

static unsigned retro_get_image_index()
{
	return disk_index;
}

static bool retro_set_image_index(unsigned index)
{
	disk_index = index;
	try {
		if (disk_index >= disk_paths.size())
		{
			// No disk in drive
			emu.insertGdrom("");
			return true;
		}

		if (disc_tray_open)
			return true;

		emu.insertGdrom(disk_paths[index]);
		return true;
	} catch (const FlycastException& e) {
		ERROR_LOG(GDROM, "%s", e.what());
		return false;
	}
}

static unsigned retro_get_num_images()
{
	return disk_paths.size();
}

static bool retro_add_image_index()
{
	disk_paths.push_back("");
	disk_labels.push_back("");

	return true;
}

static bool retro_replace_image_index(unsigned index, const struct retro_game_info *info)
{
	if (index >= disk_paths.size() || index >= disk_labels.size())
		return false;

	if (info == nullptr)
	{
		disk_paths.erase(disk_paths.begin() + index);
		disk_labels.erase(disk_labels.begin() + index);

		if (disk_index >= index && disk_index > 0)
			disk_index--;
	}
	else
	{
		char disk_label[PATH_MAX];
		disk_label[0] = '\0';

		disk_paths[index] = info->path;

		fill_short_pathname_representation(disk_label, info->path, sizeof(disk_label));
		disk_labels[index] = disk_label;
	}

	return true;
}

static bool retro_set_initial_image(unsigned index, const char *path)
{
	if (!path || *path == '\0')
		return false;

	disk_initial_index = index;
	disk_initial_path  = path;

	return true;
}

static bool retro_get_image_path(unsigned index, char *path, size_t len)
{
	if (len < 1)
		return false;

	if (index >= disk_paths.size())
		return false;

	if (disk_paths[index].empty())
		return false;

	strncpy(path, disk_paths[index].c_str(), len - 1);
	path[len - 1] = '\0';

	return true;
}

static bool retro_get_image_label(unsigned index, char *label, size_t len)
{
	if (len < 1)
		return false;

	if (index >= disk_paths.size() || index >= disk_labels.size())
		return false;

	if (disk_labels[index].empty())
		return false;

	strncpy(label, disk_labels[index].c_str(), len - 1);
	label[len - 1] = '\0';

	return true;
}

static void init_disk_control_interface()
{
	unsigned dci_version = 0;

	retro_disk_control_cb.set_eject_state     = retro_set_eject_state;
	retro_disk_control_cb.get_eject_state     = retro_get_eject_state;
	retro_disk_control_cb.set_image_index     = retro_set_image_index;
	retro_disk_control_cb.get_image_index     = retro_get_image_index;
	retro_disk_control_cb.get_num_images      = retro_get_num_images;
	retro_disk_control_cb.add_image_index     = retro_add_image_index;
	retro_disk_control_cb.replace_image_index = retro_replace_image_index;

	retro_disk_control_ext_cb.set_eject_state     = retro_set_eject_state;
	retro_disk_control_ext_cb.get_eject_state     = retro_get_eject_state;
	retro_disk_control_ext_cb.set_image_index     = retro_set_image_index;
	retro_disk_control_ext_cb.get_image_index     = retro_get_image_index;
	retro_disk_control_ext_cb.get_num_images      = retro_get_num_images;
	retro_disk_control_ext_cb.add_image_index     = retro_add_image_index;
	retro_disk_control_ext_cb.replace_image_index = retro_replace_image_index;
	retro_disk_control_ext_cb.set_initial_image   = retro_set_initial_image;
	retro_disk_control_ext_cb.get_image_path      = retro_get_image_path;
	retro_disk_control_ext_cb.get_image_label     = retro_get_image_label;

	disk_initial_index = 0;
	disk_initial_path.clear();
	if (environ_cb(RETRO_ENVIRONMENT_GET_DISK_CONTROL_INTERFACE_VERSION, &dci_version) && (dci_version >= 1))
		environ_cb(RETRO_ENVIRONMENT_SET_DISK_CONTROL_EXT_INTERFACE, &retro_disk_control_ext_cb);
	else
		environ_cb(RETRO_ENVIRONMENT_SET_DISK_CONTROL_INTERFACE, &retro_disk_control_cb);
}

static bool read_m3u(const char *file)
{
	char line[PATH_MAX];
	char name[PATH_MAX];
	FILE *f = fopen(file, "r");

	if (!f)
	{
		log_cb(RETRO_LOG_ERROR, "Could not read file\n");
		return false;
	}

	while (fgets(line, sizeof(line), f) && disk_index <= disk_paths.size())
	{
		if (line[0] == '#')
			continue;

		char *carriage_return = strchr(line, '\r');
		if (carriage_return)
			*carriage_return = '\0';

		char *newline = strchr(line, '\n');
		if (newline)
			*newline = '\0';

		// Remove any beginning and ending quotes as these can cause issues when feeding the paths into command line later
		if (line[0] == '"')
			memmove(line, line + 1, strlen(line));

		if (line[strlen(line) - 1] == '"')
			line[strlen(line) - 1]  = '\0';

		if (line[0] != '\0')
		{
			char disk_label[PATH_MAX];
			disk_label[0] = '\0';

			if (path_is_absolute(line))
				snprintf(name, sizeof(name), "%s", line);
			else
				snprintf(name, sizeof(name), "%s%s", g_roms_dir, line);
			disk_paths.push_back(name);

			fill_short_pathname_representation(disk_label, name, sizeof(disk_label));
			disk_labels.push_back(disk_label);

			disk_index++;
		}
	}

	fclose(f);
	return disk_index != 0;
}

void os_notify(const char *msg, int durationMs, const char *details)
{
	retro_message retromsg;
	retromsg.msg = msg;
	retromsg.frames = durationMs / 17;
	environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, &retromsg);
}

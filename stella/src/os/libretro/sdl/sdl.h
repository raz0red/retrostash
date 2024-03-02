#ifndef SDL_H__
#define SDL_H__
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "FrameBufferConstants.hxx"

extern bool display_ws;
extern bool display_thread_stop;
extern uint32_t display_count;
extern uint32_t display_frame_count;
extern uint8_t display_buffer_idx;
extern uint8_t display_buffer[2][480][640];
extern std::array<uInt32, kColor>* display_palette;
extern int display_width;
extern int display_height;
extern uint32_t display_start_time;
extern uint32_t display_render_count;
extern uint32_t display_skip_count;

extern SDL_Renderer* sdl_renderer;

extern int sdl_init();
extern void sdl_stop();
extern "C" void* sdl_render_thread(void* ptr);

extern int sdl_debug_init();
extern void sdl_debug_display();

#endif

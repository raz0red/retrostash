#include <pthread.h>
#include "sdl.h"

std::array<uInt32, kColor>* display_palette = 0;

bool display_ws = false;
bool display_thread_stop = false;
uint32_t display_count = 0;
uint32_t display_frame_count = 0;
uint8_t display_buffer[2][480][640] = {0};
uint8_t display_buffer_idx = 0;
int display_width = 0;
int display_height = 0;
uint32_t display_start_time = 0;
uint32_t display_render_count = 0;
uint32_t display_skip_count = 0;

static SDL_Window* sdl_window = NULL;
SDL_Renderer* sdl_renderer = NULL;
static SDL_Texture* sdl_screen_texture;
static SDL_Surface* sdl_screen_buff;

static SDL_Texture* sdl_create_blank(int width, int height);
static void sdl_flush(SDL_Surface* screen);

void sdl_stop() {
  display_thread_stop = true;
  SDL_Delay(1000);
}

int sdl_init() {
    display_start_time = SDL_GetTicks();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0) {
        printf("SDL_Init error. %s\n", SDL_GetError());
        return -1;
    }

    sdl_debug_init();

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");  // nearest-neighbor

#ifdef ATARI
    int width = 320 << 1;
    int height = 240 << 1;
#else
    int width = 1280;
    int height = 720;
#endif

    sdl_window = SDL_CreateWindow("Stella", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, width, height,
                               SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (sdl_window == NULL) {
        printf("Window could not be created. %s\n", SDL_GetError());
        return -1;
    }

    sdl_renderer = SDL_CreateRenderer(
        sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (sdl_renderer == NULL) {
        printf("Renderer could not be created. %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* tmp =
        SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0xFF000000,
                             0x00FF0000, 0x0000FF00, 0);
    if (tmp != NULL) {
        sdl_screen_buff =
            SDL_ConvertSurfaceFormat(tmp, SDL_GetWindowPixelFormat(sdl_window), 0);
        SDL_FreeSurface(tmp);
    } else {
        printf("Screen buffer could not be created. %s\n", SDL_GetError());
        return -1;
    }

    sdl_screen_texture = sdl_create_blank(width, height);
    if (sdl_screen_texture == NULL) {
        printf("Screen texture could not be created. %s\n", SDL_GetError());
        return -1;
    }

    return 0;
}

static SDL_Texture* sdl_create_blank(int width, int height) {
    SDL_Texture* newTexture;
    newTexture = SDL_CreateTexture(sdl_renderer, SDL_GetWindowPixelFormat(sdl_window),
                                   SDL_TEXTUREACCESS_STREAMING, width, height);
    if (newTexture == NULL) {
        printf("Unable to create blank texture. %s\n", SDL_GetError());
        return 0;
    }
    return newTexture;
}

static void sdl_flush(SDL_Surface* screen) {
    display_render_count++;

    SDL_UpdateTexture(sdl_screen_texture, NULL, screen->pixels, screen->pitch);
    SDL_Rect source = {0, 0, display_width /*display_width << 1*/,
                       display_height};
    SDL_Rect dest = {180, 0, 960, 720};
    if (display_ws) {
        dest = {0, 0, 1280, 720};
    }
    SDL_RenderCopy(sdl_renderer, sdl_screen_texture, &source, &dest);
    sdl_debug_display();
    SDL_RenderPresent(sdl_renderer);
}

void* sdl_render_thread(void* ptr) {
#ifdef ATARI
    sdl_init();
#endif
    uint32_t last_display_count = 0;
    int skip_count = 0;
    while (!display_thread_stop) {
        if (display_count != last_display_count) {
            last_display_count = display_count;
            int index = 0;  //! display_buffer_idx;
            for (int y = 0; y < display_height; y++) {
                int offset = 0;
                for (int x = 0; x < display_width; x++) {
                    uint32_t c =
                        (*display_palette)[display_buffer[index][y][x]];
                    uint8_t* target_pixel = (uint8_t*)sdl_screen_buff->pixels +
                                            (y * sdl_screen_buff->pitch) +
                                            (offset++ * 4);
                    *(uint32_t*)target_pixel = (c);
                }
            }
            sdl_flush(sdl_screen_buff);
        } else {
            skip_count++;
            if (skip_count == 10) {
                skip_count = 0;
                SDL_Delay(1);
            }
        }
    }
    printf("Exiting thread...\n");
    return 0;
}
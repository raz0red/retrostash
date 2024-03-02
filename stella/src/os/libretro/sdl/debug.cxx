#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include "sdl.h"
#include "StellaLIBRETRO.hxx"

static TTF_Font* main_font = NULL;

static char debug_text[512] = "";
static SDL_Texture* debug_message = NULL;
static SDL_Rect debug_source_rect;
static SDL_Rect debug_dest_rect;

extern StellaLIBRETRO stella;

int sdl_debug_init() {
#ifdef ATARI_SDL_DEBUG
    if (TTF_Init() != 0) {
        printf("TTF_Init error\n");
        return -1;
    }
#ifndef ATARI
    main_font = TTF_OpenFont("default.ttf", 20);
#else
    main_font = TTF_OpenFont("/oem/vendor/bin/default.ttf", 20);
#endif
#endif

    return 0;
}

void sdl_debug_display() {
#ifdef ATARI_SDL_DEBUG
    if (main_font) {
        uint32_t now = SDL_GetTicks();
        if (!debug_message) {
            int txtw, txth;
            SDL_Color color = {255, 255, 255, 0};
            SDL_Color bg = {0, 0, 0, 0};
            TTF_SizeUTF8(main_font, debug_text, &txtw, &txth);
            SDL_Surface* tmp_surface =
                TTF_RenderUTF8_Shaded(main_font, debug_text, color, bg);
            debug_message =
                SDL_CreateTextureFromSurface(sdl_renderer, tmp_surface);
            SDL_FreeSurface(tmp_surface);

            debug_source_rect = {0, 0, txtw, txth};
            debug_dest_rect = {10, 10, txtw, txth};
        }

        SDL_RenderCopy(sdl_renderer, debug_message, &debug_source_rect,
                       &debug_dest_rect);

        if ((display_render_count % (60 * 3)) == 0) {
            sprintf(debug_text, "FPS: %.2f (%.2f), Missed: %d, MD5: %s, Size: %d, Type: %s",
                    (1000.0 /
                     ((now - display_start_time) / (float)display_frame_count)),
                    (1000.0 / ((now - display_start_time) /
                               (float)display_render_count)),
                    display_skip_count,
                    stella.osystem().console().properties().get(PropType::Cart_MD5).c_str(),
                    stella.getROMSize(),
                    stella.osystem().console().cartridge().detectedType().c_str());

            if (debug_message) {
                SDL_DestroyTexture(debug_message);
                debug_message = NULL;
            }
        }

        if (display_render_count == 60 * 12) {
            display_start_time = now;
            display_frame_count = 0;
            display_render_count = 0;
            display_skip_count = 0;
        }
    }
#endif
}

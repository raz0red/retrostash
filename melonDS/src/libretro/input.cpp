#include "input.h"
#include "libretro_state.h"
#include "utils.h"

#include "NDS.h"

#ifdef WRC
#include "../../wrc.h"
#undef FILE
#include <emscripten.h>
#endif

InputState input_state;
u32 input_mask = 0xFFF;
static bool has_touched = false;

#define ADD_KEY_TO_MASK(key, i) if (!!input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, key)) input_mask &= ~(1 << i); else input_mask |= (1 << i);

bool cursor_enabled(InputState *state)
{
   return state->current_touch_mode == TouchMode::Mouse || state->current_touch_mode == TouchMode::Joystick;
}

#ifdef WRC
int wrc_blow = 0;
#endif

void update_input(InputState *state)
{
#ifdef WRC
   int controller = wrc_input_state[0];
   if (controller & INP_B)  input_mask &= ~(1 << 0); else input_mask |= (1 << 0);
   if (controller & INP_A)  input_mask &= ~(1 << 1); else input_mask |= (1 << 1);
   if (controller & INP_SELECT) input_mask &= ~(1 << 2); else input_mask |= (1 << 2);
   if (controller & INP_START) input_mask &= ~(1 << 3); else input_mask |= (1 << 3);
   if (controller & INP_RIGHT) input_mask &= ~(1 << 4); else input_mask |= (1 << 4);
   if (controller & INP_LEFT) input_mask &= ~(1 << 5); else input_mask |= (1 << 5);
   if (controller & INP_UP) input_mask &= ~(1 << 6); else input_mask |= (1 << 6);
   if (controller & INP_DOWN) input_mask &= ~(1 << 7); else input_mask |= (1 << 7);
   if (controller & INP_RBUMP) input_mask &= ~(1 << 8); else input_mask |= (1 << 8);
   if (controller & INP_LBUMP) input_mask &= ~(1 << 9); else input_mask |= (1 << 9);
   if (controller & INP_Y) input_mask &= ~(1 << 10); else input_mask |= (1 << 10);
   if (controller & INP_X) input_mask &= ~(1 << 11); else input_mask |= (1 << 11);

   wrc_blow = (controller & INP_LTRIG);

   NDS::SetKeyMask(input_mask);
#else
   input_poll_cb();

   ADD_KEY_TO_MASK(RETRO_DEVICE_ID_JOYPAD_A,      0);
   ADD_KEY_TO_MASK(RETRO_DEVICE_ID_JOYPAD_B,      1);
   ADD_KEY_TO_MASK(RETRO_DEVICE_ID_JOYPAD_SELECT, 2);
   ADD_KEY_TO_MASK(RETRO_DEVICE_ID_JOYPAD_START,  3);
   ADD_KEY_TO_MASK(RETRO_DEVICE_ID_JOYPAD_RIGHT,  4);
   ADD_KEY_TO_MASK(RETRO_DEVICE_ID_JOYPAD_LEFT,   5);
   ADD_KEY_TO_MASK(RETRO_DEVICE_ID_JOYPAD_UP,     6);
   ADD_KEY_TO_MASK(RETRO_DEVICE_ID_JOYPAD_DOWN,   7);
   ADD_KEY_TO_MASK(RETRO_DEVICE_ID_JOYPAD_R,      8);
   ADD_KEY_TO_MASK(RETRO_DEVICE_ID_JOYPAD_L,      9);
   ADD_KEY_TO_MASK(RETRO_DEVICE_ID_JOYPAD_X,      10);
   ADD_KEY_TO_MASK(RETRO_DEVICE_ID_JOYPAD_Y,      11);

   NDS::SetKeyMask(input_mask);

   bool lid_closed_btn = !!input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3);
   if(lid_closed_btn != state->lid_closed)
   {
      NDS::SetLidClosed(lid_closed_btn);
      state->lid_closed = lid_closed_btn;
   }

   state->holding_noise_btn = !!input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2);
   state->swap_screens_btn = !!input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2);
#endif

   if(current_screen_layout != ScreenLayout::TopOnly)
   {
      switch(state->current_touch_mode)
      {
         case TouchMode::Disabled:
            state->touching = false;
            break;

         case TouchMode::Mouse:
            {
#ifndef WRC
               int16_t mouse_x = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X);
               int16_t mouse_y = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y);
               state->touching = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT);
#else
#if 0
               int16_t mouse_x = wrc_mouse_x;
               int16_t mouse_y = wrc_mouse_y;
               state->touch_x = Clamp(state->touch_x + mouse_x, 0, VIDEO_WIDTH - 1);
               state->touch_y = Clamp(state->touch_y + mouse_y, 0, VIDEO_HEIGHT - 1);
#else
               state->touch_x = EM_ASM_INT({
                  return window.emulator.getMouseAbsX();
               });
               state->touch_y = EM_ASM_INT({
                  return window.emulator.getMouseAbsY();
               });
#endif
               state->touching = wrc_buttons & MOUSE_LEFT;
#endif
//printf("x:%d y:%d touching:%d\n", state->touch_x, state->touch_y, state->touching);
            }

            break;
         case TouchMode::Touch:
            if(input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_PRESSED))
            {
               int16_t pointer_x = input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X);
               int16_t pointer_y = input_state_cb(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y);

               unsigned int touch_scale = screen_layout_data.displayed_layout == ScreenLayout::HybridBottom ? screen_layout_data.hybrid_ratio : 1;

               unsigned int x = ((int)pointer_x + 0x8000) * screen_layout_data.buffer_width / 0x10000 / touch_scale;
               unsigned int y = ((int)pointer_y + 0x8000) * screen_layout_data.buffer_height / 0x10000 / touch_scale;

               if ((x >= screen_layout_data.touch_offset_x) && (x < screen_layout_data.touch_offset_x + screen_layout_data.screen_width) &&
                     (y >= screen_layout_data.touch_offset_y) && (y < screen_layout_data.touch_offset_y + screen_layout_data.screen_height))
               {
                  state->touching = true;

                  state->touch_x = Clamp((x - screen_layout_data.touch_offset_x) * VIDEO_WIDTH / screen_layout_data.screen_width, 0, VIDEO_WIDTH - 1);
                  state->touch_y = Clamp((y - screen_layout_data.touch_offset_y) * VIDEO_HEIGHT / screen_layout_data.screen_height, 0, VIDEO_HEIGHT - 1);
               }
            }
            else if(state->touching)
            {
               state->touching = false;
            }

            break;
         case TouchMode::Joystick:
            int16_t joystick_x = input_state_cb(0, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_X) / 2048;
            int16_t joystick_y = input_state_cb(0, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_Y) / 2048;

            state->touch_x = Clamp(state->touch_x + joystick_x, 0, VIDEO_WIDTH - 1);
            state->touch_y = Clamp(state->touch_y + joystick_y, 0, VIDEO_HEIGHT - 1);

            state->touching = !!input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3);

            break;
      }
   }
   else
   {
      state->touching = false;
   }

   if(state->touching)
   {
      NDS::TouchScreen(state->touch_x, state->touch_y);
      has_touched = true;
   }
   else if(has_touched)
   {
      NDS::ReleaseScreen();
      has_touched = false;
   }
}

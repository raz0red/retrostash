// #ifdef WRC
// #include <emscripten.h>
// #undef FILE
// #endif

#include "libretro-core.h"

#include "kbd.h"
#include "joystick.h"
#include "keyboard.h"

extern unsigned retro_key_event_state[RETROK_LAST];

static int kbd_get_modifier()
{
// printf("kbd_get_modifier\n");
// #ifndef WRC
// printf("non wrc modifier\n");
   int ret = 0;

   if (retro_key_event_state[RETROK_LSHIFT]) {
      ret |= KBD_MOD_LSHIFT;
   }
   if (retro_key_event_state[RETROK_RSHIFT]) {
      ret |= KBD_MOD_RSHIFT;
   }
   if (retro_key_event_state[RETROK_LALT]) {
      ret |= KBD_MOD_LALT;
   }
   if (retro_key_event_state[RETROK_RALT]) {
      ret |= KBD_MOD_RALT;
   }
   if (retro_key_event_state[RETROK_LCTRL]) {
      ret |= KBD_MOD_LCTRL;
   }

   return ret;
// #else
//    int ret = EM_ASM_INT({
//       return window.emulator.getKeyMods();
//    });
// printf("wrc modifier: %d\n", ret);
//    return ret;
// #endif
}

void kbd_handle_keydown(int kcode)
{
// printf("kbd_handle_keydown\n");
   keyboard_key_pressed((signed long)kcode, kbd_get_modifier());
}

void kbd_handle_keyup(int kcode)
{
   keyboard_key_released((signed long)kcode, kbd_get_modifier());
}

signed long kbd_arch_keyname_to_keynum(char *keyname)
{
   return (signed long)atoi(keyname);
}

const char *kbd_arch_keynum_to_keyname(signed long keynum)
{
   static char keyname[20];

   memset(keyname, 0, 20);

   sprintf(keyname, "%li", keynum);

   return keyname;
}

void kbd_arch_init()
{
   keyboard_clear_keymatrix();
}

void kbd_initialize_numpad_joykeys(int* joykeys)
{
}

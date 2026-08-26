/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - emulate_game_controller_via_input_plugin.c              *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2014 Bobby Smiles                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "emulate_game_controller_via_input_plugin.h"
#include "plugin/plugin.h"

#include "api/m64p_plugin.h"
#include "device/controllers/game_controller.h"
#include <libretro.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef WRC
/* mupen64plus-libretro-nx/custom/mupen64plus-core/plugin/ -> retrostash/wrc.h.
   Verify this depth against the actual build tree; a wrong count just fails
   to find the header at compile time and is a one-line fix. */
#include "../../../../wrc.h"
#endif

#define ROUND(x)    floor((x) + 0.5)

/* snprintf not available in MSVC 2010 and earlier */
#include "api/msvc_compat.h"

extern retro_environment_t environ_cb;
extern retro_input_state_t input_cb;
extern struct retro_rumble_interface rumble;
extern int pad_pak_types[4];
extern int pad_present[4];
extern int astick_deadzone;
extern int astick_sensitivity;
extern int r_cbutton;
extern int l_cbutton;
extern int d_cbutton;
extern int u_cbutton;
extern bool alternate_mapping;
static bool libretro_supports_bitmasks = false;

extern m64p_rom_header ROM_HEADER;

// Some stuff from n-rage plugin
#define RD_GETSTATUS        0x00        // get status
#define RD_READKEYS         0x01        // read button values
#define RD_READPAK          0x02        // read from controllerpack
#define RD_WRITEPAK         0x03        // write to controllerpack
#define RD_RESETCONTROLLER  0xff        // reset controller
#define RD_READEEPROM       0x04        // read eeprom
#define RD_WRITEEPROM       0x05        // write eeprom

#define PAK_IO_RUMBLE       0xC000      // the address where rumble-commands are sent to

/* global data definitions */
struct
{
    CONTROL *control;               // pointer to CONTROL struct in Core library
    BUTTONS buttons;
} controller[4];

void inputGetKeys_default( int Control, BUTTONS *Keys );
typedef void (*get_keys_t)(int, BUTTONS*);
static get_keys_t getKeys = inputGetKeys_default;

void inputGetKeys_default_descriptor(void)
{
   if (alternate_mapping)
   {
      #define independent_cbuttons_map(PAD) \
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,      "A Button" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,      "B Button" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,      "C-Right" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,      "C-Left" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,      "C-Down" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,      "C-Up" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,     "Z Trigger" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,     "R Shoulder" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "L Shoulder" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,  "Start" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT,  "D-Pad Right" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,   "D-Pad Left" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,   "D-Pad Down" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,     "D-Pad Up" },\
      { PAD, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,  RETRO_DEVICE_ID_ANALOG_X, "Control Stick X" },\
      { PAD, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,  RETRO_DEVICE_ID_ANALOG_Y, "Control Stick Y" },\
      { PAD, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_X, "C Buttons X" },\
      { PAD, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_Y, "C Buttons Y" },

      static struct retro_input_descriptor desc[] = {
         independent_cbuttons_map(0)
         independent_cbuttons_map(1)
         independent_cbuttons_map(2)
         independent_cbuttons_map(3)
         { 0 },
      };
      environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);
   }
   else
   {
      #define standard_map(PAD) \
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,      "A Button (C3)" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,      "B Button (C2)" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,      "(C1)" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,      "(C4)" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,     "C Buttons Mode" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,     "Z Trigger" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,      "R Shoulder" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,      "L Shoulder" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,  "Start" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT,  "D-Pad Right" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,   "D-Pad Left" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,   "D-Pad Down" },\
      { PAD, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,     "D-Pad Up" },\
      { PAD, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,  RETRO_DEVICE_ID_ANALOG_X, "Control Stick X" },\
      { PAD, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,  RETRO_DEVICE_ID_ANALOG_Y, "Control Stick Y" },\
      { PAD, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_X, "C Buttons X" },\
      { PAD, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_Y, "C Buttons Y" },

      static struct retro_input_descriptor desc[] = {
         standard_map(0)
         standard_map(1)
         standard_map(2)
         standard_map(3)
         { 0 },
      };
      environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);
   }
}

/* Mupen64Plus plugin functions */
EXPORT m64p_error CALL inputPluginStartup(m64p_dynlib_handle CoreLibHandle, void *Context,
                                   void (*DebugCallback)(void *, int, const char *))
{
   if (environ_cb(RETRO_ENVIRONMENT_GET_INPUT_BITMASKS, NULL))
      libretro_supports_bitmasks = true;
   getKeys = inputGetKeys_default;
   inputGetKeys_default_descriptor();
   return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL inputPluginShutdown(void)
{
   libretro_supports_bitmasks = false;
   abort();
   return 0;
}

EXPORT m64p_error CALL inputPluginGetVersion(m64p_plugin_type *PluginType, int *PluginVersion, int *APIVersion, const char **PluginNamePtr, int *Capabilities)
{
    // This function should never be called in libretro version
    return M64ERR_SUCCESS;
}

static unsigned char DataCRC( unsigned char *Data, int iLenght )
{
    unsigned char Remainder = Data[0];

    int iByte = 1;
    unsigned char bBit = 0;

    while( iByte <= iLenght )
    {
        int HighBit = ((Remainder & 0x80) != 0);
        Remainder = Remainder << 1;

        Remainder += ( iByte < iLenght && Data[iByte] & (0x80 >> bBit )) ? 1 : 0;

        Remainder ^= (HighBit) ? 0x85 : 0;

        bBit++;
        iByte += bBit/8;
        bBit %= 8;
    }

    return Remainder;
}

/******************************************************************
  Function: ControllerCommand
  Purpose:  To process the raw data that has just been sent to a
            specific controller.
  input:    - Controller Number (0 to 3) and -1 signalling end of
              processing the pif ram.
            - Pointer of data to be processed.
  output:   none

  note:     This function is only needed if the DLL is allowing raw
            data, or the plugin is set to raw

            the data that is being processed looks like this:
            initilize controller: 01 03 00 FF FF FF
            read controller:      01 04 01 FF FF FF FF
*******************************************************************/
EXPORT void CALL inputControllerCommand(int Control, unsigned char *Command)
{
    unsigned char *Data = &Command[5];

    if (Control == -1)
        return;

    switch (Command[2])
    {
        case RD_GETSTATUS:
            break;
        case RD_READKEYS:
            break;
        case RD_READPAK:
            if (controller[Control].control->Plugin == PLUGIN_RAW)
            {
                unsigned int dwAddress = (Command[3] << 8) + (Command[4] & 0xE0);

                if(( dwAddress >= 0x8000 ) && ( dwAddress < 0x9000 ) )
                    memset( Data, 0x80, 32 );
                else
                    memset( Data, 0x00, 32 );

                Data[32] = DataCRC( Data, 32 );
            }
            break;
        case RD_WRITEPAK:
            if (controller[Control].control->Plugin == PLUGIN_RAW)
            {
                unsigned int dwAddress = (Command[3] << 8) + (Command[4] & 0xE0);
                Data[32] = DataCRC( Data, 32 );

                if ((dwAddress == PAK_IO_RUMBLE) && (rumble.set_rumble_state))
                {
                    if (*Data)
                    {
                        rumble.set_rumble_state(Control, RETRO_RUMBLE_WEAK, 0xFFFF);
                        rumble.set_rumble_state(Control, RETRO_RUMBLE_STRONG, 0xFFFF);
                    }
                    else
                    {
                        rumble.set_rumble_state(Control, RETRO_RUMBLE_WEAK, 0);
                        rumble.set_rumble_state(Control, RETRO_RUMBLE_STRONG, 0);
                    }
                }
            }

            break;
        case RD_RESETCONTROLLER:
            break;
        case RD_READEEPROM:
            break;
        case RD_WRITEEPROM:
            break;
        }
}

/******************************************************************
  Function: GetKeys
  Purpose:  To get the current state of the controllers buttons.
  input:    - Controller Number (0 to 3)
            - A pointer to a BUTTONS structure to be filled with
            the controller state.
  output:   none
*******************************************************************/

// System analog stick range is -0x8000 to 0x8000
#define ASTICK_MAX 0x8000
#define CSTICK_DEADZONE 0x4000

#define CSTICK_RIGHT 0x200
#define CSTICK_LEFT 0x100
#define CSTICK_UP 0x800
#define CSTICK_DOWN 0x400


static void inputGetKeys_reuse(int16_t analogX, int16_t analogY, int Control, BUTTONS* Keys)
{
   double radius, angle;
   //  Keys->Value |= input_cb(Control, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_XX)    ? 0x4000 : 0; // Mempak switch
   //  Keys->Value |= input_cb(Control, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_XX)    ? 0x8000 : 0; // Rumblepak switch

#ifdef WRC
   analogX = (int16_t)(wrc_input_state_analog[Control][0] * 0x7fff);
   analogY = (int16_t)(wrc_input_state_analog[Control][1] * 0x7fff);
#else
   analogX = input_cb(Control, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_X);
   analogY = input_cb(Control, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT, RETRO_DEVICE_ID_ANALOG_Y);
#endif

   // Convert cartesian coordinate analog stick to polar coordinates
   radius = sqrt(analogX * analogX + analogY * analogY);
   angle = atan2(analogY, analogX);

   if (radius > astick_deadzone)
   {
      // Re-scale analog stick range to negate deadzone (makes slow movements possible)
      radius = (radius - astick_deadzone)*((float)ASTICK_MAX/(ASTICK_MAX - astick_deadzone));
      // N64 Analog stick range is from -80 to 80
      radius *= 80.0 / ASTICK_MAX * (astick_sensitivity / 100.0);
      // Convert back to cartesian coordinates
      Keys->X_AXIS = +(int32_t)ROUND(radius * cos(angle));
      Keys->Y_AXIS = -(int32_t)ROUND(radius * sin(angle));
   }
   else
   {
      Keys->X_AXIS = 0;
      Keys->Y_AXIS = 0;
   }
}

#ifdef WRC
static int UP      = 0x0001;
static int DOWN    = 0x0002;
static int LEFT    = 0x0004;
static int RIGHT   = 0x0008;
static int START   = 0x0010;
static int R_KEY   = 0x0020;
static int L_KEY   = 0x0040;
static int Z_KEY   = 0x0080;
static int A_KEY   = 0x0100;
static int B_KEY   = 0x0200;
static int CL_KEY  = 0x0400;
static int CR_KEY  = 0x0800;
static int CU_KEY  = 0x1000;
static int CD_KEY  = 0x2000;

static void inputGetKeys_wrc( int Control, BUTTONS *Keys )
{
   unsigned int wrc_state = wrc_input_state[Control];
   int16_t analogX = 0;
   int16_t analogY = 0;
   Keys->Value = 0;

   Keys->R_DPAD       = !!(wrc_state & RIGHT);
   Keys->L_DPAD       = !!(wrc_state & LEFT);
   Keys->D_DPAD       = !!(wrc_state & DOWN);
   Keys->U_DPAD       = !!(wrc_state & UP);
   Keys->START_BUTTON = !!(wrc_state & START);
   /* Z is the awkward rear trigger on a real N64 pad - accept either
      shoulder trigger so it's reachable regardless of gamepad shape. */
   Keys->Z_TRIG       = !!(wrc_state & Z_KEY);
   Keys->L_TRIG       = !!(wrc_state & L_KEY);
   Keys->R_TRIG       = !!(wrc_state & R_KEY);
   Keys->A_BUTTON     = !!(wrc_state & A_KEY);
   Keys->B_BUTTON     = !!(wrc_state & B_KEY);

   /* C buttons - right analog stick */
   // analogX = (int16_t)(wrc_input_state_analog[Control][2] * 0x7fff);
   // analogY = (int16_t)(wrc_input_state_analog[Control][3] * 0x7fff);
   analogX = (int16_t)(wrc_input_state_analog[Control][0] * 0x7fff);
   analogY = (int16_t)(wrc_input_state_analog[Control][1] * 0x7fff);

   // if (abs(analogX) > CSTICK_DEADZONE)
   //    Keys->Value |= (analogX < 0) ? CSTICK_RIGHT : CSTICK_LEFT;

   // if (abs(analogY) > CSTICK_DEADZONE)
   //    Keys->Value |= (analogY < 0) ? CSTICK_UP : CSTICK_DOWN;

   if (wrc_state & CU_KEY) Keys->Value |= CSTICK_UP;
   if (wrc_state & CD_KEY) Keys->Value |= CSTICK_DOWN;
   if (wrc_state & CL_KEY) Keys->Value |= CSTICK_LEFT;
   if (wrc_state & CR_KEY) Keys->Value |= CSTICK_RIGHT;

   /* N64 control stick - left analog stick (sourced inside inputGetKeys_reuse) */
   inputGetKeys_reuse(0, 0, Control, Keys);
}
#endif

#ifdef WRC
void inputGetKeys_default( int Control, BUTTONS *Keys )
{
   inputGetKeys_wrc(Control, Keys);
}
#else
void inputGetKeys_default( int Control, BUTTONS *Keys )
{
   unsigned i;
   bool cbuttons_mode = false;
   int16_t ret        = 0;
   int16_t analogX    = 0;
   int16_t analogY    = 0;
   Keys->Value        = 0;

   if (libretro_supports_bitmasks)
      ret = input_cb(Control, RETRO_DEVICE_JOYPAD,
            0, RETRO_DEVICE_ID_JOYPAD_MASK);
   else
   {
      for (i = 0; i <= RETRO_DEVICE_ID_JOYPAD_R3; i++)
      {
         if (input_cb(Control, RETRO_DEVICE_JOYPAD, 0, i))
            ret |= (1 << i);
      }
   }

   Keys->R_DPAD       = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_RIGHT)));
   Keys->L_DPAD       = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_LEFT)));
   Keys->D_DPAD       = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_DOWN)));
   Keys->U_DPAD       = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_UP)));
   Keys->START_BUTTON = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_START)));
   Keys->Z_TRIG       = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_L2)));

   if (alternate_mapping)
   {
      Keys->A_BUTTON  = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_B)));
      Keys->B_BUTTON  = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_Y)));
      Keys->R_CBUTTON = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_R)));
      Keys->L_CBUTTON = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_L)));
      Keys->D_CBUTTON = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_A)));
      Keys->U_CBUTTON = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_X)));
      Keys->R_TRIG    = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_R2)));
      Keys->L_TRIG    = !!((ret & (1 <<RETRO_DEVICE_ID_JOYPAD_SELECT)));
   }
   else
   {
      Keys->R_TRIG    = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_R)));
      Keys->L_TRIG    = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_L)));

      cbuttons_mode   = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_R2)));

      if (cbuttons_mode)
      {
         Keys->R_CBUTTON = !!((ret & (1 << r_cbutton)));
         Keys->L_CBUTTON = !!((ret & (1 << l_cbutton)));
         Keys->D_CBUTTON = !!((ret & (1 << d_cbutton)));
         Keys->U_CBUTTON = !!((ret & (1 << u_cbutton)));
      }
      else
      {
         Keys->A_BUTTON  = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_B)));
         Keys->B_BUTTON  = !!((ret & (1 << RETRO_DEVICE_ID_JOYPAD_Y)));
      }
   }

   // C buttons
   analogX = input_cb(Control, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_X);
   analogY = input_cb(Control, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_RIGHT, RETRO_DEVICE_ID_ANALOG_Y);

   if (abs(analogX) > CSTICK_DEADZONE)
      Keys->Value |= (analogX < 0) ? CSTICK_RIGHT : CSTICK_LEFT;

   if (abs(analogY) > CSTICK_DEADZONE)
      Keys->Value |= (analogY < 0) ? CSTICK_UP : CSTICK_DOWN;

   inputGetKeys_reuse(analogX, analogY, Control, Keys);
}
#endif


/******************************************************************
  Function: InitiateControllers
  Purpose:  This function initialises how each of the controllers
            should be handled.
  input:    - The handle to the main window.
            - A controller structure that needs to be filled for
              the emulator to know how to handle each controller.
  output:   none
*******************************************************************/
EXPORT void CALL inputInitiateControllers(CONTROL_INFO ControlInfo)
{
    int i;

    for( i = 0; i < 4; i++ )
    {
       controller[i].control = ControlInfo.Controls + i;
       controller[i].control->Present = pad_present[i];
       controller[i].control->RawData = 0;

       if (pad_pak_types[i] == PLUGIN_MEMPAK)
          controller[i].control->Plugin = PLUGIN_MEMPAK;
       else if (pad_pak_types[i] == PLUGIN_RAW)
          controller[i].control->Plugin = PLUGIN_RAW;
       else if (pad_pak_types[i] == PLUGIN_TRANSFER_PAK)
          controller[i].control->Plugin = PLUGIN_TRANSFER_PAK;
       else
          controller[i].control->Plugin = PLUGIN_NONE;
    }

   getKeys = inputGetKeys_default;
   inputGetKeys_default_descriptor();
}

/******************************************************************
  Function: ReadController
  Purpose:  To process the raw data in the pif ram that is about to
            be read.
  input:    - Controller Number (0 to 3) and -1 signalling end of
              processing the pif ram.
            - Pointer of data to be processed.
  output:   none
  note:     This function is only needed if the DLL is allowing raw
            data.
*******************************************************************/
EXPORT void CALL inputReadController(int Control, unsigned char *Command)
{
   inputControllerCommand(Control, Command);
}

/******************************************************************
  Function: RomClosed
  Purpose:  This function is called when a rom is closed.
  input:    none
  output:   none
*******************************************************************/
EXPORT void CALL inputRomClosed(void) { }

/******************************************************************
  Function: RomOpen
  Purpose:  This function is called when a rom is open. (from the
            emulation thread)
  input:    none
  output:   none
*******************************************************************/
EXPORT int CALL inputRomOpen(void) { return 1; }


int egcvip_is_connected(void* opaque, enum pak_type* pak)
{
    int channel = *(int*)opaque;

    CONTROL* c = &Controls[channel];

    switch(c->Plugin)
    {
    /*case PLUGIN_NONE: *pak = PAK_NONE; break;
    case PLUGIN_MEMPAK: *pak = PAK_MEM; break;
    case PLUGIN_RUMBLE_PAK: *pak = PAK_RUMBLE; break;
    case PLUGIN_TRANSFER_PAK: *pak = PAK_TRANSFER; break;*/

   // case PLUGIN_RAW:
        /* historically PLUGIN_RAW has been mostly (exclusively ?) used for rumble,
         * so we just reproduce that behavior */
        //*pak = PAK_RUMBLE; break;
    }

    return c->Present;
}

uint32_t egcvip_get_input(void* opaque)
{
    BUTTONS keys = { 0 };
    int channel = *(int*)opaque;

    if (getKeys)
       getKeys(channel, &keys);

    return keys.Value;

}

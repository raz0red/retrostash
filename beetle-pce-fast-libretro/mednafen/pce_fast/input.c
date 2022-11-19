/* Mednafen - Multi-system Emulator
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <string.h>

#include "../mednafen-types.h"
#include "../state_helpers.h"
#include "../mednafen.h"
#include "../mednafen-endian.h"

#include "pce.h"
#include "huc.h"
#include "huc6280.h"

#ifdef WRC
#include "../../../../wrc.h"
#include <emscripten.h>
#include <stdio.h>
#endif

static int InputTypes[5];
static uint8 *data_ptr[5];

static bool AVPad6Which[5]; // Lower(8 buttons) or higher(4 buttons).
bool AVPad6Enabled[5];

uint16 pce_jp_data[5];

static int64 mouse_last_meow[5];

static int32 mouse_x[5], mouse_y[5];
static uint16 mouse_rel[5];

static uint8 pce_mouse_button[5];
static uint8 mouse_index[5];

static uint8 sel;
static uint8 read_index = 0;

// GamepadIDII and GamepadIDII_DSR must be EXACTLY the same except for the RUN+SELECT exclusion in the latter.
static const InputDeviceInputInfoStruct GamepadIDII[] =
{
 { "i", "I", 12, IDIT_BUTTON_CAN_RAPID, NULL },
 { "ii", "II", 11, IDIT_BUTTON_CAN_RAPID, NULL },
 { "select", "SELECT", 4, IDIT_BUTTON, NULL },
 { "run", "RUN", 5, IDIT_BUTTON, NULL },
 { "up", "UP", 0, IDIT_BUTTON, "down" },
 { "right", "RIGHT", 3, IDIT_BUTTON, "left" },
 { "down", "DOWN", 1, IDIT_BUTTON, "up" },
 { "left", "LEFT", 2, IDIT_BUTTON, "right" },
 { "iii", "III", 10, IDIT_BUTTON, NULL },
 { "iv", "IV", 7, IDIT_BUTTON, NULL },
 { "v", "V", 8, IDIT_BUTTON, NULL },
 { "vi", "VI", 9, IDIT_BUTTON, NULL },
 { "mode_select", "2/6 Mode Select", 6, IDIT_BUTTON, NULL },
};
static const InputDeviceInputInfoStruct GamepadIDII_DSR[] =
{
 { "i", "I", 12, IDIT_BUTTON_CAN_RAPID, NULL },
 { "ii", "II", 11, IDIT_BUTTON_CAN_RAPID, NULL },
 { "select", "SELECT", 4, IDIT_BUTTON, "run" },
 { "run", "RUN", 5, IDIT_BUTTON, "select" },
 { "up", "UP", 0, IDIT_BUTTON, "down" },
 { "right", "RIGHT", 3, IDIT_BUTTON, "left" },
 { "down", "DOWN", 1, IDIT_BUTTON, "up" },
 { "left", "LEFT", 2, IDIT_BUTTON, "right" },
 { "iii", "III", 10, IDIT_BUTTON, NULL },
 { "iv", "IV", 7, IDIT_BUTTON, NULL },
 { "v", "V", 8, IDIT_BUTTON, NULL },
 { "vi", "VI", 9, IDIT_BUTTON, NULL },
 { "mode_select", "2/6 Mode Select", 6, IDIT_BUTTON, NULL },
};

static const InputDeviceInputInfoStruct MouseIDII[] =
{
 { "x_axis", "X Axis", -1, IDIT_X_AXIS_REL },
 { "y_axis", "Y Axis", -1, IDIT_Y_AXIS_REL },
 { "left", "Left Button", 0, IDIT_BUTTON, NULL },
 { "right", "Right Button", 1, IDIT_BUTTON, NULL },
};

// If we add more devices to this array, REMEMBER TO UPDATE the hackish array indexing in the SyncSettings() function
// below.
static InputDeviceInfoStruct InputDeviceInfo[] =
{
 // None
 {
  "none",
  "none",
  NULL,
  NULL,
  0,
  NULL
 },

 // Gamepad
 {
  "gamepad",
  "Gamepad",
  NULL,
  NULL,
  sizeof(GamepadIDII) / sizeof(InputDeviceInputInfoStruct),
  GamepadIDII,
 },

 // Mouse
 {
  "mouse",
  "Mouse",
  NULL,
  NULL,
  sizeof(MouseIDII) / sizeof(InputDeviceInputInfoStruct),
  MouseIDII,
 },

};

static const InputPortInfoStruct PortInfo[] =
{
 { "port1", "Port 1", sizeof(InputDeviceInfo) / sizeof(InputDeviceInfoStruct), InputDeviceInfo, "gamepad" },
 { "port2", "Port 2", sizeof(InputDeviceInfo) / sizeof(InputDeviceInfoStruct), InputDeviceInfo, "gamepad" },
 { "port3", "Port 3", sizeof(InputDeviceInfo) / sizeof(InputDeviceInfoStruct), InputDeviceInfo, "gamepad" },
 { "port4", "Port 4", sizeof(InputDeviceInfo) / sizeof(InputDeviceInfoStruct), InputDeviceInfo, "gamepad" },
 { "port5", "Port 5", sizeof(InputDeviceInfo) / sizeof(InputDeviceInfoStruct), InputDeviceInfo, "gamepad" },
};

static void SyncSettings(void)
{
 InputDeviceInfo[1].IDII = MDFN_GetSettingB("pce_fast.disable_softreset") ? GamepadIDII_DSR : GamepadIDII;
}

void PCEINPUT_Init(void)
{
   SyncSettings();
}

void PCEINPUT_SetInput(unsigned port, const char *type, uint8 *ptr)
{
   if(!strcmp(type, "gamepad"))
      InputTypes[port] = 1;
   else if(!strcmp(type, "mouse"))
      InputTypes[port] = 2;
   else
      InputTypes[port] = 0;
   data_ptr[port] = (uint8 *)ptr;
}

void INPUT_Frame(void)
{
   unsigned x;

#ifdef WRC
   bool pad6button = (wrc_options & OPT1);
#else
#endif

   for(x = 0; x < 5; x++)
   {
#ifdef WRC
      unsigned int state = x < 4 ? wrc_input_state[x] : 0;
#endif
      if(InputTypes[x] == 1)
      {
#ifdef WRC
      //  { "i", "I", 12, IDIT_BUTTON_CAN_RAPID, NULL },    0
      //  { "ii", "II", 11, IDIT_BUTTON_CAN_RAPID, NULL },  1
      //  { "select", "SELECT", 4, IDIT_BUTTON, NULL },     2
      //  { "run", "RUN", 5, IDIT_BUTTON, NULL },           3
      //  { "up", "UP", 0, IDIT_BUTTON, "down" },           4
      //  { "right", "RIGHT", 3, IDIT_BUTTON, "left" },     5
      //  { "down", "DOWN", 1, IDIT_BUTTON, "up" },         6
      //  { "left", "LEFT", 2, IDIT_BUTTON, "right" },      7

      //  { "iii", "III", 10, IDIT_BUTTON, NULL },          8
      //  { "iv", "IV", 7, IDIT_BUTTON, NULL },             9
      //  { "v", "V", 8, IDIT_BUTTON, NULL },               10
      //  { "vi", "VI", 9, IDIT_BUTTON, NULL },             11

         uint16 new_data = 0;
         if (state & INP_B) new_data |= (1 << 0);       // I
         if (state & INP_A) new_data |= (1 << 1);       // II
         if (state & INP_SELECT) new_data |= (1 << 2);  // Select
         if (state & INP_START) new_data |= (1 << 3);   // Run
         if (state & INP_UP) new_data |= (1 << 4);      // Up
         if (state & INP_RIGHT) new_data |= (1 << 5);   // Right
         if (state & INP_DOWN) new_data |= (1 << 6);    // Down
         if (state & INP_LEFT) new_data |= (1 << 7);    // Left
         if (pad6button) {
            if (state & INP_X) new_data |= (1 << 8);       // III
            if (state & INP_Y) new_data |= (1 << 9);       // IV
            if (state & INP_LBUMP) new_data |= (1 << 10);  // V
            if (state & INP_RBUMP) new_data |= (1 << 11);  // VI
         } else {
            if (state & INP_X) new_data |= (1 << 0);       // I
            if (state & INP_Y) new_data |= (1 << 1);       // II
         }
#else
         uint16 new_data = data_ptr[x][0] | (data_ptr[x][1] << 8);
#endif

#ifndef WRC
         if((new_data & 0x1000) && !(pce_jp_data[x] & 0x1000))
         {
            AVPad6Enabled[x] = !AVPad6Enabled[x];
            MDFN_DispMessage("%d-button mode selected for pad %d", AVPad6Enabled[x] ? 6 : 2, x + 1);
         }
#else
         AVPad6Enabled[x] = pad6button;
#endif

         pce_jp_data[x] = new_data;
      }
      else if(InputTypes[x] == 2)
      {
         mouse_x[x] += (int16)MDFN_de16lsb(data_ptr[x] + 0);
         mouse_y[x] += (int16)MDFN_de16lsb(data_ptr[x] + 2);
         pce_mouse_button[x] = *(uint8 *)(data_ptr[x] + 4);
      }
   }
}

void INPUT_FixTS(void)
{
   unsigned x;

   for(x = 0; x < 5; x++)
   {
      if(InputTypes[x] == 2)
         mouse_last_meow[x] -= HuCPU.timestamp;
   }
}

static INLINE bool CheckLM(int n)
{
   if((int64)HuCPU.timestamp - mouse_last_meow[n] > 10000)
   {
      int32 rel_x, rel_y;
      mouse_last_meow[n] = HuCPU.timestamp;

      rel_x = (int32)((0-mouse_x[n]));
      rel_y = (int32)((0-mouse_y[n]));

      if(rel_x < -127)
         rel_x = -127;
      if(rel_x > 127)
         rel_x = 127;
      if(rel_y < -127)
         rel_y = -127;
      if(rel_y > 127)
         rel_y = 127;

      mouse_rel[n] = ((rel_x & 0xF0) >> 4) | ((rel_x & 0x0F) << 4);
      mouse_rel[n] |= (((rel_y & 0xF0) >> 4) | ((rel_y & 0x0F) << 4)) << 8;

      mouse_x[n] += (int32)(rel_x);
      mouse_y[n] += (int32)(rel_y);

      return(1);
   }
   return(0);
}

uint8 INPUT_Read(unsigned int A)
{
   uint8 ret = 0xF;
   int tmp_ri = read_index;

   if(tmp_ri > 4)
      ret ^= 0xF;
   else
   {
      if(!InputTypes[tmp_ri])
         ret ^= 0xF;
      else if(InputTypes[tmp_ri] == 2) // Mouse
      {
         if(sel & 1)
         {
            CheckLM(tmp_ri);
            ret ^= 0xF;
            ret ^= mouse_rel[tmp_ri] & 0xF;

            mouse_rel[tmp_ri] >>= 4;
         }
         else
         {
           ret ^= pce_mouse_button[tmp_ri] & 0xF;
         }
      }
      else
      {
         if(InputTypes[tmp_ri] == 1) // Gamepad
         {
            if(AVPad6Which[tmp_ri] && AVPad6Enabled[tmp_ri])
            {
               if(sel & 1)
                  ret ^= 0x0F;
               else
                  ret ^= (pce_jp_data[tmp_ri] >> 8) & 0x0F;
            }
            else
            {
               if(sel & 1)
                  ret ^= (pce_jp_data[tmp_ri] >> 4) & 0x0F;
               else
                  ret ^= pce_jp_data[tmp_ri] & 0x0F;
            }
            if(!(sel & 1))
               AVPad6Which[tmp_ri] = !AVPad6Which[tmp_ri];
         }
      }
   }

   if(!PCE_IsCD)
      ret |= 0x80; // Set when CDROM is not attached

   ret |= 0x30; // Always-set?

   return ret;
}

void INPUT_Write(unsigned int A, uint8 V)
{
   if((V & 1) && !(sel & 2) && (V & 2))
      read_index = 0;
   else if((V & 1) && !(sel & 1))
   {
      if(read_index < 255)
         read_index++;
   }
   sel = V & 3;
}

int INPUT_StateAction(StateMem *sm, int load, int data_only)
{
   SFORMAT StateRegs[] =
   {
      // 0.8.A fix:
      SFARRAYB(AVPad6Enabled, 5),
      SFARRAYB(AVPad6Which, 5),

      SFVARN(mouse_last_meow[0], "mlm_0"),
      SFVARN(mouse_last_meow[1], "mlm_1"),
      SFVARN(mouse_last_meow[2], "mlm_2"),
      SFVARN(mouse_last_meow[3], "mlm_3"),
      SFVARN(mouse_last_meow[4], "mlm_4"),

      SFARRAY32(mouse_x, 5),
      SFARRAY32(mouse_y, 5),
      SFARRAY16(mouse_rel, 5),
      SFARRAY(pce_mouse_button, 5),
      SFARRAY(mouse_index, 5),
      // end 0.8.A fix

      SFARRAY16(pce_jp_data, 5),
      SFVAR(sel),
      SFVAR(read_index),
      SFEND
   };
   return MDFNSS_StateAction(sm, load, data_only, StateRegs, "JOY", false);
}

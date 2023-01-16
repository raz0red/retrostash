/*
 * input.c - keyboard, joysticks and mouse emulation
 *
 * Copyright (C) 2001-2002 Piotr Fusik
 * Copyright (C) 2001-2006 Atari800 development team (see DOC/CREDITS)
 *
 * This file is part of the Atari800 emulator project which emulates
 * the Atari 400, 800, 800XL, 130XE, and 5200 8-bit computers.
 *
 * Atari800 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Atari800 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Atari800; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "config.h"
#include <string.h>
#include "antic.h"
#include "atari.h"
#include "cartridge.h"
#include "cpu.h"
#include "gtia.h"
#include "input.h"
#include "memory.h"
#include "pia.h"
#include "pokeysnd.h"
#include "util.h"

extern UBYTE PCPOT_input[8];
unsigned int atari_analog[4] = {0};

int key_code = AKEY_NONE;
int key_shift = 0;
int key_consol = CONSOL_NONE;

int joy_multijoy = 0;

unsigned int joy_5200_digital_min = JOY_5200_MIN;
unsigned int joy_5200_digital_max = JOY_5200_MAX;
unsigned int joy_5200_analog_min  = JOY_5200_MIN;
unsigned int joy_5200_analog_max  = JOY_5200_MAX;

int mouse_mode = MOUSE_OFF;
int mouse_port = 0;
int mouse_delta_x = 0;
int mouse_delta_y = 0;
int mouse_buttons = 0;
int mouse_speed = 3;
int mouse_pot_min = 1;
int mouse_pot_max = 228;
/* There should be UI or options for light pen/gun offsets.
   Below are best offsets for different programs:
   AtariGraphics: H = 0..32, V = 0 (there's calibration in the program)
   Bug Hunt: H = 44, V = 2
   Barnyard Blaster: H = 40, V = 0
   Operation Blood (light gun version): H = 40, V = 4
 */
int mouse_pen_ofs_h = 42;
int mouse_pen_ofs_v = 2;
int mouse_joy_inertia = 10;

#ifndef MOUSE_SHIFT
#define MOUSE_SHIFT 4
#endif

static UBYTE STICK[4];
static UBYTE TRIG_input[4] = {0};

void INPUT_Initialise(void) {
	int i;
	for (i = 0; i < 4; i++)
	{
		PCPOT_input[i << 1]       = JOY_5200_CENTER;
		PCPOT_input[(i << 1) + 1] = JOY_5200_CENTER;
		TRIG_input[i]             = 1;
	}
}

void INPUT_Frame(void) {
	int i;
	static int last_key_code = AKEY_NONE;
	static int last_key_break = 0;
	static UBYTE last_stick[4] = {STICK_CENTRE, STICK_CENTRE, STICK_CENTRE, STICK_CENTRE};
	/* handle keyboard */

	/* In Atari 5200 joystick there's a second fire button, which acts
	   like the Shift key in 800/XL/XE (bit 3 in SKSTAT) and generates IRQ
	   like the Break key (bit 7 in IRQST and IRQEN).
	   Note that in 5200 the joystick position and first fire button are
	   separate for each port, but the keypad and 2nd button are common.
	   That is, if you press a key in the emulator, it's like you really pressed
	   it in all the controllers simultaneously. Normally the port to read
	   keypad & 2nd button is selected with the CONSOL register in GTIA
	   (this is simply not emulated).
	   key_code is used for keypad keys and key_shift is used for 2nd button.
	*/
  i = key_shift;
	if (i && !last_key_break) {
		if (IRQEN & 0x80) {
			IRQST &= ~0x80;
			GenerateIRQ();
		}
	}
	last_key_break = i;

  SKSTAT |= 0xc;
	if (key_shift)
		SKSTAT &= ~8;

	if (key_code < 0) {
		if (press_space) {
			key_code = AKEY_SPACE;
			press_space = 0;
		}
		else 
		{
			last_key_code = AKEY_NONE;
		}
	}

	/* The 5200 has only 4 of the 6 keyboard scan lines connected.
	 * Pressing one 5200 key is like pressing 4 Atari 800 keys.
	 * The LSB (bit 0) and bit 5 are the two missing lines.
	 * When debounce is enabled, multiple keys pressed generate
	 * no results.
	 * When debounce is disabled, multiple keys pressed generate
	 * results only when in numerical sequence.
	 * Thus the LSB being one of the missing lines is important
	 * because that causes events to be generated.
	 * Two events are generated every 64 scan lines
	 * but this code only does one every frame.
	 * Bit 5 is different for each keypress because it is one
	 * of the missing lines. */
	static int bit5_5200 = 0;
	if (bit5_5200)
		key_code &= ~0x20;

	if (cart_info.keys_debounced)
		bit5_5200 = !bit5_5200;
	else
		bit5_5200 = 0;

	/* 5200 2nd fire button generates CTRL as well */
	if (key_shift)
		key_code |= AKEY_SHFTCTRL;

	if (key_code >= 0) {
	  SKSTAT &= ~4;
		if ((key_code ^ last_key_code) & ~AKEY_SHFTCTRL) {
		/* ignore if only shift or control has changed its state */
			last_key_code = key_code;
			KBCODE = (UBYTE) key_code;
			if (IRQEN & 0x40) {
				if (IRQST & 0x40) {
					IRQST &= ~0x40;
					GenerateIRQ();
				}
				else {
					/* keyboard over-run */
					SKSTAT &= ~0x40;
					/* assert(IRQ != 0); */
				}
			}
		}
	}

	/* handle joysticks
	 * > port 0: controller 1 + 2 */
	i = Atari_PORT(0);
	STICK[0] = i & 0x0f;
	STICK[1] = (i >> 4) & 0x0f;

	/* We don't support the other two sticks, so this will result
	 * in both being in the CENTER position... */
	i = Atari_PORT(1);
	STICK[2] = i & 0x0f;
	STICK[3] = (i >> 4) & 0x0f;

	for (i = 0; i < 2; i++) {
		if (STICK[i] != STICK_CENTRE) {
			if ((STICK[i] & 0x0c) == 0) {	/* right and left simultaneously */
				if (last_stick[i] & 0x04)	/* if wasn't left before, move left */
					STICK[i] |= 0x08;
				else						/* else move right */
					STICK[i] |= 0x04;
			}
			else {
				last_stick[i] &= 0x03;
				last_stick[i] |= STICK[i] & 0x0c;
			}
			if ((STICK[i] & 0x03) == 0) {	/* up and down simultaneously */
				if (last_stick[i] & 0x01)	/* if wasn't up before, move up */
					STICK[i] |= 0x02;
				else						/* else move down */
					STICK[i] |= 0x01;
			}
			else {
				last_stick[i] &= 0x0c;
				last_stick[i] |= STICK[i] & 0x03;
			}
		}
		else
			last_stick[i] = STICK[i];

		TRIG_input[i] = Atari_TRIG(i);
	}

	for (i = 0; i < 4; i++) {
		if (atari_analog[i]) {
			PCPOT_input[(i << 1) + 0] = Atari_POT((i << 1) + 0);
			PCPOT_input[(i << 1) + 1] = Atari_POT((i << 1) + 1);
		}
		else {
			if ((STICK[i] & (STICK_CENTRE ^ STICK_LEFT)) == 0) {
				PCPOT_input[(i << 1) + 0] = joy_5200_digital_min;
			}
			else if ((STICK[i] & (STICK_CENTRE ^ STICK_RIGHT)) == 0) {
				PCPOT_input[(i << 1) + 0] = joy_5200_digital_max;
			}
			else {
				PCPOT_input[(i << 1) + 0] = JOY_5200_CENTER;
			}
			if ((STICK[i] & (STICK_CENTRE ^ STICK_FORWARD)) == 0) {
				PCPOT_input[(i << 1) + 1] = joy_5200_digital_min;
			}
			else if ((STICK[i] & (STICK_CENTRE ^ STICK_BACK)) == 0) {
				PCPOT_input[(i << 1) + 1] = joy_5200_digital_max;
			}
			else {
				PCPOT_input[(i << 1) + 1] = JOY_5200_CENTER;
			}
		}
	}

	TRIG[0] = TRIG_input[0];
	TRIG[1] = TRIG_input[1];
	TRIG[2] = TRIG_input[2];
	TRIG[3] = TRIG_input[3];
	PORT_input[0] = (STICK[1] << 4) | STICK[0];
	PORT_input[1] = (STICK[3] << 4) | STICK[2];
}

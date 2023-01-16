/*****************************************************************************/
/*                                                                           */
/* Module:  POKEY Chip Simulator Includes, V2.3                              */
/* Purpose: To emulate the sound generation hardware of the Atari POKEY chip. */
/* Author:  Ron Fries                                                        */
/*                                                                           */
/* Revision History:                                                         */
/*                                                                           */
/* 09/22/96 - Ron Fries - Initial Release                                    */
/* 04/06/97 - Brad Oliver - Some cross-platform modifications. Added         */
/*                          big/little endian #defines, removed <dos.h>,     */
/*                          conditional defines for TRUE/FALSE               */
/* 01/19/98 - Ron Fries - Changed signed/unsigned sample support to a        */
/*                        compile-time option.    */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                 License Information and Copyright Notice                  */
/*                 ========================================                  */
/*                                                                           */
/* PokeySound is Copyright(c) 1996-1998 by Ron Fries                         */
/*                                                                           */
/* This library is free software; you can redistribute it and/or modify it   */
/* under the terms of version 2 of the GNU Library General Public License    */
/* as published by the Free Software Foundation.                             */
/*                                                                           */
/* This library is distributed in the hope that it will be useful, but       */
/* WITHOUT ANY WARRANTY; without even the implied warranty of                */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library */
/* General Public License for more details.                                  */
/* To obtain a copy of the GNU Library General Public License, write to the  */
/* Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.   */
/*                                                                           */
/* Any permitted reproduction of these routines, in whole or in part, must   */
/* bear this legend.                                                         */
/*                                                                           */
/*****************************************************************************/

#ifndef _POKEYSOUND_H
#define _POKEYSOUND_H

#include <stdint.h>

#include "config.h"
#include "pokey.h"

#ifndef _TYPEDEF_H
#define _TYPEDEF_H

/* define some data types to keep it platform independent */
#define int8  int8_t
#define int16 int16_t
#define int32 int32_t

#define uint8  uint8_t
#define uint16 uint16_t
#define uint32 uint32_t

#endif

/* CONSTANT DEFINITIONS */

/* As an alternative to using the exact frequencies, selecting a playback
   frequency that is an exact division of the main clock provides a higher
   quality output due to less aliasing.  For best results, a value of
   1787520 MHz is used for the main clock.  With this value, both the
   64 kHz and 15 kHz clocks are evenly divisible.  Selecting a playback
   frequency that is also a division of the clock provides the best
   results.  The best options are FREQ_64 divided by either 2, 3, or 4.
   The best selection is based on a trade off between performance and
   sound quality.

   Of course, using a main clock frequency that is not exact will affect
   the pitch of the output.  With these numbers, the pitch will be low
   by 0.127%.  (More than likely, an actual unit will vary by this much!) */

#define FREQ_17_EXACT     1789790	/* exact 1.79 MHz clock freq */
#define FREQ_17_APPROX    1787520	/* approximate 1.79 MHz clock freq */

#ifdef __cplusplus
extern "C" {
#endif

#define SAMP_MAX 255			/* else set unsigned 8-bit clip ranges */
#define SAMP_MIN 0
#define SAMP_MID 128

/* init flags */
#define SND_BIT16	1
#define SND_STEREO	2

extern int stereo_enabled;

extern void (*Update_pokey_sound)(uint16 addr, uint8 val, uint8 /*chip*/, uint8 gain);

void Pokey_sound_init(uint32 freq17, uint16 playback_freq, uint8 num_pokeys,
                     unsigned int flags
                     );
void Pokey_process(void *sndbuffer, unsigned sndn);

#ifdef __cplusplus
}

#endif
#endif

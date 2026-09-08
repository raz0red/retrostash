/* ----------------------------------------------------------------------------
 *   ___  ___  ___  ___       ___  ____  ___  _  _
 *  /__/ /__/ /  / /__  /__/ /__    /   /_   / |/ /
 * /    / \  /__/ ___/ ___/ ___/   /   /__  /    /  emulator
 *
 * ----------------------------------------------------------------------------
 * Copyright 2005 Greg Stanton
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * ----------------------------------------------------------------------------
 * ProSystem.h
 * ----------------------------------------------------------------------------
 */
#ifndef PRO_SYSTEM_H
#define PRO_SYSTEM_H

#include <stdint.h>

#include <boolean.h>

#ifdef __cplusplus
extern "C" {
#endif

// The number of cycles per scan line
#define CYCLES_PER_SCANLINE 454

extern void prosystem_Reset(void);
extern void prosystem_ExecuteFrame(const uint8_t* input);
extern bool prosystem_Save(char *buffer, bool compress);
extern bool prosystem_Load(const char *buffer);
extern uint32_t prosystem_GetStateSize(void);
extern void prosystem_Close(bool persistent_data);

extern uint32_t prosystem_cycles;
extern uint16_t prosystem_frequency;
extern uint16_t prosystem_scanlines;
extern uint16_t maria_scanline;
extern uint8_t prosystem_mstat_adjust; /* Pole Position II hack */

#ifdef __cplusplus
}
#endif

#endif

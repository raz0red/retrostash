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
 * Database.h
 * ----------------------------------------------------------------------------
 */
#ifndef DATABASE_H
#define DATABASE_H

#include "ProSystem.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void database_Initialize(void);
// extern void database_Load(const char *digest);
extern bool database_Lookup(const char *digest, uint32_t size, uint8_t* header);
extern bool cartlist_Lookup(const char* digest,
    const char* digest64k, const char* digest32k, const char* digest16k,
    uint32_t size, uint8_t* header);

#ifdef __cplusplus
}
#endif

#endif

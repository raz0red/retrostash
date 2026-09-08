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
 * PokeySound is Copyright(c) 1997 by Ron Fries
 *                                                                           
 * This library is free software; you can redistribute it and/or modify it   
 * under the terms of version 2 of the GNU Library General Public License    
 * as published by the Free Software Foundation.                             
 *                                                                           
 * This library is distributed in the hope that it will be useful, but       
 * WITHOUT ANY WARRANTY; without even the implied warranty of                
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library 
 * General Public License for more details.                                  
 * To obtain a copy of the GNU Library General Public License, write to the  
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.   
 *                                                                           
 * Any permitted reproduction of these routines, in whole or in part, must   
 * bear this legend.                                                         
 * ----------------------------------------------------------------------------
 * Xm.h
 * ----------------------------------------------------------------------------
 */
#ifndef XM_H
#define XM_H

#include <stdint.h>
#include <boolean.h>

#define XM_RAM_SIZE 0x20000

#ifdef __cplusplus
extern "C" {
#endif

void xm_Reset();
uint8_t xm_Read(uint32_t address);
void xm_Write(uint32_t address, uint8_t data);
extern bool dma_active;
extern bool xm_mem_enabled;
extern bool xm_pokey_enabled;
extern bool xm_ym_enabled;
extern bool xm_48kram_enabled;
extern bool xm_bank0_enabled;
extern bool xm_bank1_enabled;
extern bool xm_ramwe_disabled;
extern uint8_t cntrl1;
extern uint8_t cntrl2;
extern uint8_t cntrl3;
extern uint8_t cntrl4;
extern uint8_t cntrl5;
extern uint32_t ym_addr;
extern uint8_t xm_ram[XM_RAM_SIZE];

#ifdef __cplusplus
}
#endif

#endif

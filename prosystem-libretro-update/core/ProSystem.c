/* ----------------------------------------------------------------------------
 *   ___  ___  ___  ___       ___  ____  ___  _  _
 *  /__/ /__/ /  / /__  /__/ /__    /   /_   / |/ /
 * /    / \  /__/ ___/ ___/ ___/   /   /__  /    /  emulator
 *
 * ----------------------------------------------------------------------------
 * Copyright 2003,2004 Greg Stanton
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
 * Palette.c
 * ----------------------------------------------------------------------------
 */
#include <stdio.h>
#include "ProSystem.h"
#include <string.h>
#include "Equates.h"
#include "Bios.h"
#include "Cartridge.h"
#include "Maria.h"
#include "Memory.h"
#include "Region.h"
#include "Riot.h"
#include "Sally.h"
#include "Tia.h"
#include "Pokey.h"
#include "BupChip.h"
#include "Xm.h"
#include "ym.h"
#define PRO_SYSTEM_STATE_HEADER "PRO-SYSTEM STATE"

bool prosystem_active = false;
bool prosystem_paused = false;
uint16_t prosystem_frequency = 60;
uint8_t prosystem_frame = 0;
uint16_t prosystem_scanlines = 262;
uint32_t prosystem_cycles = 0;
uint8_t prosystem_mstat_adjust = 0; /* Pole Position II hack */

/** The current Maria scan line */
uint16_t maria_scanline = 1;

void prosystem_Reset(void)
{
   if (!cartridge_IsLoaded())
      return;

   maria_scanline = 1;
   prosystem_paused = false;
   prosystem_frame = 0;

   sally_Reset();
   region_Reset();
   tia_Clear();
   tia_Reset();
   pokey_Clear();
   pokey_Reset();
   xm_Reset();
   ym_Reset();

   memory_Reset();
   maria_Clear();
   maria_Reset();
   riot_Reset();
#if 0
    if (Bios.IsEnabled()) {
        Bios.Store();
    } else {
#endif
   cartridge_Store();
#if 0
    }
#endif
   prosystem_cycles = sally_ExecuteRES() << 2;
   prosystem_active = true;
}

void prosystem_ExecuteFrame(const uint8_t *input)
{
   uint32_t scanlinesPerBupchipTick;
   uint32_t bupchipTickScanlines = 0;
   uint32_t currentBupchipTick   = 0;

#if 0
   // Is the lightgun enabled for the current frame?
   var lightgun = (isLightGunEnabled() && (memory_ram[CTRL] & 96) != 64);
#endif

   riot_SetInput(input);

   scanlinesPerBupchipTick = (prosystem_scanlines - 1) / 4;

   if (cartridge_pokey || cartridge_xm) pokey_Frame();

   for (maria_scanline = 1; maria_scanline <= prosystem_scanlines; maria_scanline++)
   {
      if (maria_scanline == maria_displayArea.top)
      {
         memory_ram[MSTAT] = 0;
      }
      else if (maria_scanline == (maria_displayArea.bottom - prosystem_mstat_adjust) /* PPII Hack */)
      {
         memory_ram[MSTAT] = 128;
      }

      // Was a WSYNC performed within the current scanline?
      bool wsync_scanline = false;

      uint32_t cycles = 0;

      // Reset ProSystem cycles for current frame
      (prosystem_cycles %= CYCLES_PER_SCANLINE);

#if 0
      // If lightgun is enabled, check to see if it should be fired
      if (lightgun) prosystem_FireLightGun();
#endif

      while (prosystem_cycles < cartridge_hblank)
      {
         cycles = (sally_ExecuteInstruction() << 2);
         prosystem_cycles += cycles;
#if 0
         if (lightgun) prosystem_FireLightGun();
#endif

         if (half_cycle) {
            prosystem_cycles += 2;
#if 0 // raz TODO
            if (lightgun) prosystem_FireLightGun();
#endif
         }

         if (riot_timing)
            riot_UpdateTimer(cycles >> 2);

         if (memory_ram[WSYNC])
         {
            memory_ram[WSYNC] = 0;
            wsync_scanline = true;
            break;
         }
      }

      dma_active = true;

      cycles = (((maria_RenderScanline(maria_scanline)) + 3) >> 2) << 2;
      if (cycles > MARIA_CYCLE_LIMIT)
      {
         // cycles = MARIA_CYCLE_LIMIT; // TODO: This causes flicker is Scramble (is it necessary?)
         wsync_scanline = true;
      }

      dma_active = false;

      prosystem_cycles += cycles;

      if (riot_timing)
      {
         riot_UpdateTimer((cycles) >> 2);
      }

      // https://atariage.com/forums/topic/201163-the-truth-about-wsync-and-other-scanline-issues/
      // - The 6502 requires two cycles to acknowledge the NMI.
      // - 0-6 cycles pass as the 6502 finishes the currently executing instruction.
      // Interrupt entry takes 7 cycles.
      if (nmi)
      {
         if (!wsync_scanline && (prosystem_cycles < CYCLES_PER_SCANLINE))
         {
            cycles = (sally_ExecuteInstruction() << 2); // 0-6 cycles pass for current instruction
            if (riot_timing)
            {
               riot_UpdateTimer(cycles >> 2);
            }
            prosystem_cycles += cycles;

            if (memory_ram[WSYNC])
            {
               memory_ram[WSYNC] = 0;
               wsync_scanline = true;
            }
         }
         sally_ExecuteNMI();
      }

      while (!wsync_scanline && prosystem_cycles < CYCLES_PER_SCANLINE)
      {
         cycles = (sally_ExecuteInstruction() << 2);
         prosystem_cycles += cycles;
#if 0
         if (lightgun) prosystem_FireLightGun();
#endif

         if (half_cycle) {
            prosystem_cycles += 2;
#if 0 // raz TODO
            if (lightgun) prosystem_FireLightGun();
#endif
         }

         if (riot_timing)
         {
            riot_UpdateTimer(cycles >> 2);
         }

         if (memory_ram[WSYNC])
         {
            memory_ram[WSYNC] = 0;
            wsync_scanline = true;
            break;
         }
      }

      // If a WSYNC was performed and the current cycle count is less than
      // the cycles per scanline, add those cycles to current timers.
      if (wsync_scanline && (prosystem_cycles < CYCLES_PER_SCANLINE))
      {
         if (riot_timing)
         {
            riot_UpdateTimer((CYCLES_PER_SCANLINE - prosystem_cycles) >> 2);
         }
         prosystem_cycles = CYCLES_PER_SCANLINE;
      }

#if 0
      if (lightgun) prosystem_FireLightGun();
#endif

      tia_Process(2);
      if (cartridge_pokey || cartridge_xm)
      {
         pokey_Process(2);
      }

#ifdef SOUPER
      if (cartridge_bupchip)
      {
         bupchipTickScanlines++;
         if (bupchipTickScanlines == scanlinesPerBupchipTick)
         {
            bupchipTickScanlines = 0;
            bupchip_Process(currentBupchipTick);
            currentBupchipTick++;
         }
      }
#endif

      if (cartridge_pokey || cartridge_xm) pokey_Scanline();
   }

   prosystem_frame++;
   if (prosystem_frame >= prosystem_frequency)
   {
      prosystem_frame = 0;
   }
}

uint32_t prosystem_GetStateSize(void)
{
   /* header + version + reserved + digest */
   uint32_t size = 16 + 1 + 4 + 32;

   /* CPU registers + bank */
   size += 7 + 1;

   /* RAM */
   size += 16384;
   if (cartridge_type == CARTRIDGE_TYPE_SUPERCART_RAM)
      size += 16384;

   /* RIOT */
   size += 8;

   /* XM (mem/pokey/ym/48k/bank0/bank1/ramwe/dma flags + cntrl1-5 + ym_addr + RAM) */
   if (cartridge_xm)
      size += 8 + 5 + 1 + XM_RAM_SIZE;

   /* POKEY */
   size += 32;

   /* YM2151 */
   size += 256;

#ifdef SOUPER
   /* Souper: chr banks + mode + ram page banks + souper RAM + bupchip state */
   if (cartridge_type == CARTRIDGE_TYPE_SOUPER)
      size += 2 + 1 + 2 + sizeof(memory_souper_ram) + 3;
#endif

   return size;
}

bool prosystem_Save(char *buffer, bool compress)
{
   uint32_t size = 0;
   uint32_t index;

   printf("Saving game state.\n");

   for(index = 0; index < 16; index++)
      buffer[size + index] = PRO_SYSTEM_STATE_HEADER[index];
   size += 16;

   buffer[size++] = 1;
   for(index = 0; index < 4; index++)
      buffer[size + index] = 0;
   size += 4;

   for(index = 0; index < 32; index++)
      buffer[size + index] = cartridge_digest[index];
   size += 32;

   buffer[size++] = sally_a;
   buffer[size++] = sally_x;
   buffer[size++] = sally_y;
   buffer[size++] = sally_p;
   buffer[size++] = sally_s;
   buffer[size++] = sally_pc.b.l;
   buffer[size++] = sally_pc.b.h;
   buffer[size++] = cartridge_bank;

   for(index = 0; index < 16384; index++)
      buffer[size + index] = memory_ram[index];
   size += 16384;

   if(cartridge_type == CARTRIDGE_TYPE_SUPERCART_RAM)
   {
      for(index = 0; index < 16384; index++)
         buffer[size + index] = memory_ram[16384 + index];
      size += 16384;
   }

   /* RIOT */
   buffer[size++] = riot_dra;
   buffer[size++] = riot_drb;
   buffer[size++] = riot_timing ? 1 : 0;
   buffer[size++] = (riot_timer >> 8) & 0xFF;
   buffer[size++] = riot_timer & 0xFF;
   buffer[size++] = riot_intervals;
   buffer[size++] = (riot_clocks >> 8) & 0xFF;
   buffer[size++] = riot_clocks & 0xFF;

   /* XM */
   if (cartridge_xm)
   {
      buffer[size++] = xm_mem_enabled ? 1 : 0;
      buffer[size++] = xm_pokey_enabled ? 1 : 0;
      buffer[size++] = xm_ym_enabled ? 1 : 0;
      buffer[size++] = xm_48kram_enabled ? 1 : 0;
      buffer[size++] = xm_bank0_enabled ? 1 : 0;
      buffer[size++] = xm_bank1_enabled ? 1 : 0;
      buffer[size++] = xm_ramwe_disabled ? 1 : 0;
      buffer[size++] = dma_active ? 1 : 0;
      buffer[size++] = cntrl1;
      buffer[size++] = cntrl2;
      buffer[size++] = cntrl3;
      buffer[size++] = cntrl4;
      buffer[size++] = cntrl5;
      buffer[size++] = (uint8_t)ym_addr;

      for(index = 0; index < XM_RAM_SIZE; index++)
         buffer[size + index] = xm_ram[index];
      size += XM_RAM_SIZE;
   }

   /* POKEY */
   for(index = 0; index < 32; index++)
      buffer[size + index] = pokey_registers[index];
   size += 32;

   /* YM2151 */
   for(index = 0; index < 256; index++)
      buffer[size + index] = ym_registers[index];
   size += 256;

#ifdef SOUPER
   if(cartridge_type == CARTRIDGE_TYPE_SOUPER)
   {
      buffer[size++] = cartridge_souper_chr_bank[0];
      buffer[size++] = cartridge_souper_chr_bank[1];
      buffer[size++] = cartridge_souper_mode;
      buffer[size++] = cartridge_souper_ram_page_bank[0];
      buffer[size++] = cartridge_souper_ram_page_bank[1];
      for(index = 0; index < sizeof(memory_souper_ram); index++)
         buffer[size + index] = memory_souper_ram[index];
      size += sizeof(memory_souper_ram);
      buffer[size++] = bupchip_GetFlags();
      buffer[size++] = bupchip_GetVolumeValue();
      buffer[size++] = bupchip_GetCurrentSong();
   }
#endif

   return true;
}

bool prosystem_Load(const char *buffer)
{
   uint32_t index;
   char digest[33];
   uint32_t offset = 0;

   printf("Loading game state.\n");

   for(index = 0; index < 16; index++)
   {
      /* File is not a valid ProSystem save state. */
      if(buffer[offset + index] != PRO_SYSTEM_STATE_HEADER[index])
      {
         printf("Buffer is not a valid ProSystem save state.\n");
         return false;
      }
   }
   offset += 16;
   offset++; /* version */

   offset += 4; /* reserved */

   for(index = 0; index < 32; index++)
      digest[index] = buffer[offset + index];
   digest[32] = '\0';

   offset += 32;

   /* Does not match loaded cartridge digest? */
   if(strncmp(cartridge_digest, digest, 32) != 0)
   {
      printf("Load state digest [%s] does not match loaded cartridge digest [%s].\n", digest, cartridge_digest);
      return false;
   }

   sally_a      = buffer[offset++];
   sally_x      = buffer[offset++];
   sally_y      = buffer[offset++];
   sally_p      = buffer[offset++];
   sally_s      = buffer[offset++];
   sally_pc.b.l = buffer[offset++];
   sally_pc.b.h = buffer[offset++];

   cartridge_StoreBank(buffer[offset++]);

   for(index = 0; index < 16384; index++)
      memory_ram[index] = buffer[offset + index];
   offset += 16384;

   if(cartridge_type == CARTRIDGE_TYPE_SUPERCART_RAM)
   {
      for(index = 0; index < 16384; index++)
         memory_ram[16384 + index] = buffer[offset + index];
      offset += 16384;
   }

   /* RIOT */
   riot_dra = buffer[offset++];
   riot_drb = buffer[offset++];
   riot_timing = buffer[offset++] == 1;
   {
      uint8_t h = buffer[offset++];
      uint8_t l = buffer[offset++];
      riot_timer = (uint16_t)((h << 8) | l);
   }
   riot_intervals = buffer[offset++];
   {
      uint8_t h = buffer[offset++];
      uint8_t l = buffer[offset++];
      riot_clocks = (uint16_t)((h << 8) | l);
   }

   /* XM */
   if (cartridge_xm)
   {
      xm_mem_enabled     = buffer[offset++] == 1;
      xm_pokey_enabled   = buffer[offset++] == 1;
      xm_ym_enabled      = buffer[offset++] == 1;
      xm_48kram_enabled  = buffer[offset++] == 1;
      xm_bank0_enabled   = buffer[offset++] == 1;
      xm_bank1_enabled   = buffer[offset++] == 1;
      xm_ramwe_disabled  = buffer[offset++] == 1;
      dma_active         = buffer[offset++] == 1;
      cntrl1 = buffer[offset++];
      cntrl2 = buffer[offset++];
      cntrl3 = buffer[offset++];
      cntrl4 = buffer[offset++];
      cntrl5 = buffer[offset++];
      ym_addr = (uint8_t)buffer[offset++];

      for(index = 0; index < XM_RAM_SIZE; index++)
         xm_ram[index] = buffer[offset + index];
      offset += XM_RAM_SIZE;
   }

   /* POKEY: replay each register write so derived internal state
      (channel dividers, AUDCTL-dependent behavior, etc.) is rebuilt */
   for(index = 0; index < 32; index++)
      pokey_SetRegister(0x4000 + index, (uint8_t)buffer[offset++]);

   /* YM2151: same replay approach */
   for(index = 0; index < 256; index++)
      ym_SetReg(index, (uint8_t)buffer[offset++]);

#ifdef SOUPER
   if(cartridge_type == CARTRIDGE_TYPE_SOUPER)
   {
      cartridge_souper_chr_bank[0] = buffer[offset++];
      cartridge_souper_chr_bank[1] = buffer[offset++];
      cartridge_souper_mode = buffer[offset++];
      cartridge_souper_ram_page_bank[0] = buffer[offset++];
      cartridge_souper_ram_page_bank[1] = buffer[offset++];
      for(index = 0; index < sizeof(memory_souper_ram); index++)
         memory_souper_ram[index] = buffer[offset++];
      bupchip_SetFlags(buffer[offset++]);
      bupchip_SetVolumeValue(buffer[offset++]);
      bupchip_SetCurrentSong(buffer[offset++]);
      bupchip_StateLoaded();
   }
#endif

   printf("%u, %u\n", (unsigned)prosystem_GetStateSize(), offset);

   return true;
}

void prosystem_Close(bool persistent_data)
{
#ifdef SOUPER
   bupchip_Release();
#endif
   cartridge_Release(persistent_data);
   maria_Reset();
   maria_Clear();
   memory_Reset();
   tia_Reset();
   tia_Clear();
}

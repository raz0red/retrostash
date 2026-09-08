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
 * Maria.c
 * ----------------------------------------------------------------------------
 */
#include <stdio.h>

#include "Maria.h"
#include "Equates.h"
#include "Pair.h"
#include "Memory.h"
#include "Sally.h"
#include "Cartridge.h"

#define MARIA_LINERAM_SIZE 160

rect maria_displayArea = {0, 17, 319, 258};
rect maria_visibleArea = {0, 26, 319, 248};

uint8_t maria_surface[MARIA_SURFACE_SIZE] = {0}; // raz: TODO, surface ram size is different

// static uint8_t maria_lineRAM[MARIA_LINERAM_SIZE];
static uint8_t maria_lineRAM_buffers[2][MARIA_LINERAM_SIZE] = {0};
static uint8_t maria_lineRAM_index = 0;
static uint8_t *maria_lineRAM = NULL;

static uint32_t maria_cycles = 0;
static bool color_kill = false;
static pair maria_dpp;
static pair maria_dp;
static pair maria_pp;
static uint8_t maria_horizontal = 0;
static uint8_t maria_palette = 0;
static int8_t maria_offset = 0;
static uint8_t maria_h08 = 0;
static uint8_t maria_h16 = 0;
static uint8_t maria_wmode = 0;

// Whether to access RAM directly
static bool dr = true;
// Whether NMI was triggered
bool nmi = false;
// banksets changes
static bool maria_read = false;

static uint8_t *ram;

static uint8_t ramf(uint16_t address)
{
#ifdef SOUPER
   if (cartridge_type == CARTRIDGE_TYPE_SOUPER)
   {
      uint32_t page, chrOffset;
      if ((cartridge_souper_mode & CARTRIDGE_SOUPER_MODE_MFT) == 0 || address < 0x8000 ||
          ((cartridge_souper_mode & CARTRIDGE_SOUPER_MODE_CHR) == 0 && address < 0xc000))
      {
         return memory_Read(address);
      }
      if (address >= 0xc000) /* EXRAM */
         return memory_Read(address - 0x8000);
      if (address < 0xa000) /* Fixed ROM */
         return memory_Read(address + 0x4000);
      page = (uint16_t)cartridge_souper_chr_bank[(address & 0x80) != 0 ? 1 : 0];
      chrOffset = (((page & 0xfe) << 4) | (page & 1)) << 7;
      return cartridge_LoadROM((address & 0x0f7f) | chrOffset);
   }
#endif

   if (maria_read)
      return memory_ReadMaria(address);
   else
      return memory_Read(address);
}

static void colorKill(uint8_t *buffer, int index)
{
   for (int i = 0; i < 8; i++)
   {
      buffer[index + i] = buffer[index + i] & 0x0f;
   }
}

static void maria_StoreCell1(uint8_t data)
{
   if (maria_horizontal < MARIA_LINERAM_SIZE)
   {
      if (data)
      {
         maria_lineRAM[maria_horizontal] = maria_palette | data;
      }
      else
      {
         uint8_t kmode = ram[CTRL] & 4;
         if (kmode)
         {
            /* Fix for 320D (playsoft) */
            maria_lineRAM[maria_horizontal] = maria_palette;
         }
      }
   }
   maria_horizontal = (maria_horizontal + 1) & 0xFF;
}

static void maria_StoreCell2(uint8_t high, uint8_t low)
{
   // banksets changes
   uint8_t kmode = ram[CTRL] & 4;
   uint8_t c = (maria_palette & 0x10) | high | low;
   if (((c & 3) || kmode) && (maria_horizontal < MARIA_LINERAM_SIZE))
   {
      maria_lineRAM[maria_horizontal] = c;
   }
   maria_horizontal = (maria_horizontal + 1) & 0xFF;
}

static bool maria_IsHolyDMA(void)
{
   if (maria_pp.w > 32767)
   {
      if (maria_h16 && (maria_pp.w & 4096))
      {
         return true;
      }
      if (maria_h08 && (maria_pp.w & 2048))
      {
         return true;
      }
   }
   return false;
}

uint8_t maria_GetColor(uint8_t data)
{
   if (data & 3)
   {
      return ram[BACKGRND + data];
   }
   else
   {
      return ram[BACKGRND];
   }
}

static void maria_StoreGraphic(void)
{
   uint8_t data = (dr ? ram[maria_pp.w] : ramf(maria_pp.w));
   if (maria_wmode)
   {
      if (maria_IsHolyDMA())
      {
         maria_horizontal = (maria_horizontal + 2) & 0xFF;
      }
      else
      {
         maria_StoreCell2((data & 12), (data & 192) >> 6);
         maria_StoreCell2((data & 48) >> 4, (data & 3) << 2);
      }
   }
   else
   {
      if (maria_IsHolyDMA())
      {
         maria_horizontal = (maria_horizontal + 4) & 0xFF;
      }
      else
      {
         maria_StoreCell1((data & 192) >> 6);
         maria_StoreCell1((data & 48) >> 4);
         maria_StoreCell1((data & 12) >> 2);
         maria_StoreCell1(data & 3);
      }
   }
   maria_pp.w++;
}

static void maria_WriteLineRAM(uint8_t *buffer, int offset)
{
   uint8_t rmode = ram[CTRL] & 3;
   int pixel = offset;
   if (rmode == 0)
   {
      // 160A/B
      for (int index = 0; index < MARIA_LINERAM_SIZE; index += 4)
      {
         uint8_t color;
         color = maria_GetColor(maria_lineRAM[index + 0]);
         buffer[pixel++] = color;
         buffer[pixel++] = color;
         color = maria_GetColor(maria_lineRAM[index + 1]);
         buffer[pixel++] = color;
         buffer[pixel++] = color;
         color = maria_GetColor(maria_lineRAM[index + 2]);
         buffer[pixel++] = color;
         buffer[pixel++] = color;
         color = maria_GetColor(maria_lineRAM[index + 3]);
         buffer[pixel++] = color;
         buffer[pixel++] = color;
         if (color_kill)
         {
            colorKill(buffer, pixel - 8);
         }
      }
   }
   else if (rmode == 2)
   {
      // 320B/D
      for (int index = 0; index < MARIA_LINERAM_SIZE; index += 4)
      {
         buffer[pixel++] = maria_GetColor((maria_lineRAM[index + 0] & 16) | ((maria_lineRAM[index + 0] & 8) >> 3) | ((maria_lineRAM[index + 0] & 2)));
         buffer[pixel++] = maria_GetColor((maria_lineRAM[index + 0] & 16) | ((maria_lineRAM[index + 0] & 4) >> 2) | ((maria_lineRAM[index + 0] & 1) << 1));
         buffer[pixel++] = maria_GetColor((maria_lineRAM[index + 1] & 16) | ((maria_lineRAM[index + 1] & 8) >> 3) | ((maria_lineRAM[index + 1] & 2)));
         buffer[pixel++] = maria_GetColor((maria_lineRAM[index + 1] & 16) | ((maria_lineRAM[index + 1] & 4) >> 2) | ((maria_lineRAM[index + 1] & 1) << 1));
         buffer[pixel++] = maria_GetColor((maria_lineRAM[index + 2] & 16) | ((maria_lineRAM[index + 2] & 8) >> 3) | ((maria_lineRAM[index + 2] & 2)));
         buffer[pixel++] = maria_GetColor((maria_lineRAM[index + 2] & 16) | ((maria_lineRAM[index + 2] & 4) >> 2) | ((maria_lineRAM[index + 2] & 1) << 1));
         buffer[pixel++] = maria_GetColor((maria_lineRAM[index + 3] & 16) | ((maria_lineRAM[index + 3] & 8) >> 3) | ((maria_lineRAM[index + 3] & 2)));
         buffer[pixel++] = maria_GetColor((maria_lineRAM[index + 3] & 16) | ((maria_lineRAM[index + 3] & 4) >> 2) | ((maria_lineRAM[index + 3] & 1) << 1));
         if (color_kill)
         {
            colorKill(buffer, pixel - 8);
         }
      }
   }
   else if (rmode == 3)
   {
      // 320A/C
      for (int index = 0; index < MARIA_LINERAM_SIZE; index += 4)
      {
         buffer[pixel++] = maria_GetColor((maria_lineRAM[index + 0] & 30));
         buffer[pixel++] = maria_GetColor((maria_lineRAM[index + 0] & 28) | ((maria_lineRAM[index + 0] & 1) << 1));
         buffer[pixel++] = maria_GetColor((maria_lineRAM[index + 1] & 30));
         buffer[pixel++] = maria_GetColor((maria_lineRAM[index + 1] & 28) | ((maria_lineRAM[index + 1] & 1) << 1));
         buffer[pixel++] = maria_GetColor((maria_lineRAM[index + 2] & 30));
         buffer[pixel++] = maria_GetColor((maria_lineRAM[index + 2] & 28) | ((maria_lineRAM[index + 2] & 1) << 1));
         buffer[pixel++] = maria_GetColor((maria_lineRAM[index + 3] & 30));
         buffer[pixel++] = maria_GetColor((maria_lineRAM[index + 3] & 28) | ((maria_lineRAM[index + 3] & 1) << 1));
         if (color_kill)
         {
            colorKill(buffer, pixel - 8);
         }
      }
   }

   // Apply "composite" smoothing filter, ported from the Atari 7800+
   // libretro-prosystem fork.
   if (cartridge_composite && ((rmode == 2) || (rmode == 3)))
   {
      uint8_t lum1 = 0;
      uint8_t lum2 = 0;
      uint8_t lum3 = 0;
      int pixel_idx;
      lum3 = buffer[offset + 1] & 0x0f;
      for (pixel_idx = offset + 2;
           pixel_idx < (MARIA_LINERAM_SIZE * 2 + offset - 1); pixel_idx += 1)
      {
         lum1 = lum2;
         lum2 = lum3;
         lum3 = buffer[pixel_idx] & 0x0f;
         // a proper composite artifact would shift chroma for even-position
         // bright pixels differently from odd-position bright pixels. We
         // don't do that (1) for performance reasons, (2) because it doesn't
         // actually enhance Tower Toppler, and (3) there's very wide variance
         // in how the shift works, depending on the TV.
         if ((lum1 == 0) && (lum2 > 5) && (lum3 == 0))
         {
            buffer[pixel_idx - 1] =
                (buffer[pixel_idx - 1] & 0xF0) | ((lum2 >> 1) + 1);
            buffer[pixel_idx - 2] = buffer[pixel_idx - 1];
         }
      }
   }
}

static void maria_StoreLineRAM(void)
{
   maria_cycles += 16; // Maria cycles (DMA Startup)

   uint8_t mode = (dr ? ram[maria_dp.w + 1] : ramf(maria_dp.w + 1));
   while ((mode & 0x5f) && (maria_cycles < MARIA_CYCLE_LIMIT))
   {
      uint8_t width = 0;
      uint8_t indirect = 0;

      maria_pp.b.l = ((dr ? ram[maria_dp.w] : ramf(maria_dp.w)));
      maria_pp.b.h = ((dr ? ram[maria_dp.w + 2] : ramf(maria_dp.w + 2)));

      if (mode & 31)
      {
         maria_cycles += 8; // Maria cycles (Header 4 byte)
         maria_palette = (((dr ? ram[maria_dp.w + 1] : ramf(maria_dp.w + 1)) & 224) >> 3) & 0xFF;
         maria_horizontal = (dr ? ram[maria_dp.w + 3] : ramf(maria_dp.w + 3));
         width = (dr ? ram[maria_dp.w + 1] : ramf(maria_dp.w + 1)) & 31;
         width = (((~width) & 31) + 1) & 0xFF;
         maria_dp.w += 4;
      }
      else
      {
         maria_cycles += 10; // Maria cycles (Header 5 byte)
         maria_palette = (((dr ? ram[maria_dp.w + 3] : ramf(maria_dp.w + 3)) & 224) >> 3) & 0xFF;
         maria_horizontal = (dr ? ram[maria_dp.w + 4] : ramf(maria_dp.w + 4));
         indirect = (dr ? ram[maria_dp.w + 1] : ramf(maria_dp.w + 1)) & 32;
         maria_wmode = (dr ? ram[maria_dp.w + 1] : ramf(maria_dp.w + 1)) & 128;
         width = (dr ? ram[maria_dp.w + 3] : ramf(maria_dp.w + 3)) & 31;
         width = ((width == 0) ? 32 : ((~width) & 31) + 1) & 0xFF;
         maria_dp.w += 5;
      }

      bool dma_hole_known = false;

      if (!indirect)
      {
         maria_pp.b.h += maria_offset;
         for (int index = 0; index < width; index++)
         {
            if (maria_cycles >= MARIA_CYCLE_LIMIT)
               break;

            if (maria_IsHolyDMA())
            {
               if (!dma_hole_known)
               {
                  maria_cycles += 3;
                  dma_hole_known = true;
               }
            }
            else
            {
               maria_cycles += 3;
            }
            maria_StoreGraphic();
         }
      }
      else
      {
         uint8_t cwidth = ram[CTRL] & 16;
         pair basePP = maria_pp;
         for (int index = 0; index < width; index++)
         {
            if (maria_cycles >= MARIA_CYCLE_LIMIT)
               break;

            maria_pp.b.l = ((dr ? ram[basePP.w++] : ramf(basePP.w++)));
            maria_pp.b.h = (ram[CHARBASE] + maria_offset);

            if (maria_IsHolyDMA())
            {
               if (!dma_hole_known)
               {
                  maria_cycles += 3;
                  dma_hole_known = true;
               }
            }
            else
            {
               maria_cycles += 6;
               if (cwidth)
               {
                  maria_cycles += 3;
               }
            }

            maria_StoreGraphic(); // Maria cycles (Indirect, 1 byte)
            if (cwidth)
            {
               maria_StoreGraphic();
            }
         }
      }
      mode = (dr ? ram[maria_dp.w + 1] : ramf(maria_dp.w + 1));
   }

   // Last Line post-render DMA cycle penalties...
   if (maria_offset == 0)
   {
      maria_cycles += 6; // extra shutdown time
      if ((dr ? ram[maria_dpp.w + 3] : ramf(maria_dpp.w + 3)) & 128)
      {
         maria_cycles += 17; // interrupt overhead
      }
   }
}

void maria_Reset(void)
{
   maria_lineRAM = maria_lineRAM_buffers[maria_lineRAM_index];
   ram = memory_ram;

   for (int index = 0; index < MARIA_SURFACE_SIZE; index++)
   {
      maria_surface[index] = 0;
   }

   for (int index = 0; index < MARIA_LINERAM_SIZE; index++)
   {
      maria_lineRAM_buffers[0][index] = 0;
      maria_lineRAM_buffers[1][index] = 0;
   }

   maria_cycles = 0;
   color_kill = false;
   maria_dpp.w = 0;
   maria_dp.w = 0;
   maria_pp.w = 0;
   maria_horizontal = 0;
   maria_palette = 0;
   maria_offset = 0;
   maria_h08 = 0;
   maria_h16 = 0;
   maria_wmode = 0;
   nmi = false;
   maria_read = false;
}

uint32_t maria_RenderScanline(uint16_t maria_scanline)
{
   // banksets changes
   if (cartridge_banksets)
      maria_read = true;

   maria_cycles = 0;
   color_kill = (ram[CTRL] & 0x80);
   nmi = false;

   //
   // Displays the background color when Maria is disabled (if applicable)
   //
   if (((ram[CTRL] & 96) != 64) &&
       maria_scanline >= maria_visibleArea.top &&
       maria_scanline <= maria_visibleArea.bottom /*&&
     (!lightgun_enabled || wii_lightgun_flash)*/
   )
   {
      uint8_t bgcolor = maria_GetColor(0);
      uint8_t bgstart_idx = ((maria_scanline - maria_displayArea.top) * Rect_GetLength(&maria_displayArea));
      for (int index = 0; index < MARIA_LINERAM_SIZE; index++)
      {
         maria_surface[bgstart_idx++] = bgcolor;
         maria_surface[bgstart_idx++] = bgcolor;
      }
   }
   else if ((ram[CTRL] & 96) == 64 && maria_scanline >= maria_displayArea.top && maria_scanline <= maria_displayArea.bottom)
   {
      if (maria_scanline == maria_displayArea.top)
      {
         maria_dpp.b.l = (ram[DPPL]);
         maria_dpp.b.h = (ram[DPPH]);
         maria_h08 = (dr ? ram[maria_dpp.w] : ramf(maria_dpp.w)) & 32;
         maria_h16 = (dr ? ram[maria_dpp.w] : ramf(maria_dpp.w)) & 64;
         maria_offset = (dr ? ram[maria_dpp.w] : ramf(maria_dpp.w)) & 15;
         maria_dp.b.l = ((dr ? ram[maria_dpp.w + 2] : ramf(maria_dpp.w + 2)));
         maria_dp.b.h = ((dr ? ram[maria_dpp.w + 1] : ramf(maria_dpp.w + 1)));
         if ((dr ? ram[maria_dpp.w] : ramf(maria_dpp.w)) & 128)
         {
            nmi = true;
         }
      }

      if (maria_scanline >= maria_displayArea.top && maria_scanline != maria_displayArea.bottom)
      {
         maria_dp.b.l = ((dr ? ram[maria_dpp.w + 2] : ramf(maria_dpp.w + 2)));
         maria_dp.b.h = ((dr ? ram[maria_dpp.w + 1] : ramf(maria_dpp.w + 1)));

         maria_lineRAM = maria_lineRAM_buffers[maria_lineRAM_index];
         maria_StoreLineRAM();

         // Swap buffers
         maria_lineRAM_index = (maria_lineRAM_index == 1 ? 0 : 1);
         maria_lineRAM = maria_lineRAM_buffers[maria_lineRAM_index];

         if (maria_scanline >= maria_visibleArea.top && maria_scanline <= maria_visibleArea.bottom)
         {
            maria_WriteLineRAM(maria_surface, ((maria_scanline - maria_displayArea.top) * Rect_GetLength(&maria_displayArea)));
         }

         for (int index = 0; index < MARIA_LINERAM_SIZE; index++)
         {
            maria_lineRAM[index] = 0;
         }

         if (maria_scanline > maria_displayArea.top)
         {
            if (maria_offset == 0)
            {
               maria_dpp.w += 3;
               maria_h08 = (dr ? ram[maria_dpp.w] : ramf(maria_dpp.w)) & 32;
               maria_h16 = (dr ? ram[maria_dpp.w] : ramf(maria_dpp.w)) & 64;
               maria_offset = (dr ? ram[maria_dpp.w] : ramf(maria_dpp.w)) & 15;
               if ((dr ? ram[maria_dpp.w] : ramf(maria_dpp.w)) & 128)
               {
                  nmi = true;
               }
            }
            else
            {
               maria_offset--;
            }
         }
      }
   }

   // banksets changes
   if (cartridge_banksets)
      maria_read = false;

   return maria_cycles;
}

void maria_Clear(void)
{
   for (int index = 0; index < MARIA_SURFACE_SIZE; index++)
   {
      maria_surface[index] = 0;
   }
}

void maria_PostCartLoad(void)
{
   dr = !cartridge_xm && !cartridge_banksets && cartridge_type != CARTRIDGE_TYPE_SOUPER;
   printf("Maria direct memory: %d\n", dr);
}
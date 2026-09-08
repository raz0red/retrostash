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
 * Cartridge.c
 * ----------------------------------------------------------------------------
 */
#include "Cartridge.h"
#include "Database.h"
#include "Equates.h"
#include "Memory.h"
#include "Hash.h"
#include "Pokey.h"
#include "Sally.h"
#include "Region.h"
#include "BupChip.h"
#include <streams/file_stream.h>
#include <stdlib.h>
#include <string.h>
#define CARTRIDGE_SOURCE "Cartridge.cpp"

#define HBLANK_DEFAULT 28

char cartridge_digest[33];
/* WRC: trailing-N digests for the CartList.c fallback lookup below, matching
 * the Atari Plus fork's approach to identifying headerless/oversized real
 * cartridge dumps. */
char cartridge_digest_16k[33];
char cartridge_digest_32k[33];
char cartridge_digest_64k[33];
uint8_t cartridge_type = 0;
uint8_t cartridge_region = 0;
uint8_t cartridge_composite = 0;
bool cartridge_pokey = false;
bool cartridge_pokey450 = false;
uint8_t cartridge_controller[2] = {1, 1};
;
uint8_t cartridge_bank = 0;
uint32_t cartridge_flags = 0;
bool cartridge_xm = false;
#ifdef SOUPER
bool cartridge_bupchip = false;
#endif
uint16_t cartridge_hblank = HBLANK_DEFAULT;

// banksets changes
bool cartridge_banksets = false;
uint32_t cartridge_banksets_begin = 0;
uint32_t cartridge_banksets_end = 0;
bool cartridge_halt_banked_ram = false;
bool cartridge_pokey_write_only = false;
bool cartridge_pokey800 = false;
bool cartridge_pokey_range = false;
uint32_t cartridge_pokey_range_begin = 0;
uint32_t cartridge_pokey_range_end = 0;

/* Per-title compatibility fixes, ported from the Atari 7800+ libretro-prosystem fork */
bool is_bbcq_atari = false;
bool cartridge_adjust_audio = false;
float cartridge_adjust_tia = 1.0f;
float cartridge_adjust_pokey = 1.0f;

static bool is_tiger_heli(const uint8_t *rom, size_t len);

// 0: Disabled, 1: Enabled, 2: Automatic
#define XM_MODE_DEFAULT 2
uint8_t xm_mode = XM_MODE_DEFAULT;

#ifdef SOUPER
/* SOUPER-specific stuff, used for "Rikki & Vikki" */
uint8_t cartridge_souper_chr_bank[2];
uint8_t cartridge_souper_mode;
uint8_t cartridge_souper_ram_page_bank[2];
#endif

uint8_t *cartridge_buffer = NULL;
static uint32_t cartridge_size = 0;
bool cartridge_stored = false;

#ifdef SOUPER
uint8_t cartridge_LoadROM(uint32_t address)
{
   if(address >= cartridge_size)
      return 0;
   return cartridge_buffer[address];
}

static void cartridge_souper_StoreChrBank(uint8_t page, uint8_t bank)
{
   if (page < 2)
      cartridge_souper_chr_bank[page] = bank;
}

static void cartridge_souper_SetMode(uint8_t data)
{
   cartridge_souper_mode = data;
}

static void cartridge_souper_SetRamPageBank(uint8_t which, uint8_t data)
{
   if (which < 2)
      cartridge_souper_ram_page_bank[which] = data & 7;
}

char *cartridge_GetNextNonemptyLine(const char **stream, size_t *size)
{
   while (*size != 0)
   {
      char *line_buffer;
      const char *end;
      const char *line = *stream;
      while (*size > 0 && **stream != '\r' && **stream != '\n')
      {
         (*stream)++;
         (*size)--;
      }

      /* Skip CR/LF. */
      end = *stream;
      while (*size > 0 && (**stream == '\r' || **stream == '\n'))
      {
         (*stream)++;
         (*size)--;
      }

      if (line == end || line[0] == '\n' || line[0] == '\r')
         continue;

      line_buffer = (char *)malloc(end - line + 1);
      memcpy(line_buffer, line, end - line);
      line_buffer[end - line] = '\0';
      return line_buffer;
   }

   return NULL;
}

bool cartridge_ReadFile(uint8_t **outData, size_t *outSize, const char *subpath, const char *relativeTo)
{
   int64_t len = 0;
   size_t pathLen = strlen(subpath) + strlen(relativeTo) + 1;
   char *path = (char *)malloc(pathLen + 1);
#ifdef _WIN32
   char pathSeparator = '\\';
#else
   char pathSeparator = '/';
#endif
   sprintf(path, "%s%c%s", relativeTo, pathSeparator, subpath);

   filestream_read_file(path, (void **)outData, &len);
   *outSize = (size_t)len;
   return len > 0;
}
#endif

/* Detects any Tiger Heli hack/rerelease variant by scanning for encoded
   publisher/developer name strings, since these ROMs don't share a single
   digest. Ported from the Atari 7800+ libretro-prosystem fork. */
static bool is_tiger_heli(const uint8_t *rom, size_t len)
{
   /* Encoded strings (A=1 mapping) */
   static const uint8_t toaplan[]   = {0x14, 0x0F, 0x01, 0x10, 0x0C, 0x01, 0x0E};
   static const uint8_t plaion[]    = {0x10, 0x0C, 0x01, 0x09, 0x0F, 0x0E};
   static const uint8_t tatsujin[]  = {0x14, 0x01, 0x14, 0x13, 0x15, 0x0A, 0x09, 0x0E};
   static const uint8_t steux[]     = {0x13, 0x14, 0x05, 0x15, 0x18};
   static const uint8_t benjones[]  = {0x02, 0x05, 0x0E, 0x20, 0x0A, 0x0F, 0x0E, 0x05, 0x13};

   /* Plain ASCII string */
   static const uint8_t tiger_heli_str[] = {'T', 'i', 'g', 'e', 'r', ' ', 'H', 'e', 'l', 'i'};

   bool found_toaplan = false;
   bool found_plaion = false;
   bool found_tatsujin = false;
   bool found_steux = false;
   bool found_benjones = false;
   bool found_tiger_heli = false;
   size_t i;

   for (i = 0; i < len; i++)
   {
      if (!found_toaplan && i + sizeof(toaplan) <= len &&
          memcmp(&rom[i], toaplan, sizeof(toaplan)) == 0)
         found_toaplan = true;

      if (!found_plaion && i + sizeof(plaion) <= len &&
          memcmp(&rom[i], plaion, sizeof(plaion)) == 0)
         found_plaion = true;

      if (!found_tatsujin && i + sizeof(tatsujin) <= len &&
          memcmp(&rom[i], tatsujin, sizeof(tatsujin)) == 0)
         found_tatsujin = true;

      if (!found_steux && i + sizeof(steux) <= len &&
          memcmp(&rom[i], steux, sizeof(steux)) == 0)
         found_steux = true;

      if (!found_benjones && i + sizeof(benjones) <= len &&
          memcmp(&rom[i], benjones, sizeof(benjones)) == 0)
         found_benjones = true;

      if (!found_tiger_heli && i + sizeof(tiger_heli_str) <= len &&
          memcmp(&rom[i], tiger_heli_str, sizeof(tiger_heli_str)) == 0)
         found_tiger_heli = true;

      if (found_toaplan && found_plaion && found_tatsujin &&
          found_steux && found_benjones && found_tiger_heli)
         return true;
   }

   return false;
}

/* Forces the header's TV-type byte to PAL for a handful of known ROM dumps
   that misreport their region. Ported from the Atari 7800+ libretro-prosystem
   fork's Database.c. */
static void checkPal(const char *digest, uint8_t *header)
{
   if (
      // Pac-Man
      !strcmp(digest, "f47dace752419f4b1622b02f3cbca490") ||
      // Asteroids Deluxe and Space Duel
      !strcmp(digest, "eb8147218f8e6413eb99d656eb2e00c8") ||
      !strcmp(digest, "eeca90944949f047805d8da3bc89a994") ||
      // Asteroids Deluxe
      !strcmp(digest, "aac5902912901d7f9c4d99ac372ec972") ||
      // Bounty Bob Strikes Back (PAL)
      !strcmp(digest, "ba0d50c8617537ae45e7df4d299fbfe0") ||
      // Crazy Brix
      !strcmp(digest, "28297d8802f3ca83b6872fc71083b859") ||
      // Moon Cresta
      !strcmp(digest, "5cee00541197fe047b13c25d19dda202") ||
      // Pac-Man Collection
      !strcmp(digest, "29e32171cac88b05f80a0ff9cf7d5829") ||
      // Scramble
      !strcmp(digest, "60d577872dba2da400ab5fb2d2a87b71") ||
      // Space Duel
      !strcmp(digest, "a4f35435de2a135ac26a3a96b1317a5b") ||
      // Space Invaders
      !strcmp(digest, "b2c67409454c1010595d81f20d07c86d")
   )
   {
      printf("Found cartridge in PAL list, forcing...\n");
      header[0x39] |= 0x01; /* TV_PAL */
   }
}

static bool cartridge_HasHeader(const uint8_t *header)
{
   unsigned index;
   const char HEADER_ID[] = {"ATARI7800"};

   for (index = 0; index < 9; index++)
   {
      if (HEADER_ID[index] != header[index + 1])
         return false;
   }
   return true;
}

/* ----------------------------------------------------------------------------
 * Header for CC2 hack
 * ----------------------------------------------------------------------------
 */
static bool cartridge_CC2(const uint8_t *header)
{
   unsigned index;
   const char HEADER_ID[] = {">>"};

   for (index = 0; index < 2; index++)
   {
      if (HEADER_ID[index] != header[index + 1])
         return false;
   }
   return true;
}

static uint32_t cartridge_GetBank(uint8_t bank)
{
   if ((cartridge_type == CARTRIDGE_TYPE_SUPERCART ||
        cartridge_type == CARTRIDGE_TYPE_SUPERCART_ROM ||
        cartridge_type == CARTRIDGE_TYPE_SUPERCART_RAM) &&
       cartridge_size <= 65536)
   {
      // for some of these carts, there are only 4 banks. in this case we ignore bit 3
      // previously, games of this type had to be doubled. The first 4 banks needed to be duplicated at the end of the ROM
      return (bank & 3);
   }
   return bank;
}

static uint32_t cartridge_GetBankOffset(uint8_t bank)
{
   return cartridge_GetBank(bank) * 16384;
}

static void cartridge_WriteBank(uint16_t address, uint8_t bank)
{
   // banksets changes
   uint32_t size = cartridge_size;
   if (cartridge_banksets)
      size = size >> 1;

   uint32_t offset = cartridge_GetBankOffset(bank);
   if (offset < size)
   {
      memory_WriteROM(address, 16384, cartridge_buffer, offset);
      cartridge_bank = bank;
   }
}

static void cartridge_SetTypeBySize(uint32_t size)
{
   // banksets changes
   if (cartridge_banksets)
      size = size >> 1;

   if (size <= 0x10000)
   {
      uint8_t old_type = cartridge_type;
      cartridge_type = CARTRIDGE_TYPE_NORMAL;
      printf("Update: no bits and <= 64k: %d, %d\n", old_type, cartridge_type);
   }
   else if (size == 0x24000)
   {
      uint8_t old_type = cartridge_type;
      cartridge_type = CARTRIDGE_TYPE_SUPERCART_LARGE;
      printf("Update: size == 144k: %d, %d\n", old_type, cartridge_type);
   }
   else if (size == 0x20000)
   {
      uint8_t old_type = cartridge_type;
      cartridge_type = CARTRIDGE_TYPE_SUPERCART_ROM;
      printf("Update: size == 128k: %d, %d\n", old_type, cartridge_type);
   }
   else
   {
      uint8_t old_type = cartridge_type;
      cartridge_type = CARTRIDGE_TYPE_SUPERCART;
      printf("Update: default for > 64k: %d, %d\n", old_type, cartridge_type);
   }
}

static void cartridge_ReadHeader(const uint8_t *header)
{
   printf("Reading cartridge header\n");

   cartridge_size = header[49] << 24;
   cartridge_size |= header[50] << 16;
   cartridge_size |= header[51] << 8;
   cartridge_size |= header[52];

   if (header[53] == 0)
   {
      if (cartridge_size > 131072)
         cartridge_type = CARTRIDGE_TYPE_SUPERCART_LARGE;
      else if (header[54] == 2 || header[54] == 3)
         cartridge_type = CARTRIDGE_TYPE_SUPERCART;
      else if (header[54] == 4 || header[54] == 5 || header[54] == 6 || header[54] == 7)
         cartridge_type = CARTRIDGE_TYPE_SUPERCART_RAM;
      else if (header[54] == 8 || header[54] == 9 || header[54] == 10 || header[54] == 11)
         cartridge_type = CARTRIDGE_TYPE_SUPERCART_ROM;
      else
         cartridge_type = CARTRIDGE_TYPE_NORMAL;
   }
   else
   {
      if (header[53] & 0x02) /* raz, updated */
         cartridge_type = CARTRIDGE_TYPE_ABSOLUTE;
      else if (header[53] & 0x01) /* raz, updated */
         cartridge_type = CARTRIDGE_TYPE_ACTIVISION;
      else if (header[53] == 16)
         cartridge_type = CARTRIDGE_TYPE_SOUPER;
      else
         cartridge_type = CARTRIDGE_TYPE_NORMAL;
   }

   cartridge_pokey = (header[54] & 1) ? true : false;
   cartridge_pokey450 = (header[54] & 0x40) ? true : false;
   // banksets changes
   cartridge_pokey800 = (header[53] & 0x80) ? true : false;
   if (cartridge_pokey800)
   {
      cartridge_pokey_range = true;
      cartridge_pokey_range_begin = 0x800;
      cartridge_pokey_range_end = 0xFFF;
   }
   if (cartridge_pokey450 || cartridge_pokey800)
   {
      cartridge_pokey = true;
   }

   cartridge_controller[0] = header[55];
   cartridge_controller[1] = header[56];
   cartridge_region = header[57] & 0x1;
   cartridge_composite = (header[57] & 0x2) ? 1 : 0;
   cartridge_flags = 0;
   // banksets changes (check for 0x08, ym2151)
   cartridge_xm = (header[63] & 1) || ((header[53] & 0x08) == 0x08) ? true : false;
   // banksets changes
   cartridge_banksets = header[53] & 0x20 ? true : false;
   if (cartridge_banksets &&
       (cartridge_size == (2 * 48 * 1024) || cartridge_size == (2 * 52 * 1024)))
   {
      cartridge_pokey_write_only = true;
   }
   cartridge_halt_banked_ram = header[53] & 0x40 ? true : false;
#ifdef SOUPER
   cartridge_bupchip = false;
#endif

   uint8_t ct1 = header[54];
   uint8_t ct2 = header[53];
   if (header[53] & 0x02)
   { /* raz, updated */
      cartridge_type = CARTRIDGE_TYPE_ABSOLUTE;
   }
   else if (header[53] & 0x01)
   { /* raz, updated */
      cartridge_type = CARTRIDGE_TYPE_ACTIVISION;
   }
   else if (header[53] & 0x10)
   { /* raz, updated */
      cartridge_type = CARTRIDGE_TYPE_SOUPER;
   }
   else if ((ct1 & 0x0a) == 0x0a)
   { // BIT1 and BIT3 (Supercart Large: 2) rom at $4000
      uint8_t old_type = cartridge_type;
      cartridge_type = CARTRIDGE_TYPE_SUPERCART_LARGE;
      printf("Update: (0x10) bit1 & bit3: %d, %d\n", old_type, cartridge_type);
   }
   else if ((ct1 & 0x12) == 0x12)
   { // BIT1 and BIT4 (Supercart ROM: 4) bank6 at $4000
      uint8_t old_type = cartridge_type;
      cartridge_type = CARTRIDGE_TYPE_SUPERCART_ROM;
      printf("Update: (0x12) bit1 & bit4: %d, %d\n", old_type, cartridge_type);
   }
   else if ((ct1 & 0x06) == 0x06)
   { // BIT1 and BIT2 (Supercart RAM: 3) ram at $4000
      uint8_t old_type = cartridge_type;
      cartridge_type = CARTRIDGE_TYPE_SUPERCART_RAM;
      printf("Update: (0x06) bit1 & bit2: %d, %d\n", old_type, cartridge_type);
   }
   else if ((ct1 & 0x02) == 0x02)
   { // BIT1 (Supercart) bank switched
      uint8_t old_type = cartridge_type;
      cartridge_type = CARTRIDGE_TYPE_SUPERCART;
      printf("Update: (0x01) bit1: %d, %d\n", old_type, cartridge_type);
   }
   else if (cartridge_size <= 0x10000 &&
            ((ct1 & 0x04) == 0x04))
   { // Size < 64k && BIT2 (Normal RAM: ?) ram at $4000 )
      uint8_t old_type = cartridge_type;
      cartridge_type = CARTRIDGE_TYPE_NORMAL_RAM;
      printf("Update: (0x04) bit2: %d, %d\n", old_type, cartridge_type);
   }
   else
   {
      // Attempt to determine the cartridge type based on its size
      cartridge_SetTypeBySize(cartridge_size);
   }

   // banksets changes
   if (cartridge_banksets)
   {
      if (cartridge_type == CARTRIDGE_TYPE_NORMAL ||
          cartridge_type == CARTRIDGE_TYPE_NORMAL_RAM)
      {
         if (cartridge_type == CARTRIDGE_TYPE_NORMAL &&
             cartridge_halt_banked_ram)
         {
            uint8_t old_type = cartridge_type;
            cartridge_type = CARTRIDGE_TYPE_NORMAL_RAM;
            printf(
                "Normal cart with halt based ram, switching type: %d, %d\n",
                old_type, cartridge_type);
         }
      }
      else
      {
         uint8_t old_type = cartridge_type;
         if (cartridge_halt_banked_ram)
         {
            cartridge_type = CARTRIDGE_TYPE_SUPERCART_RAM;
         }
         else if ((ct1 & 0x10) == 0x10)
         {
            cartridge_type = CARTRIDGE_TYPE_SUPERCART_ROM;
         }
         else
         {
            cartridge_type = CARTRIDGE_TYPE_SUPERCART;
         }
         if (old_type != cartridge_type)
         {
            printf("Bank switched banksets, switching type: %d, %d\n",
                   old_type, cartridge_type);
         }
      }
   }

   // super game bank switched and nothing at $4000
   if ((ct1 & 0x02) && (!((ct1 & 0x01) ||  // pokey at $4000
                          (ct1 & 0x04) ||  // supergame ram at $4000
                          (ct1 & 0x08) ||  // rom at $4000
                          (ct1 & 0x10) ||  // bank 6 at $4000
                          (ct1 & 0x80) ||       // mirror ram at $4000
                          (cartridge_banksets)  // banksets
                          ))) {
       printf(
           "Super game bank switched, and nothing at $4000, enabling bank 6 at "
           "$4000. (cartridge_type switch to 4).\n");
       cartridge_type = CARTRIDGE_TYPE_SUPERCART_ROM;
   }

   printf("Header info:\n");
   if (ct1 & 0x01)
   {
      printf("  bit0: pokey at $4000\n");
   }
   if (ct1 & 0x02)
   {
      printf("  bit1: supergame bank switched\n");
   }
   if (ct1 & 0x04)
   {
      printf("  bit2: supergame ram at $4000\n");
   }
   if (ct1 & 0x08)
   {
      printf("  bit3: rom at $4000\n");
   }
   if (ct1 & 0x10)
   {
      printf("  bit4: bank 6 at $4000\n");
   }
   if (ct1 & 0x20)
   {
      printf("  bit5: supergame banked ram\n");
   }
   if (ct1 & 0x40)
   {
      printf("  bit6: pokey at $450\n");
   }
   if (ct1 & 0x80)
   {
      printf("  bit7: mirror ram at $4000\n");
   }
   // banksets changes
   if (ct2 & 0x01)
   {
      printf("  bit8: activision banking\n");
   }
   if (ct2 & 0x02)
   {
      printf("  bit9: absolute banking\n");
   }
   if (ct2 & 0x04)
   {
      printf("  bit10: pokey at $440\n");
   }
   if (ct2 & 0x08)
   {
      printf("  bit11: ym2151 at $460/$461\n");
   }
   if (ct2 & 0x10)
   {
      printf("  bit12: souper\n");
   }
   if (ct2 & 0x20)
   {
      printf("  bit13: banksets\n");
   }
   if (ct2 & 0x40)
   {
      printf("  bit14: halt banked ram\n");
   }
   if (ct2 & 0x80)
   {
      printf("  bit15: pokey@800\n");
   }

   printf("  xm: %s\n", (cartridge_xm ? "1" : "0"));
   printf("  banksets: %s\n", (cartridge_banksets ? "1" : "0"));
   printf("  pokey: %s\n", (cartridge_pokey ? "1" : "0"));
   printf("  pokey450: %s\n", (cartridge_pokey450 ? "1" : "0"));
   printf("  pokey800: %s\n", (cartridge_pokey800 ? "1" : "0"));
   printf("  pokey write only: %s\n",
          cartridge_pokey_write_only ? "1" : "0");
   printf("  halt banked ram: %s\n", cartridge_halt_banked_ram ? "1" : "0");
   printf("  tv type: %s\n", cartridge_region ? "PAL" : "NTSC");
   printf("  Save device: [%d]%s%s\n", header[58],
          ((header[58] & 0x02) ? " SaveKey/AtariVox" : ""),
          ((header[58] & 0x01) ? " HSC" : ""));
   printf("  controller1: %d\n", cartridge_controller[0]);
   printf("  controller2: %d\n", cartridge_controller[1]);
   printf("  cartridge_type 53: %d\n", header[53]);
   printf("  cartridge_type 54: %d\n", header[54]);
   printf("  cartridge_size: %d\n", cartridge_size);
   printf("cartridge_type (from header): %d\n", cartridge_type);
}

#ifdef SOUPER
bool cartridge_LoadFromCDF(const char *data, size_t size,
                           const char *workingDir)
{
   static const char *cartridgeTypes[] = {
       "EMPTY",
       "SUPER",
       NULL,
       NULL,
       NULL,
       "ABSOLUTE",
       "ACTIVISION",
       "SOUPER",
   };
   int i;
   char *line;
   size_t cart_size;
   if ((line = cartridge_GetNextNonemptyLine(&data, &size)) == NULL)
      return false;
   if (strcmp(line, "ProSystem") != 0)
      return false;
   free(line);

   if ((line = cartridge_GetNextNonemptyLine(&data, &size)) == NULL)
      return false;

   for (i = 0; i < sizeof(cartridgeTypes) / sizeof(cartridgeTypes[0]); i++)
   {
      if (cartridgeTypes[i] != NULL && strcmp(line, cartridgeTypes[i]) == 0)
      {
         cartridge_type = i;
         break;
      }
   }
   free(line);

   if ((line = cartridge_GetNextNonemptyLine(&data, &size)) == NULL)
      return false;
   /* Just ignore the cartridge title in `libretro`. */
   free(line);

   /* Read binary file. */
   if ((line = cartridge_GetNextNonemptyLine(&data, &size)) == NULL)
      return false;

   if (!cartridge_ReadFile(&cartridge_buffer, &cart_size, line, workingDir))
      return false;
   free(line);

   cartridge_size = (uint32_t)cart_size;
   hash_Compute(cartridge_digest, cartridge_buffer, cart_size);

   cartridge_bupchip = false;
   if ((line = cartridge_GetNextNonemptyLine(&data, &size)))
   {
      cartridge_bupchip = strcmp(line, "CORETONE") == 0;
      free(line);
   }

   if (cartridge_bupchip)
   {
      if (!bupchip_InitFromCDF(&data, &size, workingDir))
      {
         free(cartridge_buffer);
         return false;
      }
   }
   return true;
}
#endif

bool cartridge_Load(bool persistent_data, const uint8_t *data, uint32_t size)
{
   int index;
   uint32_t offset = 0;
   uint8_t header[128] = {0};

   /* Cartridge data is invalid. */
   if (size <= 128)
   {
      printf("Cartridge data is invalid.\n");
      return false;
   }

   printf("Actual cartridge size: %d\n", size);

   for (index = 0; index < 128; index++)
      header[index] = data[index];

   /* Prosystem doesn't support CC2 hacks. */
   if (cartridge_CC2(header))
   {
      printf("Prosystem doesn't support CC2 hacks.\n");
      return false;
   }

   // See if the cartridge has a header
   bool has_header = cartridge_HasHeader(header);
   if (has_header) {
      size -= 128;
      offset = 128;
   }

   // Allocate cartridge buffer
   if (persistent_data)
      cartridge_buffer = (uint8_t *)data + offset;
   else
   {
      cartridge_buffer = (uint8_t *)malloc(size * sizeof(uint8_t));
      for (index = 0; index < size; index++)
         cartridge_buffer[index] = data[index + offset];
   }

   // Compute the hash
   hash_Compute(cartridge_digest, cartridge_buffer, size);
   printf("Cartridge digest: %s\n", cartridge_digest);

   // WRC: trailing-N digests, for the CartList.c fallback lookup below.
   // Cleared first so a smaller cart this load doesn't inherit a stale
   // digest computed for a larger cart on a previous load.
   cartridge_digest_16k[0] = '\0';
   cartridge_digest_32k[0] = '\0';
   cartridge_digest_64k[0] = '\0';
   if (size >= 16 * 1024)
   {
      hash_Compute(cartridge_digest_16k, cartridge_buffer + (size - 16 * 1024), 16 * 1024);
      printf("Cartridge digest (16k): %s\n", cartridge_digest_16k);
   }
   if (size >= 32 * 1024)
   {
      hash_Compute(cartridge_digest_32k, cartridge_buffer + (size - 32 * 1024), 32 * 1024);
      printf("Cartridge digest (32k): %s\n", cartridge_digest_32k);
   }
   if (size >= 64 * 1024)
   {
      hash_Compute(cartridge_digest_64k, cartridge_buffer + (size - 64 * 1024), 64 * 1024);
      printf("Cartridge digest (64k): %s\n", cartridge_digest_64k);
   }

   // Lookup the hash in the database
   bool has_db_header = database_Lookup(cartridge_digest, size, header);

   // WRC: fall back to the CartList.c database (known commercial/retail
   // cartridge dumps, ported from the Atari 2600+/7800+ fork) when our own
   // smaller db_list[] above doesn't have this cart. This is what makes
   // headerless dumps (e.g. No-Intro sets) work correctly.
   if (!has_db_header)
      has_db_header = cartlist_Lookup(cartridge_digest, cartridge_digest_64k,
                                       cartridge_digest_32k, cartridge_digest_16k,
                                       size, header);

   // Several ROM dumps misreport their TV type; force the known-correct ones to PAL.
   checkPal(cartridge_digest, header);

   if (has_header || has_db_header)
   {
      if (!has_db_header) {
      printf("Found cartridge header\n");
      } else {
         printf("Found cartridge header in database.\n");
      }

      // Read the header
      cartridge_ReadHeader(header);

      // Several cartridge headers do not have the proper size. So attempt to
      // use the size of the file.
      if (cartridge_size != size)
      {
         printf("!!! CARTRIDGE SIZE IN HEADER DOES NOT MATCH !!! : %d %d\n", cartridge_size, size);
         // Necessary for the following roms:
         // Impossible Mission hacks w/ C64 style graphics
         if (size % 1024 == 0)
         {
            printf("!!! ROM size is 1k multiple, using ROM size !!! : %d\n", size);
            cartridge_size = size;
         }
         else
         {
            printf("!!! ROM size is not 1k multiple, using header size !!! : %d\n", cartridge_size);
         }
      }
   }
   else
   {
      printf("Unable to find cartridge header\n");
      cartridge_size = size;
      // Attempt to guess the cartridge type based on its size
      cartridge_SetTypeBySize(size);
   }

   printf("cartridge_type: %d\n", cartridge_type);
   printf("cartridge_size: %d\n", cartridge_size);

   if (cartridge_size != size) {
      if (!persistent_data) {
         free(cartridge_buffer);
      cartridge_buffer = (uint8_t *)malloc(cartridge_size * sizeof(uint8_t));

      for (index = 0; index < cartridge_size; index++)
         cartridge_buffer[index] = data[index + offset];
   }

   hash_Compute(cartridge_digest, cartridge_buffer, cartridge_size);
      printf("Updated cartridge digest: %s\n", cartridge_digest);
   }

   // Diagnostic cartridge
   if (!strcmp(cartridge_digest, "91041aadd1700a7a4076f4005f2c362f") ||
       !strcmp(cartridge_digest, "66a90a41b11faa0f6d1e05aefb59ec6f") ||
       !strcmp(cartridge_digest, "c6a4661e3f1eb57e14b966d1497a2412"))
   {
      printf("Patching diagnostic cartridge...\n");
      int diag_offset = cartridge_size - 0x1914 - 128;
      cartridge_buffer[diag_offset + 0] = 0xDF; // raz fix
      cartridge_buffer[diag_offset + 1] = 0xE6; // raz fix
   }

   /* Per-title compatibility fixes, ported from the Atari 7800+
      libretro-prosystem fork. */
   if (cartridge_size >= (16 * 1024))
   {
      uint16_t crc16 = hash_CRC16(cartridge_buffer + (cartridge_size - (16 * 1024)), 1024);
      printf("CRC 16: 0x%x\n", crc16);

      // Pole Position II hack
      if ((crc16 & 0xFFFF) == 0x9e8a)
      {
         printf("Applying Pole Position II hack...\n");
         prosystem_mstat_adjust = 3;
      }
   }

   if (is_tiger_heli(cartridge_buffer, cartridge_size))
   {
      printf("Tiger Heli (Atari)\n");
      cartridge_adjust_audio = true;
      cartridge_adjust_pokey = 0.7f;
      cartridge_adjust_tia = 0.7f;
   }

   // Bentley Bear
   if (!strcmp(cartridge_digest, "34483432b92f565f4ced82a141119164") || // AA NTSC (Trebor's pack)
       !strcmp(cartridge_digest, "1926b9b322ac0f8f36e119b524aa48bd"))   // AA PAL  (Trebor's pack)
   {
      is_bbcq_atari = true;
   }

   // Knight Guy fix
   if (!strcmp(cartridge_digest, "5d9b6a8fd552b54fdcb1a3b10fe54006") ||
       !strcmp(cartridge_digest, "a6f24db7b9baf8ebe4eb7de0d615595d"))
   {
      disable_sed = true;
   }

   // Klax fix
   if (!strcmp(cartridge_digest, "17b3b764d33eae9b5260f01df7bb9d2f") ||
       !strcmp(cartridge_digest, "14256da558bfa8184a2aa9c2c53840f5") ||
       !strcmp(cartridge_digest, "9de4ecd1cad6bedfc580db48bc6441f4"))
   {
      printf("Applying KLAX Fix...\n");
      static const uint8_t klax_p1[] = {
         0x60, 0x85, 0x60, 0xa9, 0x00, 0x85, 0x20, 0x20, 0x33, 0xff, 0xa9, 0x01, 0x20, 0xdf, 0xfe, 0xa9,
         0x01, 0x20, 0xdf, 0xfe, 0xa9, 0x78, 0x85, 0x34, 0xa9, 0x00, 0x85, 0xfd, 0x85, 0xfc, 0x58, 0x20,
         0x5a, 0xff
      };
      memcpy(cartridge_buffer + 0x1cb0d - 128, klax_p1, sizeof(klax_p1));
      static const uint8_t klax_p2[] = {
         0x20, 0x49, 0xcc, 0xa9, 0x48, 0x85, 0x3c, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xf4, 0xe2, 0xba, 0x19, 0xa1, 0x4b, 0xd6, 0x4f, 0xb8,
         0x68, 0x15, 0x98, 0xfa, 0x16, 0xf3, 0xbc, 0xcf, 0x1e, 0xfd, 0x8e, 0x87, 0x10, 0xbb, 0x67, 0x8a,
         0xda, 0x94, 0x25, 0x96, 0x67, 0x6f, 0x0b, 0xe5, 0x51, 0x2b, 0x1c, 0x12, 0x88, 0x90, 0x62, 0xf6,
         0xab, 0x53, 0x3b, 0x54, 0xcd, 0x27, 0x06, 0xa9, 0x15, 0x66, 0x75, 0xd3, 0xdf, 0x79, 0x64, 0x70,
         0x56, 0x7f, 0x35, 0xbf, 0x78, 0x6c, 0xf4, 0x54, 0x13, 0x9a, 0x74, 0x13, 0x04, 0x97, 0x27, 0xe3,
         0xfe, 0x6e, 0x53, 0x44, 0xc2, 0xbc, 0x66, 0x25, 0xb3, 0x92, 0x23, 0x41, 0x40, 0x4e, 0x81, 0x57,
         0xb6, 0x93, 0xad, 0xc5, 0xbc, 0x19, 0xd1, 0x54, 0x22, 0x2e, 0xf2, 0x1a, 0x3c, 0xd4, 0xdf, 0xfe,
         0x84, 0x77, 0x38, 0xee, 0x78, 0x1e, 0x5d, 0x35, 0xb5, 0xba, 0x4a, 0x34, 0x40, 0x87, 0xff, 0xc7,
         0xaa, 0xc9, 0x30, 0xca, 0x26, 0xca
      };
      memcpy(cartridge_buffer + 0x1ffda - 128, klax_p2, sizeof(klax_p2));

      hash_Compute(cartridge_digest, cartridge_buffer, cartridge_size);
      printf("Cartridge digest (after KLAX fix): %s\n", cartridge_digest);
   }

   // Joust (PAL)
   if (!strcmp(cartridge_digest, "f2dae0264a4b4a73762b9d7177e989f6"))
   {
      printf("Patching Joust (PAL)\n");
      cartridge_buffer[0x099fc - 128] = 0xe0;
   }

   // Xevious (PAL)
   if (!strcmp(cartridge_digest, "b1a9f196ce5f47ca8caf8fa7bc4ca46c"))
   {
      printf("Patching Xevious (PAL)\n");
      cartridge_buffer[0x6b1b - 128] = 0xa6;
   }

   // Dig Dug (PAL)
   if (!strcmp(cartridge_digest, "408dca9fc40e2b5d805f403fa0509436"))
   {
      printf("Patching Dig Dug (PAL)\n");
      static const struct { uint32_t offset; uint8_t value; } digdug_patch[] = {
         {0x0178, 0x4c}, {0x0179, 0x30}, {0x017a, 0x81}, {0x017b, 0x4c},
         {0x017c, 0x47}, {0x017d, 0x81}, {0x017e, 0x00}, {0x017f, 0x00},
         {0x0180, 0x00}, {0x0181, 0x00}, {0x0182, 0x00}, {0x0183, 0x00},
         {0x0184, 0x00}, {0x0185, 0x00}, {0x0186, 0x00}, {0x0187, 0x00},
         {0x0188, 0x00}, {0x0189, 0x00}, {0x018a, 0x00}, {0x018b, 0x00},
         {0x018c, 0x00}, {0x018d, 0x00}, {0x018e, 0x00}, {0x018f, 0x00},
         {0x0190, 0x00}, {0x0191, 0x00}, {0x0192, 0x00}, {0x0193, 0x00},
         {0x0194, 0x00}, {0x0195, 0x00}, {0x0196, 0x00}, {0x0197, 0x00},
         {0x0198, 0x00}, {0x0199, 0x00}, {0x019a, 0x00}, {0x019b, 0x00},
         {0x019c, 0x00}, {0x019d, 0x00}, {0x019e, 0x00},
         {0x01b0, 0x48}, {0x01b1, 0xa9}, {0x01b2, 0x50}, {0x01b3, 0x85},
         {0x01b4, 0x3c}, {0x01b5, 0xa9}, {0x01b6, 0xe0}, {0x01b7, 0x85},
         {0x01b8, 0x34}, {0x01b9, 0xee}, {0x01ba, 0x22}, {0x01bb, 0x23},
         {0x01bc, 0xad}, {0x01bd, 0x22}, {0x01be, 0x23}, {0x01bf, 0x85},
         {0x01c0, 0x24}, {0x01c1, 0x85}, {0x01c2, 0x3b}, {0x01c3, 0xa9},
         {0x01c4, 0xfb}, {0x01c5, 0xd0}, {0x01c6, 0x0b}, {0x01c7, 0x48},
         {0x01c8, 0xa9}, {0x01c9, 0x4b}, {0x01ca, 0x85}, {0x01cb, 0x3c},
         {0x01cc, 0xa9}, {0x01cd, 0x39}, {0x01ce, 0x85}, {0x01cf, 0x34},
         {0x01d0, 0xa9}, {0x01d1, 0xf8}, {0x01d2, 0x8d}, {0x01d3, 0xbe},
         {0x01d4, 0x1f}, {0x01d5, 0x68}, {0x01d6, 0x40},
      };
      size_t digdug_i;
      for (digdug_i = 0; digdug_i < sizeof(digdug_patch) / sizeof(digdug_patch[0]); digdug_i++)
         cartridge_buffer[digdug_patch[digdug_i].offset - 128] = digdug_patch[digdug_i].value;
   }

   // Drone Patrol and EXO Collector's Edition
   if (!strcmp(cartridge_digest, "af9db007c33e7b1d1d083e62401e67e2") ||
       !strcmp(cartridge_digest, "1bf88dec6ba40a78518212b5f688fde5") ||
       !strcmp(cartridge_digest, "5816449c656d76d19dd7b22a9fb05ce8"))
   {
      cartridge_pokey_range = true;
      cartridge_pokey_range_begin = 0xC000;
      cartridge_pokey_range_end = 0xFFFF;
   }

   /* Popeye (Final 2.4, HSCart POKEY test builds). All of these share an
      identical embedded header that declares POKEY at $450, but the actual
      game code for these specific test builds writes POKEY registers at a
      different fixed-bank address instead - confirmed by diffing the ROMs
      directly (STA $0808/$080F, STA $C008/$C00F, and STA $C058/$C05F). */
   // Popeye_Final_2.4_HSCart_pokey800.a78 (POKEY at $800)
   if (!strcmp(cartridge_digest, "bb0014deace539a78d81ac8f3c04c64e"))
   {
      cartridge_pokey800 = true;
      cartridge_pokey_range = true;
      cartridge_pokey_range_begin = 0x800;
      cartridge_pokey_range_end = 0xFFF;
   }
   // Popeye_Final_2.4_HSCart_pokeyC000.a78 and _pokeyC050.a78 (POKEY in the
   // fixed high bank)
   if (!strcmp(cartridge_digest, "86e84049787a2765797218c5b844557c") ||
       !strcmp(cartridge_digest, "892058513c47b9e2815b649f8960b4f2"))
   {
      cartridge_pokey_range = true;
      cartridge_pokey_range_begin = 0xC000;
      cartridge_pokey_range_end = 0xFFFF;
   }

   // Pac-Man palette hack
   if (!strcmp(cartridge_digest, "f47dace752419f4b1622b02f3cbca490") ||
       !strcmp(cartridge_digest, "a9792abc71547a8793268e00690de0a8"))
   {
      force_original_palette = 1;
   }

   // Swap hi and lo
   if (
      // Double Dragon (OM) (NTSC) (Activision) (1989) (F20773D5).a78
      !strcmp(cartridge_digest, "426298bcc76405aa7b58a9858d828db5") ||
      // Rampage (OM) (NTSC) (Activision) (1989) (D2876EE2).a78
      !strcmp(cartridge_digest, "825126724d520ecbdcb096201420dc39") ||
      // Double Dragon (OM) (PAL) (Activision) (1989) (4D634BF5).a78
      !strcmp(cartridge_digest, "d3477fc0f7a42a8a5e69aa3de2c38062")
   ) {
      uint8_t *swap = malloc(8 * 1024);
      printf("Swap hi and lo 8k...\n");
      for (int i = 0; i < cartridge_size; i += 16 * 1024) {
         printf("swap %d\n", i);
         for (int j = 0; j < 8 * 1024; j++) {
            swap[j] = cartridge_buffer[i + j];
         }
         for (int j = 0; j < 8 * 1024; j++) {
            cartridge_buffer[i + j] = cartridge_buffer [i + (8 * 1024) + j];
         }
         for (int j = 0; j < 8 * 1024; j++) {
            cartridge_buffer[i + (8 * 1024) + j] = swap[j];
         }
      }
      free(swap);
   }

#ifdef SOUPER
   /* Combined "Rikki & Vikki" dump: base ROM (512K) followed by an
      appended 512K blob of BupChip sample/instrument/song data. */
   printf("[bupchip debug] cartridge_type=%d (SOUPER=%d) cartridge_size=%u (1MB=%u)\n",
          cartridge_type, CARTRIDGE_TYPE_SOUPER, cartridge_size, (unsigned)(1024 * 1024));
   if (cartridge_type == CARTRIDGE_TYPE_SOUPER && cartridge_size == 1024 * 1024)
   {
      printf("[bupchip debug] combined dump detected, extracting music blob at offset %d, size %d\n",
             512 * 1024, bupchip_GetMusicSize());
      cartridge_bupchip = true;
      memcpy(bupchip_GetMusicBuffer(), cartridge_buffer + (512 * 1024), bupchip_GetMusicSize());
      printf("[bupchip debug] first 8 bytes of music blob: %02x %02x %02x %02x %02x %02x %02x %02x\n",
             bupchip_GetMusicBuffer()[0], bupchip_GetMusicBuffer()[1], bupchip_GetMusicBuffer()[2],
             bupchip_GetMusicBuffer()[3], bupchip_GetMusicBuffer()[4], bupchip_GetMusicBuffer()[5],
             bupchip_GetMusicBuffer()[6], bupchip_GetMusicBuffer()[7]);
      bupchip_Unpack();
      printf("[bupchip debug] cartridge_bupchip=%d\n", cartridge_bupchip);
   }
   else
   {
      printf("[bupchip debug] combined dump NOT detected, cartridge_bupchip will stay false\n");
   }
#endif

   return true;
}

void cartridge_Store(void)
{
   // banksets changes
   cartridge_stored = true;
   uint32_t size = cartridge_size;
   if (cartridge_banksets)
      size = size >> 1;

   switch (cartridge_type)
   {
   case CARTRIDGE_TYPE_NORMAL:
      memory_WriteROM((65536 - size), size, cartridge_buffer, 0);
      break;
   case CARTRIDGE_TYPE_NORMAL_RAM:
      memory_WriteROM(65536 - size, size, cartridge_buffer, 0);
      memory_ClearROM(16384, 16384);
      break;
   case CARTRIDGE_TYPE_SUPERCART:
   {
      uint32_t offset = size - 16384;
      if (offset < size)
      {
         memory_WriteROM(49152, 16384, cartridge_buffer, offset);
         // Default to bank 0
         // banksets changes
         cartridge_StoreBank(0);
      }
   }
   break;
   case CARTRIDGE_TYPE_SUPERCART_LARGE:
   {
      uint32_t offset = size - 16384;
      if (offset < size)
      {
         memory_WriteROM(49152, 16384, cartridge_buffer, offset);
         memory_WriteROM(16384, 16384, cartridge_buffer, cartridge_GetBankOffset(0));
         // Default to bank 0
         // banksets changes
         cartridge_StoreBank(0);
      }
   }
   break;
   case CARTRIDGE_TYPE_SUPERCART_RAM:
   {
      uint32_t offset = size - 16384;
      if (offset < size)
      {
         memory_WriteROM(49152, 16384, cartridge_buffer, offset);
         memory_ClearROM(16384, 16384);
         // Default to bank 0
         // banksets changes
         cartridge_StoreBank(0);
      }
   }
   break;
   case CARTRIDGE_TYPE_SUPERCART_ROM:
   {
      uint32_t offset = size - 16384;
      if (offset < size && cartridge_GetBankOffset(6) < size)
      {
         memory_WriteROM(49152, 16384, cartridge_buffer, offset);
         memory_WriteROM(16384, 16384, cartridge_buffer, cartridge_GetBankOffset(6));
         // Default to bank 0
         // banksets changes
         cartridge_StoreBank(0);
      }
   }
   break;
   case CARTRIDGE_TYPE_ABSOLUTE:
      memory_WriteROM(16384, 16384, cartridge_buffer, 0);
      memory_WriteROM(32768, 32768, cartridge_buffer, cartridge_GetBankOffset(2));
      break;
   case CARTRIDGE_TYPE_ACTIVISION:
      if (122880 < size)
      {
         memory_WriteROM(40960, 16384, cartridge_buffer, 0);
         memory_WriteROM(16384, 8192, cartridge_buffer, 106496);
         memory_WriteROM(24576, 8192, cartridge_buffer, 98304);
         memory_WriteROM(32768, 8192, cartridge_buffer, 122880);
         memory_WriteROM(57344, 8192, cartridge_buffer, 114688);
      }
      break;
#ifdef SOUPER
   case CARTRIDGE_TYPE_SOUPER:
      memory_WriteROM(0xc000, 0x4000, cartridge_buffer, cartridge_GetBankOffset(31));
      memory_WriteROM(0x8000, 0x4000, cartridge_buffer, cartridge_GetBankOffset(0));
      memory_ClearROM(0x4000, 0x4000);
      break;
#endif
   }
}

void cartridge_Write(uint16_t address, uint8_t data)
{
   // banksets changes
   uint32_t size = cartridge_size;
   if (cartridge_banksets)
      size = size >> 1;

   switch (cartridge_type)
   {
   case CARTRIDGE_TYPE_SUPERCART:
   case CARTRIDGE_TYPE_SUPERCART_RAM:
   case CARTRIDGE_TYPE_SUPERCART_ROM:
   {
      uint32_t maxbank = size / 16384;
      if (address >= 32768 && address < 49152 &&
          cartridge_GetBank(data) < maxbank /*9*/)
      {
         cartridge_StoreBank(data);
      }
   }
   break;
   case CARTRIDGE_TYPE_SUPERCART_LARGE:
   {
      uint32_t maxbank = size / 16384;
      if (address >= 32768 && address < 49152 &&
          cartridge_GetBank(data) < maxbank /*9*/)
      {
         cartridge_StoreBank(data + 1);
      }
   }
   break;
   case CARTRIDGE_TYPE_ABSOLUTE:
      if (address == 32768 && (data == 1 || data == 2))
      {
         cartridge_StoreBank(data - 1);
      }
      break;
   case CARTRIDGE_TYPE_ACTIVISION:
      if (address >= 65408)
      {
         cartridge_StoreBank(address & 7);
      }
      break;
#ifdef SOUPER
   case CARTRIDGE_TYPE_SOUPER:
      if (address >= 0x4000 && address < 0x8000)
      {
         memory_souper_ram[memory_souper_GetRamAddress(address)] = data;
         break;
      }
      switch (address)
      {
      case CARTRIDGE_SOUPER_BANK_SEL:
         cartridge_StoreBank(data & 31);
         break;
      case CARTRIDGE_SOUPER_CHR_A_SEL:
         cartridge_souper_StoreChrBank(0, data);
         break;
      case CARTRIDGE_SOUPER_CHR_B_SEL:
         cartridge_souper_StoreChrBank(1, data);
         break;
      case CARTRIDGE_SOUPER_MODE_SEL:
         cartridge_souper_SetMode(data);
         break;
      case CARTRIDGE_SOUPER_EXRAM_V_SEL:
         cartridge_souper_SetRamPageBank(0, data);
         break;
      case CARTRIDGE_SOUPER_EXRAM_D_SEL:
         cartridge_souper_SetRamPageBank(1, data);
         break;
      case CARTRIDGE_SOUPER_AUDIO_CMD:
         bupchip_ProcessAudioCommand(data);
         break;
      }
      break;
#endif
   }
}

void cartridge_StoreBank(uint8_t bank)
{
   switch (cartridge_type)
   {
   case CARTRIDGE_TYPE_SUPERCART:
      cartridge_WriteBank(32768, bank);
      break;
   case CARTRIDGE_TYPE_SUPERCART_RAM:
      cartridge_WriteBank(32768, bank);
      break;
   case CARTRIDGE_TYPE_SUPERCART_ROM:
      cartridge_WriteBank(32768, bank);
      break;
   case CARTRIDGE_TYPE_SUPERCART_LARGE:
      cartridge_WriteBank(32768, bank);
      break;
   case CARTRIDGE_TYPE_ABSOLUTE:
      cartridge_WriteBank(16384, bank);
      break;
   case CARTRIDGE_TYPE_ACTIVISION:
      cartridge_WriteBank(40960, bank);
      break;
#ifdef SOUPER
   case CARTRIDGE_TYPE_SOUPER:
      cartridge_WriteBank(32768, bank);
      break;
#endif
   }
}

bool cartridge_IsLoaded(void)
{
   return (cartridge_buffer != NULL) ? true : false;
}

void cartridge_Release(bool persistent_data)
{
   cartridge_stored = false;
   if (!persistent_data)
   {
      if (cartridge_buffer)
         free(cartridge_buffer);
   }
   cartridge_buffer = NULL;
   cartridge_size = 0;

   cartridge_type = 0;
   cartridge_region = 0;
   cartridge_composite = 0;
   cartridge_pokey = 0;
   cartridge_pokey450 = 0;
   // banksets changes
   cartridge_pokey800 = 0;
   cartridge_pokey_range = false;
   cartridge_pokey_range_begin = 0;
   cartridge_pokey_range_end = 0;
   cartridge_xm = false;
   cartridge_hblank = HBLANK_DEFAULT;
   // Default to joysticks
   cartridge_controller[0] = 1;
   cartridge_controller[1] = 1;
   cartridge_bank = 0;
   cartridge_flags = 0;
   // banksets changes
   cartridge_banksets = false;
   cartridge_pokey_write_only = false;
   cartridge_halt_banked_ram = false;
}

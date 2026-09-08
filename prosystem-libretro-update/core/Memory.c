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
 * Memory.c
 * ----------------------------------------------------------------------------
 */
#include <stdlib.h>
#include <stdio.h>
#include "Memory.h"
#include "Equates.h"
#include "Bios.h"
#include "Cartridge.h"
#include "Tia.h"
#include "Riot.h"
#include "Pokey.h"
#include "Xm.h"

uint8_t memory_ram[MEMORY_SIZE] = {0};
uint8_t memory_rom[MEMORY_SIZE] = {0};
#ifdef SOUPER
uint8_t memory_souper_ram[MEMORY_SOUPER_EXRAM_SIZE] = {0};
#endif

// banksets changes
uint8_t maria_memory_ram[MEMORY_SIZE] = {0};

static bool lock = false;
static bool maria_read = false;

void memory_Reset(void)
{
  uint32_t index;
  for (index = 0; index < MEMORY_SIZE; index++)
  {
    memory_ram[index] = 0;
    // banksets changes
    maria_memory_ram[index] = 0;
    memory_rom[index] = 1;
  }
  for (index = 0; index < 16384; index++)
    memory_rom[index] = 0;

  // banksets changes
  lock = false;
}

#ifdef SOUPER
uint16_t memory_souper_GetRamAddress(uint16_t address)
{
  uint8_t page = (address - 0x4000) >> 12;
  if ((cartridge_souper_mode & CARTRIDGE_SOUPER_MODE_EXS) != 0)
  {
    if (address >= 0x6000 && address < 0x7000)
      page = cartridge_souper_ram_page_bank[0];
    else if (address >= 0x7000 && address < 0x8000)
      page = cartridge_souper_ram_page_bank[1];
  }
  return (address & 0x0fff) | ((uint16_t)page << 12);
}
#endif

uint8_t _memory_Read(uint16_t address)
{
  uint8_t tmp_byte;

  if (cartridge_xm &&
      ((address >= 0x0470 && address < 0x0480) ||
        (xm_pokey_enabled && (address >= 0x0450 && address < 0x0470)) ||
        (xm_mem_enabled && (address >= 0x4000 && address < 0x8000)) ||
        (xm_ym_enabled && (address >= 0x0460 && address <= 0x0461)))) {
    return xm_Read(address);
  }

  // banksets changes (pokey@800)
  if (!cartridge_pokey_write_only)
  {
    if (cartridge_pokey && ((!cartridge_pokey450 && !cartridge_pokey800 && (address >= 0x4000 && address <= 0x400f)) ||
                            (cartridge_pokey800 && (address >= 0x0800 && address < 0x0820)) ||
                            (cartridge_pokey450 && (address >= 0x0450 && address < 0x0470))))
    {

      return pokey_GetRegister(
        cartridge_pokey800 ? 0x4000 + (address - 0x0800) :
          cartridge_pokey450 ? 0x4000 + (address - 0x0450) :
            address);
    }
  }

  // Maria registers.
  // banksets changes
  if ((address >= 0x20 && address <= 0x3F) && (address != MSTAT))
  {
    return 0;
  }

  switch (address)
  {
  case INTIM:
  case INTIM | 0x2:
    memory_ram[INTFLG] &= 0x7f;
    return memory_ram[INTIM];
    break;
  case INTFLG:
  case INTFLG | 0x2:
    tmp_byte = memory_ram[INTFLG];
    memory_ram[INTFLG] &= 0x7f;
    return tmp_byte;
    break;
  default:

#ifdef SOUPER
    if (cartridge_type == CARTRIDGE_TYPE_SOUPER && address >= 0x4000 && address < 0x8000)
      return memory_souper_ram[memory_souper_GetRamAddress(address)];
#endif

    // banksets changes
    if (maria_read)
    {
      if (cartridge_halt_banked_ram && (address >= 16384 && address <= 32767))
      {
        return maria_memory_ram[address];
      }
      if (cartridge_type == CARTRIDGE_TYPE_NORMAL || cartridge_type == CARTRIDGE_TYPE_NORMAL_RAM)
      {
        if (address >= cartridge_banksets_begin && address <= cartridge_banksets_end)
        {
          return maria_memory_ram[address];
        }
      }
      else
      {
        if (cartridge_type == CARTRIDGE_TYPE_SUPERCART_ROM &&
            address >= 16384 && address <= 32767)
        {
          return maria_memory_ram[address];
        }
        if (address >= 32768 && address <= 49151)
        {
          return maria_memory_ram[address];
        }
        if (address >= 49152 && address <= 65535)
        {
          return maria_memory_ram[address];
        }
      }
    }
    return memory_ram[address];
    break;
  }
}

uint8_t memory_Read(uint16_t address)
{
  uint8_t data = _memory_Read(address);
  if (data < 0)
  {
    printf("Less than zero memory read: %d %d\n", address, data);
  }
  return data;
}

// banksets changes
uint8_t memory_ReadMaria(uint16_t address)
{
  maria_read = true;
  uint8_t data = _memory_Read(address);
  maria_read = false;
  if (data < 0)
  {
    printf("Less than zero memory read: %d %d\n", address, data);
  }
  return data;
}

void memory_Write(uint16_t address, uint8_t data)
{
  if (cartridge_xm &&
      ((address >= 0x0470 && address < 0x0480) ||
        (xm_pokey_enabled && (address >= 0x0450 && address < 0x0470)) ||
        (xm_mem_enabled && (address >= 0x4000 && address < 0x8000)) ||
        (xm_ym_enabled && (address >= 0x0460 && address <= 0x0461)))) {
    xm_Write(address, data);
    return;
  }

  // banksets changes (pokey@800)
  if (cartridge_pokey && ((
      !cartridge_pokey450 && !cartridge_pokey800 && !cartridge_pokey_range &&
        (address >= 0x4000 && address <= 0x400f)) ||
      (cartridge_pokey_range && (address >= cartridge_pokey_range_begin && address <= cartridge_pokey_range_end)) ||
      (cartridge_pokey800 && (address >= 0x0800 && address < 0x0820)) ||
      (cartridge_pokey450 && (address >= 0x0450 && address < 0x0470))))
  {
    pokey_SetRegister(
      cartridge_pokey_range ? 0x4000 + (address & 0xF) :
          cartridge_pokey800 ? 0x4000 + (address - 0x0800) :
            cartridge_pokey450 ? 0x4000 + (address - 0x0450) : address,
        data);
    return;
  }

  if (!memory_rom[address] || (cartridge_halt_banked_ram && (address >= 49152 && address <= 65535)))
  {

    // Track writes to high score SRAM
#if 0
    if (highScoreCartEnabled && ((address >= 0x1000) && (address <= 0x17FF))) {
      //hs_sram_write_count++;
      if (highScoreCallback) {
        highScoreCallback.write(address, data);
      }
    }
#endif

    // INPTCTRL
    // banksets changes
    // Diagnosed by RevEng
    // Multiple addresses are used to set INPTCTRL
    // Lock Mode needs to be set
    if (address >= 0 && address <= 0xf)
    {
      if (!lock)
      {
        if (data & 1)
        {
          lock = true;
          printf("Lock: %d\n", data);
          memory_ram[MSTAT] = 0x80; // Required for Bouncing Balls demo
        }
        if ((data & 4) && cartridge_IsLoaded())
        {
          if (!cartridge_stored)
          {
            cartridge_Store();
          }
        }
#if 0
        else if (!(data & 4) && Bios.IsEnabled()) {
          Bios.Store();
        }
#endif
      }
    }
    else
    {
      switch (address)
      {
      case WSYNC:
        // memory_ram[WSYNC] = true;
        memory_ram[WSYNC] = 1;
        break;
      case INPT0:
      case INPT1:
      case INPT2:
      case INPT3:
      case INPT4:
      case INPT5:
      case MSTAT: // MSTAT is read-only
        break;
      case AUDC0:
        tia_SetRegister(AUDC0, data);
        break;
      case AUDC1:
        tia_SetRegister(AUDC1, data);
        break;
      case AUDF0:
        tia_SetRegister(AUDF0, data);
        break;
      case AUDF1:
        tia_SetRegister(AUDF1, data);
        break;
      case AUDV0:
        tia_SetRegister(AUDV0, data);
        break;
      case AUDV1:
        tia_SetRegister(AUDV1, data);
        break;

      case SWCHB:
        /*gdement:  Writing here actually writes to DRB inside the RIOT chip.
      This value only indirectly affects output of SWCHB.*/
        riot_SetDRB(data);
        break;

      case SWCHA:
        riot_SetDRA(data);
        break;
      case TIM1T:
      case TIM1T | 0x8:
        riot_SetTimer(TIM1T, data);
        break;
      case TIM8T:
      case TIM8T | 0x8:
        riot_SetTimer(TIM8T, data);
        break;
      case TIM64T:
      case TIM64T | 0x8:
        riot_SetTimer(TIM64T, data);
        break;
      case T1024T:
      case T1024T | 0x8:
        riot_SetTimer(T1024T, data);
        break;
      default:
#ifdef SOUPER
        if (cartridge_type == CARTRIDGE_TYPE_SOUPER && address >= 0x4000 && address < 0x8000)
        {
          memory_souper_ram[memory_souper_GetRamAddress(address)] = data;
          break;
        }
#endif
        // banksets changes
        if (cartridge_halt_banked_ram && (address >= 49152 && address <= 65535))
        {
          // console.log(16384 + (address - 49152) + ", " + data);
          maria_memory_ram[16384 + (address - 49152)] = data;
        }
        else
        {
          memory_ram[address] = data;
          if (address >= 8256 && address <= 8447)
          {
            memory_ram[address - 8192] = data;
          }
          else if (address >= 8512 && address <= 8703)
          { // banksets changes (8703)
            memory_ram[address - 8192] = data;
          }
          else if (address >= 64 && address <= 255)
          {
            memory_ram[address + 8192] = data;
          }
          else if (address >= 320 && address <= 511)
          {
            memory_ram[address + 8192] = data;
          }
          // banksets changes
          else if (address >= 10240 && address <= 12287)
          {
            memory_ram[address - 2048] = data;
          }
          // banksets changes
          else if (address >= 8192 && address <= 10239)
          {
            memory_ram[address + 2048] = data;
          }
        }
        break;
      }
    }
  }
  else
  {
    cartridge_Write(address, data);
  }
}

void memory_WriteROM(uint16_t address, uint16_t size, const uint8_t *data, uint32_t offset)
{
  bool write_to_maria = false;
  uint32_t maria_offset = 0;

  // banksets changes
  if (cartridge_banksets)
  {
    uint8_t type = cartridge_type;
    if (type == CARTRIDGE_TYPE_NORMAL || type == CARTRIDGE_TYPE_NORMAL_RAM)
    {
      maria_offset = size;
      cartridge_banksets_begin = address;
      cartridge_banksets_end = address + size - 1;
      write_to_maria = true;
    }
    else if (address == 32768 || address == 49152 ||
             (type == CARTRIDGE_TYPE_SUPERCART_ROM && address == 16384))
    {
      maria_offset = 128 * 1024;
      write_to_maria = true;
    }
  }

  if ((address + size) <= MEMORY_SIZE && data != NULL)
  {
    for (uint32_t index = 0; index < size; index++)
    {
      memory_ram[address + index] = data[index + offset];
      memory_rom[address + index] = 1;
      if (write_to_maria)
      {
        // banksets changes
        maria_memory_ram[address + index] = data[index + offset + maria_offset];
      }
    }
  }
}

void memory_ClearROM(uint16_t address, uint16_t size)
{
  if ((address + size) <= MEMORY_SIZE)
  {
    for (uint32_t index = 0; index < size; index++)
    {
      memory_ram[address + index] = 0;
      memory_rom[address + index] = 0;
      // banksets changes
      if (cartridge_halt_banked_ram && address == 16384)
      {
        maria_memory_ram[address + index] = 0;
      }
    }
  }
}

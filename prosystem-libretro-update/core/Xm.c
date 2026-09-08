/*
 A7800  XM expansion module emulation
    pokey   $450
    ym2151  $460 (R)  FM status register
        $460 (W)  FM address register
        $461 (RW) FM data register
    cntrl1	$470
        d0 rof lo on
        d1 rof hi on
        d2 0=bios,1=top slot
        d3 1=hsc on
        d4 1=pokey on
        d5 1=bank0 on 4000-5fff
        d6 1=bank1 on 6000-7fff
        d7 1=ym2151 on
    cntrl2	$478  - SALLY RAM bank 8K page multiplexer.
        d0-d3 sally ram page 0 a0-a3
        d4-d7 sally ram page 1 a0-a3
    cntrl3	$47c  - MARIA RAM bank 8K page multiplexer.
        d0-d3 maria ram page 0 a0-a3
        d4-d7 maria ram page 1 a0-a3
    cntrl4	$471
        d0 1=pia on
        d1-d3 flash bank lo a1-a3
        d4-d6 flash bank hi a1-a3
        d7 1=top slot lock
    cntrl5 	$472
        d0 1=48k ram enable
        d1 1=ram we# disabled
        d2 1=bios enabled (in test mode)
        d3 1=POKEY enable/disable locked
        d4 1=HSC enable/disable locked - cannot disable after enable
        d5 1=PAL HSC enabled, 0=NTSC HSC enabled - cannot disable after enable

    RAM0	$4000    $5FFF     8192 bytes
    RAM1	$6000    $7FFF     8192 bytes
*/

#include "ProSystem.h"
#include "Pokey.h"
#include "ym.h"
#include "Xm.h"
// import * as YM from "../3rdparty/ym2151.js"

uint8_t xm_ram[XM_RAM_SIZE] = {0};

bool xm_mem_enabled = false;
bool xm_pokey_enabled = false;
bool xm_ym_enabled = false;
bool xm_48kram_enabled = false;
bool xm_bank0_enabled = false;
bool xm_bank1_enabled = false;
bool xm_ramwe_disabled = false;
bool dma_active = false;

uint8_t cntrl1 = 0;
uint8_t cntrl2 = 0;
uint8_t cntrl3 = 0;
uint8_t cntrl4 = 0;
uint8_t cntrl5 = 0;
uint32_t ym_addr = 0;

// TODO: Do proper timing for YM (Hack for YM auto-detection)
// banksets changes
static uint32_t auto_detect_start = 0;
static uint32_t auto_detect_return = 2;
static uint32_t auto_detect_count = -1;

void xm_Reset()
{
   for (int i = 0; i < XM_RAM_SIZE; i++)
   {
      xm_ram[i] = 0;
   }
   xm_mem_enabled = false;
   xm_pokey_enabled = false;
   xm_ym_enabled = false;
   xm_48kram_enabled = false;
   xm_bank0_enabled = false;
   xm_bank1_enabled = false;
   xm_ramwe_disabled = false;
   dma_active = false;
   cntrl1 = 0;
   cntrl2 = 0;
   cntrl3 = 0;
   cntrl4 = 0;
   cntrl5 = 0;
   ym_addr = 0;
   // YM.reset();

   // banksets changes
   auto_detect_start = 0;
   auto_detect_return = 2;
   auto_detect_count = -1;
}

// byte xm_Read(word address) {
uint8_t xm_Read(uint32_t address)
{
   uint8_t b;
   if (xm_pokey_enabled && (address >= 0x0450 && address < 0x0460))
   {
      b = pokey_GetRegister(0x4000 + (address - 0x0450));
      // console.log("Read from Pokey1: %d %d", address, b);
      return b;
   }
   else if (xm_ym_enabled && (address >= 0x0460 && address <= 0x0461))
   {
      b = address & 1 ? ym_GetStatus() : 0; // 0xFF? Breaks some demos... hmm...

      // TODO: Do proper timing for YM (Hack for YM auto-detection)
      // banksets changes
      if (auto_detect_count != -1 && address & 1)
      {
         if (auto_detect_count > 0)
         {
            auto_detect_count--;
         }
         else if (auto_detect_count == 0)
         {
            if (auto_detect_return > 0)
            {
               b = 2;
               // console.log("### RETURNING!")
               auto_detect_count = auto_detect_start;
               auto_detect_return--;
               if (auto_detect_return == 0)
               {
                  auto_detect_count = -1;
               }
            }
         }
      }

      // console.log("Read from YM: %s %d", "" + address.toString(16), b);
      return b;
   }
   else if (xm_mem_enabled && (address >= 0x4000 && address < 0x8000))
   {
      // console.log("read: " + address);
      if (!xm_ramwe_disabled)
      {
         uint32_t offset = address - 0x4000;
         if (xm_48kram_enabled)
         {
            return xm_ram[offset]; // raz TODO fix
         }
         else
         {
            if (xm_bank0_enabled && (offset < 0x2000))
            {
               if (dma_active)
               {
                  return xm_ram[((offset & 0x1fff) + (((cntrl3 & 15) * 0x2000))) | ((cntrl1 & 1) << 8)];
               }
               else
               {
                  return xm_ram[((offset & 0x1fff) + (((cntrl2 & 15) * 0x2000))) | ((cntrl1 & 1) << 8)];
               }
            }
            else if (xm_bank1_enabled && ((offset >= 0x2000) && (offset < 0x4000)))
            {
               if (dma_active)
               {
                  return xm_ram[((offset & 0x1fff) + ((((cntrl3 >> 4) & 15) * 0x2000))) | ((cntrl1 & 2) << 7)];
               }
               else
               {
                  return xm_ram[((offset & 0x1fff) + ((((cntrl2 >> 4) & 15) * 0x2000))) | ((cntrl1 & 2) << 7)];
               }
            }
         }
      }
   }
   return 0;
}

void xm_Write(uint32_t address, uint8_t data)
{
   if (xm_pokey_enabled && (address >= 0x0450 && address < 0x0460))
   {
      // console.log("Wrote to Pokey1: %d %d", address, data);
      pokey_SetRegister(0x4000 + (address - 0x0450), data);
   }
   else if (xm_ym_enabled && (address >= 0x0460 && address <= 0x0461))
   {
      // console.log("Wrote to YM: %s %d", "" + address.toString(16), data);
      if (address & 1)
      {
         ym_SetReg(ym_addr, data);

         // TODO: Do proper timing for YM (Hack for YM auto-detection)
         // banksets changes
         if (ym_addr == 18 && data == 252)
         {
            // console.log("### STARTING!")
            auto_detect_start = 129;
            auto_detect_count = auto_detect_start;
         }
      }
      else
      {
         ym_addr = data;
      }
   }
   else if (xm_mem_enabled && (address >= 0x4000 && address < 0x8000))
   {
      uint32_t offset = address - 0x4000;
      if (xm_48kram_enabled)
      {
         xm_ram[offset] = data; // raz TODO fix
      }
      else
      {
         if (xm_bank0_enabled && (offset < 0x2000))
         {
            // console.log("write byte2:" + offset + ", " + data);
            xm_ram[(offset & 0x1fff) + (((cntrl2 & 15) * 0x2000))] = data;
         }
         else if (xm_bank1_enabled && ((offset >= 0x2000) && (offset < 0x4000)))
         {
            // console.log("write byte3:" + data);
            xm_ram[(offset & 0x1fff) + ((((cntrl2 >> 4) & 15) * 0x2000))] = data;
         }
      }
   }
   else if (address >= 0x0470 && address < 0x0480)
   {
      if (address == 0x0470)
      {
         // console.log("Wrote CNTRL1: %d", data);
         cntrl1 = data;
         xm_pokey_enabled = cntrl1 & 0x10;
         xm_bank0_enabled = cntrl1 & 0x20;
         xm_bank1_enabled = cntrl1 & 0x40;
         xm_ym_enabled = cntrl1 & 0x80;
      }
      else if (address == 0x0478)
      {
         // console.log("Wrote CNTRL2 %d",  data);
         cntrl2 = data;
      }
      else if (address == 0x047c)
      {
         // console.log("Wrote CNTRL3: %d",  data);
         cntrl3 = data;
      }
      else if (address == 0x0471)
      {
         // console.log("Wrote CNTRL4: %d",  data);
         cntrl4 = data;
      }
      else if (address == 0x0472)
      {
         // console.log("Wrote CNTRL5: %d",  data);
         cntrl5 = data;
         xm_48kram_enabled = cntrl5 & 1;
         xm_ramwe_disabled = cntrl5 & 2;
      }

      xm_mem_enabled = xm_48kram_enabled || xm_bank0_enabled || xm_bank1_enabled;

      /*
      console.log("xm_pokey_enabled: %d, xm_ym_enabled: %d, " +
        "xm_mem_enabled: %d, 48k ram: %d, bank0: %d, bank1: %d, ramwedisabled: %d",
        xm_pokey_enabled ? 1 : 0,
        xm_ym_enabled ? 1 : 0,
        xm_mem_enabled ? 1 : 0,
        xm_48kram_enabled ? 1 : 0,
        xm_bank0_enabled ? 1 : 0,
        xm_bank1_enabled ? 1 : 0,
        xm_ramwe_disabled ? 1 : 0
      );
      */
   }
}

/* Mednafen - Multi-system Emulator
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <string.h>
#include <assert.h>

#include "../mednafen-types.h"
#include "../state_helpers.h"

#include "vsu.h"

static void VSU_CalcCurrentOutput(int ch, int *left, int *right);

static void VSU_Update(int32 timestamp);

uint8 IntlControl[6];
uint8 LeftLevel[6];
uint8 RightLevel[6];
uint16 Frequency[6];
uint16 EnvControl[6];	/* Channel 5/6 extra functionality tacked on too. */

uint8 RAMAddress[6];

uint8 SweepControl;

uint8 WaveData[5][0x20];

uint8 ModData[0x20];

int32 EffFreq[6];
int32 Envelope[6];

int32 WavePos[6];
int32 ModWavePos;

int32 LatcherClockDivider[6];

int32 FreqCounter[6];
int32 IntervalCounter[6];
int32 EnvelopeCounter[6];
int32 SweepModCounter;

int32 EffectsClockDivider[6];
int32 IntervalClockDivider[6];
int32 EnvelopeClockDivider[6];
int32 SweepModClockDivider;

int32 NoiseLatcherClockDivider;
uint32 NoiseLatcher;

uint32 lfsr;

int32 last_output[6][2];
int32 last_ts;

Blip_Buffer *bb_l;
Blip_Buffer *bb_r;
Blip_Synth Synth;
Blip_Synth NoiseSynth;

static const unsigned int Tap_LUT[8] = { 15 - 1, 11 - 1, 14 - 1, 5 - 1, 9 - 1, 7 - 1, 10 - 1, 12 - 1 };

void VSU_Init(Blip_Buffer *_bb_l, Blip_Buffer *_bb_r)
{
   unsigned ch, lr;

   bb_l    = _bb_l;
   bb_r    = _bb_r;

   Blip_Synth_set_volume(&Synth, 1.0 / 6 / 2, 0x400);

   for(ch = 0; ch < 6; ch++)
      for(lr = 0; lr < 2; lr++)
         last_output[ch][lr] = 0;
}

void VSU_Power(void)
{
   unsigned ch;

   SweepControl = 0;
   SweepModCounter = 0;
   SweepModClockDivider = 1;

   for(ch = 0; ch < 6; ch++)
   {
      IntlControl[ch] = 0;
      LeftLevel[ch] = 0;
      RightLevel[ch] = 0;
      Frequency[ch] = 0;
      EnvControl[ch] = 0;
      RAMAddress[ch] = 0;

      EffFreq[ch] = 0;
      Envelope[ch] = 0;
      WavePos[ch] = 0;
      FreqCounter[ch] = 0;
      IntervalCounter[ch] = 0;
      EnvelopeCounter[ch] = 0;

      EffectsClockDivider[ch] = 4800;
      IntervalClockDivider[ch] = 4;
      EnvelopeClockDivider[ch] = 4;

      LatcherClockDivider[ch] = 120;
   }

   ModWavePos = 0;

   NoiseLatcherClockDivider = 120;
   NoiseLatcher = 0;

   lfsr = 0;

   memset(WaveData, 0, sizeof(WaveData));
   memset(ModData, 0, sizeof(ModData));

   last_ts = 0;
}

void VSU_Write(int32 timestamp, uint32 A, uint8 V)
{
   if(MDFN_UNLIKELY(A & 0x3))
      return;

   A &= 0x7FF;

   VSU_Update(timestamp);

   if(A < 0x280)
      WaveData[A >> 7][(A >> 2) & 0x1F] = V & 0x3F;
   else if(A < 0x400) /* Modulation mirror write? */
      ModData[(A >> 2) & 0x1F] = V;
   else if(A < 0x600)
   {
      int ch = (A >> 6) & 0xF;

      if(ch > 5)
      {
         if(A == 0x580 && (V & 1))
         {
            int i;
            for(i = 0; i < 6; i++)
               IntlControl[i] &= ~0x80;
         }
      }
      else
         switch((A >> 2) & 0xF)
         {
            case 0x0:
               IntlControl[ch] = V & ~0x40;

               if(V & 0x80)
               {
                  EffFreq[ch] = Frequency[ch];
                  if(ch == 5)
                     FreqCounter[ch] = 10 * (2048 - EffFreq[ch]);
                  else
                     FreqCounter[ch] = 2048 - EffFreq[ch];
                  IntervalCounter[ch] = (V & 0x1F) + 1;
                  EnvelopeCounter[ch] = (EnvControl[ch] & 0x7) + 1;

                  if(ch == 4)
                  {
                     SweepModCounter = (SweepControl >> 4) & 7;
                     SweepModClockDivider = (SweepControl & 0x80) ? 8 : 1;
                     ModWavePos = 0;
                  }

                  WavePos[ch] = 0;

                  if(ch == 5)	/* Not sure if this is correct. */
                     lfsr = 1;

#if 0
                  if(!(IntlControl[ch] & 0x80))
                     Envelope[ch] = (EnvControl[ch] >> 4) & 0xF;
#endif

                  EffectsClockDivider[ch] = 4800;
                  IntervalClockDivider[ch] = 4;
                  EnvelopeClockDivider[ch] = 4;
               }
               break;

            case 0x1:
               LeftLevel[ch] = (V >> 4) & 0xF;
               RightLevel[ch] = (V >> 0) & 0xF;
               break;

            case 0x2:
               Frequency[ch] &= 0xFF00;
               Frequency[ch] |= V << 0;
               EffFreq[ch] &= 0xFF00;
               EffFreq[ch] |= V << 0;
               break;

            case 0x3:
               Frequency[ch] &= 0x00FF;
               Frequency[ch] |= (V & 0x7) << 8;
               EffFreq[ch] &= 0x00FF;
               EffFreq[ch] |= (V & 0x7) << 8;
               break;

            case 0x4:
               EnvControl[ch] &= 0xFF00;
               EnvControl[ch] |= V << 0;

               Envelope[ch] = (V >> 4) & 0xF;
               break;

            case 0x5:
               EnvControl[ch] &= 0x00FF;
               if(ch == 4)
                  EnvControl[ch] |= (V & 0x73) << 8;
               else if(ch == 5)
               {
                  EnvControl[ch] |= (V & 0x73) << 8;
                  lfsr = 1;
               }
               else
                  EnvControl[ch] |= (V & 0x03) << 8;
               break;

            case 0x6:
               RAMAddress[ch] = V & 0xF;
               break;

            case 0x7:
               if(ch == 4)
                  SweepControl = V;
               break;
         }
   }
}

static INLINE void VSU_CalcCurrentOutput(int ch, int *left, int *right)
{
   int WD;
   int l_ol, r_ol;

   if(!(IntlControl[ch] & 0x80))
   {
      *left = *right = 0;
      return;
   }

   if(ch == 5)
      WD = NoiseLatcher;	/*(NoiseLatcher << 6) - NoiseLatcher; */
   else
   {
      if(RAMAddress[ch] > 4)
         WD = 0;
      else
         WD = WaveData[RAMAddress[ch]][WavePos[ch]];	/* - 0x20; */
   }
   l_ol = Envelope[ch] * LeftLevel[ch];
   if(l_ol)
   {
      l_ol >>= 3;
      l_ol += 1;
   }

   r_ol = Envelope[ch] * RightLevel[ch];
   if(r_ol)
   {
      r_ol >>= 3;
      r_ol += 1;
   }

   *left = WD * l_ol;
   *right = WD * r_ol;
}

void VSU_Update(int32 timestamp)
{
   int left, right;
   unsigned ch;

   for(ch = 0; ch < 6; ch++)
   {
      int32 clocks = timestamp - last_ts;
      int32 running_timestamp = last_ts;

      /* Output sound here */
      VSU_CalcCurrentOutput(ch, &left, &right);
      Blip_Synth_offset(&Synth, running_timestamp, left - last_output[ch][0], bb_l);
      Blip_Synth_offset(&Synth, running_timestamp, right - last_output[ch][1], bb_r);
      last_output[ch][0] = left;
      last_output[ch][1] = right;

      if(!(IntlControl[ch] & 0x80))
         continue;

      while(clocks > 0)
      {
         int32 chunk_clocks = clocks;

         if(chunk_clocks > EffectsClockDivider[ch])
            chunk_clocks = EffectsClockDivider[ch];

         if(ch == 5)
         {
            if(chunk_clocks > NoiseLatcherClockDivider)
               chunk_clocks = NoiseLatcherClockDivider;
         }
         else
         {
            if(EffFreq[ch] >= 2040)
            {
               if(chunk_clocks > LatcherClockDivider[ch])
                  chunk_clocks = LatcherClockDivider[ch];
            }
            else
            {
               if(chunk_clocks > FreqCounter[ch])
                  chunk_clocks = FreqCounter[ch];
            }
         }

         FreqCounter[ch] -= chunk_clocks;
         while(FreqCounter[ch] <= 0)
         {
            if(ch == 5)
            {
               int feedback = ((lfsr >> 7) & 1) ^ ((lfsr >> Tap_LUT[(EnvControl[5] >> 12) & 0x7]) & 1) ^ 1;
               lfsr = ((lfsr << 1) & 0x7FFF) | feedback;

               FreqCounter[ch] += 10 * (2048 - EffFreq[ch]);
            }
            else
            {
               FreqCounter[ch] += 2048 - EffFreq[ch];
               WavePos[ch] = (WavePos[ch] + 1) & 0x1F;
            }
         }

         LatcherClockDivider[ch] -= chunk_clocks;
         while(LatcherClockDivider[ch] <= 0)
            LatcherClockDivider[ch] += 120;

         if(ch == 5)
         {
            NoiseLatcherClockDivider -= chunk_clocks;
            if(!NoiseLatcherClockDivider)
            {
               NoiseLatcherClockDivider = 120;
               NoiseLatcher = ((lfsr & 1) << 6) - (lfsr & 1);
            }
         }

         EffectsClockDivider[ch] -= chunk_clocks;
         while(EffectsClockDivider[ch] <= 0)
         {
            EffectsClockDivider[ch] += 4800;

            IntervalClockDivider[ch]--;
            while(IntervalClockDivider[ch] <= 0)
            {
               IntervalClockDivider[ch] += 4;

               if(IntlControl[ch] & 0x20)
               {
                  IntervalCounter[ch]--;
                  if(!IntervalCounter[ch])
                  {
                     IntlControl[ch] &= ~0x80;
                  }
               }

               EnvelopeClockDivider[ch]--;
               while(EnvelopeClockDivider[ch] <= 0)
               {
                  EnvelopeClockDivider[ch] += 4;

                  if(EnvControl[ch] & 0x0100)	/* Enveloping enabled? */
                  {
                     EnvelopeCounter[ch]--;
                     if(!EnvelopeCounter[ch])
                     {
                        EnvelopeCounter[ch] = (EnvControl[ch] & 0x7) + 1;

                        if(EnvControl[ch] & 0x0008)	/* Grow */
                        {
                           if(Envelope[ch] < 0xF || (EnvControl[ch] & 0x200))
                              Envelope[ch] = (Envelope[ch] + 1) & 0xF;
                        }
                        else				/* Decay */
                        {
                           if(Envelope[ch] > 0 || (EnvControl[ch] & 0x200))
                              Envelope[ch] = (Envelope[ch] - 1) & 0xF;
                        }
                     }
                  }

               } /* end while(EnvelopeClockDivider[ch] <= 0) */
            } /* end while(IntervalClockDivider[ch] <= 0) */

            if(ch == 4)
            {
               SweepModClockDivider--;
               while(SweepModClockDivider <= 0)
               {
                  SweepModClockDivider += (SweepControl & 0x80) ? 8 : 1;

                  if(((SweepControl >> 4) & 0x7) && (EnvControl[ch] & 0x4000))
                  {
                     if(SweepModCounter)
                        SweepModCounter--;

                     if(!SweepModCounter)
                     {
                        SweepModCounter = (SweepControl >> 4) & 0x7;

                        if(EnvControl[ch] & 0x1000)	/* Modulation */
                        {
                           if(ModWavePos < 32 || (EnvControl[ch] & 0x2000))
                           {
                              ModWavePos &= 0x1F;

                              EffFreq[ch] = (EffFreq[ch] + (int8)ModData[ModWavePos]);
                              if(EffFreq[ch] < 0) /* underflow */
                                 EffFreq[ch] = 0;
                              else if(EffFreq[ch] > 0x7FF) /* overflow */
                                 EffFreq[ch] = 0x7FF;
                              ModWavePos++;
                           }
                        }
                        else				/* Sweep */
                        {
                           int32 delta = EffFreq[ch] >> (SweepControl & 0x7);
                           int32 NewFreq = EffFreq[ch] + ((SweepControl & 0x8) ? delta : -delta);

                           if(NewFreq < 0) /* underflow */
                              EffFreq[ch] = 0;
                           else if(NewFreq > 0x7FF) /* overflow */
                              IntlControl[ch] &= ~0x80;
                           else
                              EffFreq[ch] = NewFreq;
                        }
                     }
                  }
               } /* end while(SweepModClockDivider <= 0) */
            } /* end if(ch == 4) */
         } /* end while(EffectsClockDivider[ch] <= 0) */
         clocks -= chunk_clocks;
         running_timestamp += chunk_clocks;

         /* Output sound here too. */
         VSU_CalcCurrentOutput(ch, &left, &right);
         Blip_Synth_offset(&Synth, running_timestamp, left - last_output[ch][0], bb_l);
         Blip_Synth_offset(&Synth, running_timestamp, right - last_output[ch][1], bb_r);
         last_output[ch][0] = left;
         last_output[ch][1] = right;
      }
   }

   last_ts = timestamp;
}

void VSU_EndFrame(int32 timestamp)
{
   VSU_Update(timestamp);
   last_ts = 0;
}

int VSU_StateAction(StateMem *sm, int load, int data_only)
{
   SFORMAT StateRegs[] =
   {
      SFARRAY(IntlControl, 6),
      SFARRAY(LeftLevel, 6),
      SFARRAY(RightLevel, 6),

      SFARRAY16(Frequency, 6),
      SFARRAY16(EnvControl, 6),
      SFARRAY(RAMAddress, 6),
      SFVARN(SweepControl, "SweepControl"),

      SFARRAY(&WaveData[0][0], 5 * 0x20),
      SFARRAY(ModData, 0x20),

      SFARRAY32(EffFreq, 6),
      SFARRAY32(Envelope, 6),

      SFARRAY32(WavePos, 6),

      SFVARN(ModWavePos, "ModWavePos"),

      SFARRAY32(LatcherClockDivider, 6),
      SFARRAY32(FreqCounter, 6),
      SFARRAY32(IntervalCounter, 6),
      SFARRAY32(EnvelopeCounter, 6),

      SFVARN(SweepModCounter, "SweepModCounter"),

      SFARRAY32(EffectsClockDivider, 6),
      SFARRAY32(IntervalClockDivider, 6),
      SFARRAY32(EnvelopeClockDivider, 6),

      SFVARN(SweepModClockDivider, "SweepModClockDivider"),

      SFVARN(NoiseLatcherClockDivider, "NoiseLatcherClockDivider"),
      SFVARN(NoiseLatcher, "NoiseLatcher"),
      SFVARN(lfsr, "lfsr"),
      SFEND
   };

   return MDFNSS_StateAction(sm, load, data_only, StateRegs, "VSU", false);
}

uint8 VSU_PeekWave(const unsigned int which, uint32 Address)
{
   Address &= 0x1F;

   return(WaveData[which][Address]);
}

void VSU_PokeWave(const unsigned int which, uint32 Address, uint8 value)
{
   Address &= 0x1F;

   WaveData[which][Address] = value & 0x3F;
}

uint8 VSU_PeekModWave(uint32 Address)
{
   Address &= 0x1F;
   return(ModData[Address]);
}

void VSU_PokeModWave(uint32 Address, uint8 value)
{
   Address &= 0x1F;

   ModData[Address] = value & 0xFF;
}

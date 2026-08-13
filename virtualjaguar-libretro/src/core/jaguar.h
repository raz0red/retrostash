#ifndef __JAGUAR_H__
#define __JAGUAR_H__

#include <stdint.h>

#include <boolean.h>

#include "vjag_memory.h"							// For "UNKNOWN" enum

#ifdef __cplusplus
extern "C" {
#endif

void JaguarSetScreenBuffer(uint32_t * buffer);
void JaguarSetScreenPitch(uint32_t pitch);
void JaguarInit(void);
void JaguarReset(void);
void JaguarApplyHLEBIOSState(void);
void JaguarDone(void);
void JaguarSeedPRNG(uint32_t seed);
uint32_t JaguarRand(void);

uint8_t JaguarReadByte(uint32_t offset, uint32_t who);
uint16_t JaguarReadWord(uint32_t offset, uint32_t who);
uint32_t JaguarReadLong(uint32_t offset, uint32_t who);
void JaguarWriteByte(uint32_t offset, uint8_t data, uint32_t who);
void JaguarWriteWord(uint32_t offset, uint16_t data, uint32_t who);
void JaguarWriteLong(uint32_t offset, uint32_t data, uint32_t who);

bool JaguarInterruptHandlerIsValid(uint32_t i);

void JaguarExecuteNew(void);

// Exports from JAGUAR.CPP

extern int32_t jaguarCPUInExec;
extern char * jaguarEepromsPath;
extern bool jaguarCartInserted;
extern bool jaguarMemTrackInserted;
extern bool bpmActive;
extern uint32_t bpmAddress1;

#ifdef __cplusplus
extern "C" {
#endif
extern uint32_t jaguarMainROMCRC32, jaguarROMSize, jaguarRunAddress;
extern uint32_t jaguarLoadedRAMStart, jaguarLoadedRAMEnd;
#ifdef __cplusplus
}
#endif

// Various clock rates

#define M68K_CLOCK_RATE_PAL		13296950
#define M68K_CLOCK_RATE_NTSC	13295453
#define RISC_CLOCK_RATE_PAL		26593900
#define RISC_CLOCK_RATE_NTSC	26590906

#define SYSTEM_CLOCK_RATE		(vjs.hardwareTypeNTSC ? RISC_CLOCK_RATE_NTSC : RISC_CLOCK_RATE_PAL)
#define M68K_CLOCK_RATE			(vjs.hardwareTypeNTSC ? M68K_CLOCK_RATE_NTSC : M68K_CLOCK_RATE_PAL)

/* Clock-scale enhancement levers (issue #314), in percent of the stock
 * rate (100 = stock).  Config, not state: set from the core options in
 * libretro.c::check_variables(), never serialized.  Applied ONLY where
 * execution budgets are handed out (JaguarExecuteNew() timeslices and
 * the 68K->GPU 2:1 coupling in GPUSyncToM68K()) -- bus costs
 * (bus_arbiter_m68k_access() DRAM latencies, OP-fetch/refresh occupancy,
 * blitter occupancy) and all event scheduling (video, PIT, UART, I2S)
 * stay on the real, unscaled sysclock.  At 100 the integer arithmetic
 * (c * 100 / 100) is an exact identity, so 1x is bit-identical to the
 * unscaled build. */
extern uint32_t m68kClockScalePct;
void M68KClockScaleReset(void);
extern uint32_t riscClockScalePct;

#define SCALE_M68K_CYCLES(c)	((uint32_t)(((uint64_t)(c) * m68kClockScalePct) / 100u))
#define SCALE_RISC_CYCLES(c)	((uint32_t)(((uint64_t)(c) * riscClockScalePct) / 100u))

/* WRC: whether the M68K/RISC clock-scale editor properties (pulled from
 * JS via wrc_get_m68k_clock_scale_pct()/wrc_get_risc_clock_scale_pct()
 * in libretro.c) are wired up at all. Tried as a lever for the "games
 * run too fast" investigation; empirically had zero effect in both
 * directions on both processors, so disabled -- check_variables() just
 * hardcodes both to 100 (stock) under WRC when this is 0, regardless of
 * what the (now UI-hidden) props say. Set to 1 to re-enable pulling
 * from JS if this gets revisited later. */
#ifndef VJ_WRC_CLOCK_SCALE_ENABLED
#define VJ_WRC_CLOCK_SCALE_ENABLED 0
#endif

/* GPU RISC under-charged instruction timing. gpu_opcode_cycles[]
 * historically charged every GPU opcode a flat 1 cycle -- no evidence it
 * was ever deliberately tuned (unlike dsp_opcode_cycles[], which carries
 * a comment about being hand-adjusted against real audio behavior, so
 * that table is left alone here; this is GPU-only). Two specific,
 * documented gaps, both undercharging in a way that lets DIV/3D-math-
 * heavy GPU code (exactly what Doom's software renderer leans on) run
 * proportionally faster than real hardware, even though frame timing
 * itself (VC/halfline-driven) stays correctly paced -- which reads as
 * "the game runs too fast" without FPS itself ever being wrong:
 *
 *   - DIV: real Jaguar RISC hardware's divider is bit-serial -- 2
 *     bits/cycle for a 32-bit operand -- so DIV genuinely costs 16
 *     cycles, not 1. See https://www.mulle-kybernetik.com/jagdox/risc_doc.html.
 *   - MMULT: documented as "one (16-bit) multiply per tick" -- gpu_
 *     opcode_mmult() already loops `count` times (gpu_matrix_control &
 *     0x0F, the configured matrix width), doing one multiply-accumulate
 *     per iteration, so the real cost is `count` cycles, not 1.
 *
 * One flag disables both and reverts to the historical flat-1 behavior:
 * set to 0 to fully back out. */
#ifndef VJ_ACCURATE_RISC_CYCLES
#define VJ_ACCURATE_RISC_CYCLES 0
#endif

// Stuff for IRQ handling

#define ASSERT_LINE		1
#define CLEAR_LINE		0

//Temp debug stuff (will go away soon, so don't depend on these)
uint8_t * GetRamPtr(void);

#ifdef __cplusplus
}
#endif

#endif	// __JAGUAR_H__

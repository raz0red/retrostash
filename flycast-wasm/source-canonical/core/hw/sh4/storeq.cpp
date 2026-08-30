/*
	This file is part of Flycast.

    Flycast is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Flycast is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Flycast.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "sh4_if.h"
#include "sh4_mem.h"
#include "modules/mmu.h"
#include "hw/pvr/pvr_mem.h"

static u32 CCN_QACR_TR[2];

#if defined(__EMSCRIPTEN__) && !defined(JIT_PROD_BUILD)
#include <emscripten.h>
// SQ lifecycle counters (JGR zero-hole hunt, 2026-06-12): which stage loses
// the ~2% of 32-byte bursts in production? [0]=flush calls total,
// [1]=flushes to RAM area3, [2]=flushes landing in the JGR staging region
// (0x6dae00..0x740000 RAM offset), [3]=flushes routed to TA, [4]=handler
// is generic sqWrite (QACR-translated), [5]=current QACR_TR[0] sample.
extern "C" {
u32 g_fly_sq[8];
u32 EMSCRIPTEN_KEEPALIVE fly_sq_ptr() { return (u32)(uintptr_t)g_fly_sq; }
u32 EMSCRIPTEN_KEEPALIVE fly_sq_len() { return 8; }
}
// Watch the live hole addresses (shared dynamic targets, see rec_wasm.cpp)
extern "C" u32 g_fly_watch_off[2];
static inline void fly_sq_watch(u32 ramOff, const void* data) {
	const u32* WOFF = g_fly_watch_off;
	for (int i = 0; i < 2; i++) {
		if (ramOff <= WOFF[i] && WOFF[i] < ramOff + 32) {
			extern u32 FrameCount;
			EM_ASM({
				if (!window._flyLog) window._flyLog = [];
				window._flyLog.push('[WATCH-SQ] frame=' + $0 + ' off=0x' + ($1>>>0).toString(16)
					+ ' first=0x' + ($2>>>0).toString(16));
			}, FrameCount, ramOff, *(const u32*)data);
		}
	}
}
static inline void fly_sq_note(u32 address) {
	g_fly_sq[0]++;
	if (((address >> 26) & 7) == 3) {
		g_fly_sq[1]++;
		u32 off = address & RAM_MASK;
		if (off >= 0x6dae00 && off < 0x740000) g_fly_sq[2]++;
	} else if (((address >> 26) & 7) == 4) {
		g_fly_sq[3]++;
	}
}
#endif

template<bool mmu_on>
static void DYNACALL sqWrite(u32 dest, Sh4Context *ctx)
{
	u32 address;
	//Translate the SQ addresses as needed
	if (mmu_on)
	{
		mmu_TranslateSQW(dest, &address);
	}
	else
	{
		//sanity/optimisation check
		//verify(CCN_QACR_TR[0]==CCN_QACR_TR[1]);

		u32 QACR = CCN_QACR_TR[0];
		//QACR has already 0xE000_0000
		address = QACR + (dest & ~0x1f);
	}

	if (((address >> 26) & 7) != 4)//Area 4
	{
		const SQBuffer *sq = &ctx->sq_buffer[(dest >> 5) & 1];
		WriteMemBlock_nommu_sq(address, sq);
	}
	else
	{
		TAWriteSQ(address, ctx->sq_buffer);	// TODO pass the correct SQBuffer instead of letting TAWriteSQ deal with it
	}
#if defined(__EMSCRIPTEN__) && !defined(JIT_PROD_BUILD)
	fly_sq_note(address);
	g_fly_sq[4]++;  // generic QACR-translated handler used
	g_fly_sq[5] = CCN_QACR_TR[0];
#endif
}

//yes, this micro optimization makes a difference
static void DYNACALL sqWrite_nommu_area_3(u32 dest, Sh4Context *ctx)
{
	SQBuffer *pmem = (SQBuffer *)((u8 *)ctx + sizeof(Sh4Context) + 0x0C000000);
	pmem += (dest & (RAM_SIZE_MAX - 1)) >> 5;
	*pmem = ctx->sq_buffer[(dest >> 5) & 1];
}

static void DYNACALL sqWrite_nommu_area_3_nonvmem(u32 dest, Sh4Context *ctx)
{
	u8* pmem = &mem_b[0];

	memcpy((SQBuffer *)&pmem[dest & (RAM_MASK - 0x1F)], &ctx->sq_buffer[(dest >> 5) & 1], sizeof(SQBuffer));
#if defined(__EMSCRIPTEN__) && !defined(JIT_PROD_BUILD)
	{
		u32 off = dest & (RAM_MASK - 0x1F);
		g_fly_sq[0]++; g_fly_sq[1]++;
		if (off >= 0x6dae00 && off < 0x740000) g_fly_sq[2]++;
		fly_sq_watch(off, &ctx->sq_buffer[(dest >> 5) & 1]);
	}
#endif
}

static void DYNACALL sqWriteTA(u32 dest, Sh4Context *ctx)
{
	TAWriteSQ(dest, ctx->sq_buffer);
#if defined(__EMSCRIPTEN__) && !defined(JIT_PROD_BUILD)
	g_fly_sq[0]++; g_fly_sq[3]++;
#endif
}

void setSqwHandler()
{
	Sh4Context& ctx = p_sh4rcb->cntx;
	if (CCN_MMUCR.AT == 1)
	{
		ctx.doSqWrite = &sqWrite<true>;
	}
	else
	{
		u32 area = CCN_QACR0.Area;

		CCN_QACR_TR[0] = (area << 26) - 0xE0000000; //-0xE0000000 because 0xE0000000 is added on the translation again ...

		switch (area)
		{
		case 3:
			if (addrspace::virtmemEnabled())
				ctx.doSqWrite = &sqWrite_nommu_area_3;
			else
				ctx.doSqWrite = &sqWrite_nommu_area_3_nonvmem;
			break;

		case 4:
			ctx.doSqWrite = &sqWriteTA;
			break;

		default:
			ctx.doSqWrite = &sqWrite<false>;
			break;
		}
	}
}

/*
	DMAC is not really emulated on nullDC. We just fake the dmas ;p
		Dreamcast uses sh4's dmac in ddt mode to multiplex ch0 and ch2 for dma access.
		nullDC just 'fakes' each dma as if it was a full channel, never bothering properly
		updating the dmac regs -- it works just fine really :|
*/
#include "types.h"
#include "hw/sh4/sh4_mmr.h"
#include "hw/holly/sb.h"
#include "hw/sh4/sh4_mem.h"
#include "hw/pvr/pvr_mem.h"
#include "dmac.h"
#include "hw/sh4/sh4_interrupts.h"
#include "hw/holly/holly_intc.h"
#if defined(__EMSCRIPTEN__) && !defined(JIT_PROD_BUILD)
#include <emscripten.h>
#include "hw/sh4/sh4_core.h"   // Sh4cntx — CH2-DMA storm probe
#endif

DMACRegisters dmac;

void DMAC_Ch2St()
{
	u32 dmaor = DMAC_DMAOR.full;

	u32 src = DMAC_SAR(2) & 0x1fffffe0;
	u32 dst = SB_C2DSTAT & 0x01ffffe0;
	u32 len = SB_C2DLEN;

	if (0x8201 != (dmaor & DMAOR_MASK))
	{
		INFO_LOG(SH4, "DMAC: DMAOR has invalid settings (%X) !", dmaor);
#if defined(__EMSCRIPTEN__) && !defined(JIT_PROD_BUILD)
		// CH2-DMA STORM PROBE (2026-06-12): this silent return swallows the
		// kick — no completion int, no TE, no C2DST clear → the game's TA-DMA
		// submit loop (JGR: 0x8c21faac) retries forever. Log it loudly.
		{
			static u32 bador = 0;
			bador++;
			if (bador <= 20 || (bador & 0x3FF) == 0)
				EM_ASM({
					if (!window._flyLog) window._flyLog = [];
					window._flyLog.push('[CH2-BADOR] #' + $0 + ' DMAOR=0x' + ($1>>>0).toString(16)
						+ ' pc=0x' + ($2>>>0).toString(16) + ' CHCR2=0x' + ($3>>>0).toString(16));
				}, bador, dmaor, Sh4cntx.pc, DMAC_CHCR(2).full);
		}
#endif
		return;
	}
	if ((src >> 26) != 3)
	{
		// Source address must be in system RAM
		WARN_LOG(SH4, "DMAC: invalid source address %x dest %x len %x", DMAC_SAR(2), SB_C2DSTAT, SB_C2DLEN);
		DMAC_DMAOR.AE = 1;
		asic_RaiseInterrupt(holly_CH2_DMA);
		return;
	}

	DEBUG_LOG(SH4, ">> DMAC: Ch2 DMA SRC=%X DST=%X LEN=%X", src, SB_C2DSTAT, SB_C2DLEN);
#if defined(__EMSCRIPTEN__) && !defined(JIT_PROD_BUILD)
	// CH2-DMA transfer log: EOL storm shows ~1024 opaque-EOLs/frame vs ~4.4
	// ch2 completions/frame → each transfer must carry hundreds of EOL params
	// = garbage stream. Log src/dst/len to see whether SAR2/LEN are sane.
	{
		static u32 ch2n = 0;
		ch2n++;
		// STREAM-CONTENT SCAN at transfer time (the dump-based zero-tail read
		// was ambiguous: the game may clear the buffer post-DMA, so only the
		// content AT TRANSFER TIME is trustworthy). Count params whose PCW
		// parses as End-Of-List (type 0 = top 3 bits clear) and, of those, how
		// many are fully zero words. Expectation: clean ~1 EOL/transfer-ish;
		// prod dozens+ — and zeroEol tells zeros-in-stream vs real repeats.
		u32 eolParams = 0, zeroEol = 0, nParams = 0;
		// Zero-run geometry: discriminates tail-truncation (builder/kick race)
		// vs scattered 32B holes (lost SQ bursts) vs interior blocks (skipped
		// builder section). A "zero param" = PCW == 0 at a 32B boundary.
		u32 zeroRuns = 0, firstZeroOff = 0xFFFFFFFF, lastDataOff = 0, inRun = 0;
		if ((SB_C2DSTAT & 0x01000000) == 0 && (src & RAM_MASK) + len <= RAM_SIZE) {
			const u8* p = (const u8*)GetMemPtr(src, len);
			if (p) {
				for (u32 off = 0; off + 32 <= len; off += 32) {
					u32 pcw = *(const u32*)(p + off);
					nParams++;
					if ((pcw >> 29) == 0) {
						eolParams++;
						if (pcw == 0) zeroEol++;
					}
					if (pcw == 0) {
						if (!inRun) { zeroRuns++; inRun = 1; if (firstZeroOff == 0xFFFFFFFF) firstZeroOff = off; }
					} else {
						inRun = 0;
						lastDataOff = off;
					}
				}
			}
		}
		extern u32 FrameCount;
		if (ch2n <= 40 || eolParams > 8 || (ch2n & 0xFF) == 0)
			EM_ASM({
				if (!window._flyLog) window._flyLog = [];
				window._flyLog.push('[CH2-DMA] #' + $0 + ' frame=' + $1
					+ ' len=0x' + ($2>>>0).toString(16)
					+ ' params=' + $3 + ' eol=' + $4 + ' zeroEol=' + $5
					+ ' runs=' + $6 + ' firstZero=0x' + ($7>>>0).toString(16)
					+ ' lastData=0x' + ($8>>>0).toString(16));
			}, ch2n, FrameCount, SB_C2DLEN, nParams, eolParams, zeroEol,
			   zeroRuns, firstZeroOff, lastDataOff);
		// Adopt this transfer's first hole as the live watch target (dynamic
		// write-provenance: see g_fly_watch_off in rec_wasm.cpp). Only when a
		// real anomaly exists (>8 EOLs) to avoid chasing the legit terminator.
		if (eolParams > 8 && firstZeroOff != 0xFFFFFFFF) {
			extern u32 g_fly_watch_off[2];
			u32 newoff = (src & RAM_MASK) + firstZeroOff;
			if (g_fly_watch_off[0] != newoff) {
				g_fly_watch_off[1] = g_fly_watch_off[0];
				g_fly_watch_off[0] = newoff;
				EM_ASM({
					if (!window._flyLog) window._flyLog = [];
					window._flyLog.push('[WATCH-SET] frame=' + $0
						+ ' off=0x' + ($1>>>0).toString(16));
				}, FrameCount, newoff);
			}
		}
	}
#endif

	// Direct DList DMA (Ch2)

	// TA FIFO - Polygon and YUV converter paths and mirror
	// 10000000 - 10FFFFE0
	// 12000000 - 12FFFFE0
	if ((dst & 0x01000000) == 0)
	{
		if ((src & RAM_MASK) + len > RAM_SIZE)
		{
			u32 newLen = RAM_SIZE - (src & RAM_MASK);
			SQBuffer *psrc = (SQBuffer *)GetMemPtr(src, newLen);
			TAWrite(dst, psrc, newLen / sizeof(SQBuffer));
			len -= newLen;
			src += newLen;
		}
		SQBuffer *psrc = (SQBuffer *)GetMemPtr(src, len);
		TAWrite(dst, psrc, len / sizeof(SQBuffer));
		src += len;
	}
	// Direct Texture path and mirror
	// 11000000 - 11FFFFE0
	// 13000000 - 13FFFFE0
	else
	{
		bool path64b = SB_C2DSTAT & 0x02000000 ? SB_LMMODE1 == 0 : SB_LMMODE0 == 0;

		if (path64b)
		{
			// 64-bit path
			dst = (dst & 0x00FFFFFF) | 0xa4000000;
			if ((src & RAM_MASK) + len > RAM_SIZE)
			{
				u32 newLen = RAM_SIZE - (src & RAM_MASK);
				WriteMemBlock_nommu_dma(dst, src, newLen);
				len -= newLen;
				src += newLen;
				dst += newLen;
			}
			WriteMemBlock_nommu_dma(dst, src, len);
			src += len;
			dst += len;
		}
		else
		{
			// 32-bit path
			dst = (dst & 0xFFFFFF) | 0xa5000000;
			while (len > 0)
			{
				u32 v = ReadMem32_nommu(src);
				pvr_write32p<u32>(dst, v);
				len -= 4;
				src += 4;
				dst += 4;
			}
		}
		SB_C2DSTAT = dst;
	}

	// Setup some of the regs so it thinks we've finished DMA

	DMAC_CHCR(2).TE = 1;
	DMAC_DMATCR(2) = 0;

	SB_C2DST = 0;
	SB_C2DLEN = 0;

	asic_RaiseInterrupt(holly_CH2_DMA);
}

static const InterruptID dmac_itr[] = { sh4_DMAC_DMTE0, sh4_DMAC_DMTE1, sh4_DMAC_DMTE2, sh4_DMAC_DMTE3 };

template<u32 ch>
static void WriteCHCR(u32 addr, u32 data)
{
	if constexpr (ch == 0 || ch == 1)
		DMAC_CHCR(ch).full = data & 0xff0ffff7;
	else
		// no AL or RL on channels 2 and 3
		DMAC_CHCR(ch).full = data & 0xff0afff7;

	if (DMAC_CHCR(ch).TE == 0 && DMAC_CHCR(ch).DE && DMAC_DMAOR.DME)
	{
		if (DMAC_CHCR(ch).RS == 4)
		{
			DEBUG_LOG(SH4, "DMAC: Manual DMA ch:%d TS:%d src: %08X dst: %08X len: %08X SM: %d, DM: %d", ch, DMAC_CHCR(ch).TS,
					DMAC_SAR(ch), DMAC_DAR(ch), DMAC_DMATCR(ch), DMAC_CHCR(ch).SM, DMAC_CHCR(ch).DM);
			u32 src = DMAC_SAR(ch);
			u32 len = DMAC_DMATCR(ch);
			u32 dst = DMAC_DAR(ch);

			int srcIncr, dstIncr;
			switch (DMAC_CHCR(ch).SM)
			{
			case 1:
				srcIncr = 1;
				break;
			case 2:
				srcIncr = -1;
				break;
			default:
				srcIncr = 0;
				break;
			}
			switch (DMAC_CHCR(ch).DM)
			{
			case 1:
				dstIncr = 1;
				break;
			case 2:
				dstIncr = -1;
				break;
			default:
				dstIncr = 0;
				break;
			}

			switch (DMAC_CHCR(ch).TS)
			{
			case 0:	// 64 bits
				srcIncr *= sizeof(u64);
				dstIncr *= sizeof(u64);
				for (; len != 0; len--)
				{
					u64 data = addrspace::read64(src);
					addrspace::write64(dst, data);
					src += srcIncr;
					dst += dstIncr;
				}
				break;

			case 1: // 8 bits
				for (; len != 0; len--)
				{
					u8 data = addrspace::read8(src);
					addrspace::write8(dst, data);
					src += srcIncr;
					dst += dstIncr;
				}
				break;

			case 2: // 16 bits
				srcIncr *= sizeof(u16);
				dstIncr *= sizeof(u16);
				for (; len != 0; len--)
				{
					u16 data = addrspace::read16(src);
					addrspace::write16(dst, data);
					src += srcIncr;
					dst += dstIncr;
				}
                break;

			case 4: // 32-byte block
				len *= 32 / sizeof(u32);
				[[fallthrough]];

            default: // 32 bits
				srcIncr *= sizeof(u32);
				dstIncr *= sizeof(u32);
				for (; len != 0; len--)
				{
					u32 data = addrspace::read32(src);
					addrspace::write32(dst, data);
					src += srcIncr;
					dst += dstIncr;
				}
				break;
            }
            DMAC_CHCR(ch).TE = 1;
           	DMAC_SAR(ch) = src;
           	DMAC_DAR(ch) = dst;
           	DMAC_DMATCR(ch) = len;
        }

        InterruptPend(dmac_itr[ch], DMAC_CHCR(ch).TE);
        InterruptMask(dmac_itr[ch], DMAC_CHCR(ch).IE);
    }
}

//Init term res
void DMACRegisters::init()
{
	super::init();

	//DMAC SAR0 0xFFA00000 0x1FA00000 32 Undefined Undefined Held Held Bclk
	setRW<DMAC_SAR0_addr>();

	//DMAC DAR0 0xFFA00004 0x1FA00004 32 Undefined Undefined Held Held Bclk
	setRW<DMAC_DAR0_addr>();

	//DMAC DMATCR0 0xFFA00008 0x1FA00008 32 Undefined Undefined Held Held Bclk
	setRW<DMAC_DMATCR0_addr, u32, 0x00ffffff>();

	//DMAC CHCR0 0xFFA0000C 0x1FA0000C 32 0x00000000 0x00000000 Held Held Bclk
	setWriteHandler<DMAC_CHCR0_addr>(WriteCHCR<0>);

	//DMAC SAR1 0xFFA00010 0x1FA00010 32 Undefined Undefined Held Held Bclk
	setRW<DMAC_SAR1_addr>();

	//DMAC DAR1 0xFFA00014 0x1FA00014 32 Undefined Undefined Held Held Bclk
	setRW<DMAC_DAR1_addr>();

	//DMAC DMATCR1 0xFFA00018 0x1FA00018 32 Undefined Undefined Held Held Bclk
	setRW<DMAC_DMATCR1_addr, u32, 0x00ffffff>();

	//DMAC CHCR1 0xFFA0001C 0x1FA0001C 32 0x00000000 0x00000000 Held Held Bclk
	setWriteHandler<DMAC_CHCR1_addr>(WriteCHCR<1>);

	//DMAC SAR2 0xFFA00020 0x1FA00020 32 Undefined Undefined Held Held Bclk
	setRW<DMAC_SAR2_addr>();

	//DMAC DAR2 0xFFA00024 0x1FA00024 32 Undefined Undefined Held Held Bclk
	setRW<DMAC_DAR2_addr>();

	//DMAC DMATCR2 0xFFA00028 0x1FA00028 32 Undefined Undefined Held Held Bclk
	setRW<DMAC_DMATCR2_addr, u32, 0x00ffffff>();

	//DMAC CHCR2 0xFFA0002C 0x1FA0002C 32 0x00000000 0x00000000 Held Held Bclk
	setWriteHandler<DMAC_CHCR2_addr>(WriteCHCR<2>);

	//DMAC SAR3 0xFFA00030 0x1FA00030 32 Undefined Undefined Held Held Bclk
	setRW<DMAC_SAR3_addr>();

	//DMAC DAR3 0xFFA00034 0x1FA00034 32 Undefined Undefined Held Held Bclk
	setRW<DMAC_DAR3_addr>();

	//DMAC DMATCR3 0xFFA00038 0x1FA00038 32 Undefined Undefined Held Held Bclk
	setRW<DMAC_DMATCR3_addr, u32, 0x00ffffff>();

	//DMAC CHCR3 0xFFA0003C 0x1FA0003C 32 0x00000000 0x00000000 Held Held Bclk
	setWriteHandler<DMAC_CHCR3_addr>(WriteCHCR<3>);

	//DMAC DMAOR 0xFFA00040 0x1FA00040 32 0x00000000 0x00000000 Held Held Bclk
	setRW<DMAC_DMAOR_addr, u32, 0x00008307>();

	reset();
}

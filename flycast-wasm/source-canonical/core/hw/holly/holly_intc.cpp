#include "holly_intc.h"
#include "sb.h"
#include "hw/sh4/sh4_interrupts.h"

/*
	ASIC Interrupt controller
	part of the holly block on dc
*/

//asic_RLXXPending: Update the intc flags for pending interrupts
static void asic_RL6Pending()
{
	bool t1 = (SB_ISTNRM & SB_IML6NRM) != 0;
	bool t2 = (SB_ISTERR & SB_IML6ERR) != 0;
	bool t3 = (SB_ISTEXT & SB_IML6EXT) != 0;
	bool t4 = (SB_ISTNRM1 & SB_IML6NRM) != 0;

	InterruptPend(sh4_IRL_9, t1 || t2 || t3 || t4);
}

static void asic_RL4Pending()
{
	bool t1 = (SB_ISTNRM & SB_IML4NRM) != 0;
	bool t2 = (SB_ISTERR & SB_IML4ERR) != 0;
	bool t3 = (SB_ISTEXT & SB_IML4EXT) != 0;
	bool t4 = (SB_ISTNRM1 & SB_IML4NRM) != 0;

	InterruptPend(sh4_IRL_11, t1 || t2 || t3 || t4);
}

static void asic_RL2Pending()
{
	bool t1 = (SB_ISTNRM & SB_IML2NRM) != 0;
	bool t2 = (SB_ISTERR & SB_IML2ERR) != 0;
	bool t3 = (SB_ISTEXT & SB_IML2EXT) != 0;
	bool t4 = (SB_ISTNRM1 & SB_IML2NRM) != 0;

	InterruptPend(sh4_IRL_13, t1 || t2 || t3 || t4);
}

#if defined(__EMSCRIPTEN__) && !defined(JIT_PROD_BUILD)
#include <emscripten.h>
extern "C" {
u32 g_fly_asic_total = 0;        // every asic_RaiseInterrupt call
u32 g_fly_asic_vblank_in = 0;    // holly_SCANINT1 = 0x0003
u32 g_fly_asic_vblank_out = 0;   // holly_SCANINT2 = 0x0004
u32 g_fly_asic_hblank = 0;       // holly_HBLank = 0x0005
u32 g_fly_asic_maple = 0;        // holly_MAPLE_DMA = 0x0012
// Full per-ID histogram: index = (type << 5) | bit, type 0=NRM 1=EXT 2=ERR.
// For the JGR interrupt-storm hunt: production raises ASIC interrupts at up
// to ~360/vblank (clean: 10-14) — this identifies WHICH interrupt storms.
u32 g_fly_asic_by_id[96];
u32 EMSCRIPTEN_KEEPALIVE fly_asic_hist_ptr() { return (u32)(uintptr_t)g_fly_asic_by_id; }
u32 EMSCRIPTEN_KEEPALIVE fly_asic_hist_len() { return 96; }
// Raise-vs-ack accounting (dropped-delivery hunt, 2026-06-12): if the game's
// ISR misses a CH2-DMA-end delivery, its display-list append pointer never
// resets and lists accumulate — the observed storm shape. An "ack" = the game
// write-clearing an ISTNRM bit that was SET (it saw + handled the interrupt).
// "clears(unset)" = write-clearing an already-clear bit (double delivery).
// [0] CH2 raises [1] CH2 acks(set) [2] CH2 clears(unset)
// [3] OPQ raises [4] OPQ acks(set) [5] OPQ clears(unset)
// [6] ISTNRM reads seeing CH2 set [7] reads seeing OPQ set
u32 g_fly_ack[8];
u32 EMSCRIPTEN_KEEPALIVE fly_ack_ptr() { return (u32)(uintptr_t)g_fly_ack; }
u32 EMSCRIPTEN_KEEPALIVE fly_ack_len() { return 8; }
}
#endif

void asic_RaiseInterrupt(HollyInterruptID inter)
{
#if defined(__EMSCRIPTEN__) && !defined(JIT_PROD_BUILD)
	g_fly_asic_total++;
	g_fly_asic_by_id[(((u32)inter >> 8) << 5) | ((u32)inter & 31)]++;
	if ((u32)inter == 0x0013) g_fly_ack[0]++;       // CH2-DMA-end raise
	else if ((u32)inter == 0x0007) g_fly_ack[3]++;  // opaque-EOL raise
	switch ((u32)inter) {
	case 0x0003: g_fly_asic_vblank_in++;  break;
	case 0x0004: g_fly_asic_vblank_out++; break;
	case 0x0005: g_fly_asic_hblank++;     break;
	case 0x0012: g_fly_asic_maple++;      break;
	}
#endif
	u8 type = inter >> 8;
	u32 mask = 1 << (u8)inter;
	switch(type)
	{
	case 0:
		SB_ISTNRM |= mask;
		break;
	case 1:
		SB_ISTEXT |= mask;
		break;
	case 2:
		SB_ISTERR |= mask;
		break;
	}
	asic_RL2Pending();
	asic_RL4Pending();
	asic_RL6Pending();
}

void asic_RaiseInterruptBothCLX(HollyInterruptID inter)
{
	u8 type = inter >> 8;
	u32 mask = 1 << (u8)inter;
	switch(type)
	{
	case 0:
		SB_ISTNRM1 |= mask;
		SB_ISTNRM |= mask;
		break;
	case 1:
		SB_ISTEXT |= mask;
		break;
	case 2:
		SB_ISTERR |= mask;
		break;
	}
	asic_RL2Pending();
	asic_RL4Pending();
	asic_RL6Pending();
}

template<bool Naomi2>
static u32 Read_SB_ISTNRM(u32 addr)
{
	/* Note that the two highest bits indicate
	 * the OR'ed result of all the bits in
	 * SB_ISTEXT and SB_ISTERR, respectively,
	 * and writes to these two bits are ignored. */
	u32 tmp = (Naomi2 && (addr & 0x02000000) != 0 ? SB_ISTNRM1 : SB_ISTNRM) & 0x3FFFFFFF;
#if defined(__EMSCRIPTEN__) && !defined(JIT_PROD_BUILD)
	{
		extern u32 g_fly_ack[8];
		if (tmp & (1u << 19)) g_fly_ack[6]++;
		if (tmp & (1u << 7))  g_fly_ack[7]++;
	}
#endif

	if (SB_ISTEXT)
		tmp|=0x40000000;

	if (SB_ISTERR)
		tmp|=0x80000000;

	return tmp;
}

template<bool Naomi2>
static void Write_SB_ISTNRM(u32 addr, u32 data)
{
#if defined(__EMSCRIPTEN__) && !defined(JIT_PROD_BUILD)
	{
		extern u32 g_fly_ack[8];
		if (data & (1u << 19)) ((SB_ISTNRM & (1u << 19)) ? g_fly_ack[1] : g_fly_ack[2])++;
		if (data & (1u << 7))  ((SB_ISTNRM & (1u << 7))  ? g_fly_ack[4] : g_fly_ack[5])++;
	}
#endif
	/* writing a 1 clears the interrupt */
	if (Naomi2 && (addr & 0x02000000) != 0)
		SB_ISTNRM1 &= ~data;
	else
		SB_ISTNRM &= ~data;

	asic_RL2Pending();
	asic_RL4Pending();
	asic_RL6Pending();
}

void asic_CancelInterrupt(HollyInterruptID inter)
{
	u8 type = inter >> 8;
	u32 mask = ~(1 << (u8)inter);
	switch (type)
	{
	case 0:
		SB_ISTNRM &= mask;
		break;
	case 1:
		SB_ISTEXT &= mask;
		break;
	case 2:
		SB_ISTERR &= mask;
		break;
	}
	asic_RL2Pending();
	asic_RL4Pending();
	asic_RL6Pending();
}

static void Write_SB_ISTEXT(u32 addr, u32 data)
{
	//nothing happens -- asic_CancelInterrupt is used instead
}

static void Write_SB_ISTERR(u32 addr, u32 data)
{
	SB_ISTERR &= ~data;

	asic_RL2Pending();
	asic_RL4Pending();
	asic_RL6Pending();
}

template<bool Naomi2>
static void Write_SB_IML6NRM(u32 addr, u32 data)
{
	if (Naomi2 && (addr & 0x2000000) != 0)
		// Ignore CLXB settings
		return;
	SB_IML6NRM = data;

	asic_RL6Pending();
}

template<bool Naomi2>
static void Write_SB_IML4NRM(u32 addr, u32 data)
{
	if (Naomi2 && (addr & 0x2000000) != 0)
		// Ignore CLXB settings
		return;
	SB_IML4NRM = data;

	asic_RL4Pending();
}

template<bool Naomi2>
static void Write_SB_IML2NRM(u32 addr, u32 data)
{
	if (Naomi2 && (addr & 0x2000000) != 0)
		// Ignore CLXB settings
		return;
	SB_IML2NRM = data;

	asic_RL2Pending();
}

template<bool Naomi2>
static void Write_SB_IML6EXT(u32 addr, u32 data)
{
	if (Naomi2 && (addr & 0x2000000) != 0)
		// Ignore CLXB settings
		return;
	SB_IML6EXT = data;

	asic_RL6Pending();
}

template<bool Naomi2>
static void Write_SB_IML4EXT(u32 addr, u32 data)
{
	if (Naomi2 && (addr & 0x2000000) != 0)
		// Ignore CLXB settings
		return;
	SB_IML4EXT = data;

	asic_RL4Pending();
}

template<bool Naomi2>
static void Write_SB_IML2EXT(u32 addr, u32 data)
{
	if (Naomi2 && (addr & 0x2000000) != 0)
		// Ignore CLXB settings
		return;
	SB_IML2EXT = data;

	asic_RL2Pending();
}

template<bool Naomi2>
static void Write_SB_IML6ERR(u32 addr, u32 data)
{
	if (Naomi2 && (addr & 0x2000000) != 0)
		// Ignore CLXB settings
		return;
	SB_IML6ERR = data;

	asic_RL6Pending();
}

template<bool Naomi2>
static void Write_SB_IML4ERR(u32 addr, u32 data)
{
	if (Naomi2 && (addr & 0x2000000) != 0)
		// Ignore CLXB settings
		return;
	SB_IML4ERR = data;

	asic_RL4Pending();
}

template<bool Naomi2>
static void Write_SB_IML2ERR(u32 addr, u32 data)
{
	if (Naomi2 && (addr & 0x2000000) != 0)
		// Ignore CLXB settings
		return;
	SB_IML2ERR = data;

	asic_RL2Pending();
}

void asic_reg_Init()
{
}

void asic_reg_Term()
{

}
//Reset -> Reset - Initialise to default values
void asic_reg_Reset(bool hard)
{
	if (hard)
	{
		hollyRegs.setWriteHandler<SB_ISTEXT_addr>(Write_SB_ISTEXT);
		hollyRegs.setWriteHandler<SB_ISTERR_addr>(Write_SB_ISTERR);

		if (settings.platform.isNaomi2())
		{
			hollyRegs.setHandlers<SB_ISTNRM_addr>(Read_SB_ISTNRM<true>, Write_SB_ISTNRM<true>);

			//NRM
			//6
			hollyRegs.setWriteHandler<SB_IML6NRM_addr>(Write_SB_IML6NRM<true>);
			//4
			hollyRegs.setWriteHandler<SB_IML4NRM_addr>(Write_SB_IML4NRM<true>);
			//2
			hollyRegs.setWriteHandler<SB_IML2NRM_addr>(Write_SB_IML2NRM<true>);
			//EXT
			//6
			hollyRegs.setWriteHandler<SB_IML6EXT_addr>(Write_SB_IML6EXT<true>);
			//4
			hollyRegs.setWriteHandler<SB_IML4EXT_addr>(Write_SB_IML4EXT<true>);
			//2
			hollyRegs.setWriteHandler<SB_IML2EXT_addr>(Write_SB_IML2EXT<true>);
			//ERR
			//6
			hollyRegs.setWriteHandler<SB_IML6ERR_addr>(Write_SB_IML6ERR<true>);
			//4
			hollyRegs.setWriteHandler<SB_IML4ERR_addr>(Write_SB_IML4ERR<true>);
			//2
			hollyRegs.setWriteHandler<SB_IML2ERR_addr>(Write_SB_IML2ERR<true>);
		}
		else
		{
			hollyRegs.setHandlers<SB_ISTNRM_addr>(Read_SB_ISTNRM<false>, &Write_SB_ISTNRM<false>);

			//NRM
			//6
			hollyRegs.setWriteHandler<SB_IML6NRM_addr>(Write_SB_IML6NRM<false>);
			//4
			hollyRegs.setWriteHandler<SB_IML4NRM_addr>(Write_SB_IML4NRM<false>);
			//2
			hollyRegs.setWriteHandler<SB_IML2NRM_addr>(Write_SB_IML2NRM<false>);
			//EXT
			//6
			hollyRegs.setWriteHandler<SB_IML6EXT_addr>(Write_SB_IML6EXT<false>);
			//4
			hollyRegs.setWriteHandler<SB_IML4EXT_addr>(Write_SB_IML4EXT<false>);
			//2
			hollyRegs.setWriteHandler<SB_IML2EXT_addr>(Write_SB_IML2EXT<false>);
			//ERR
			//6
			hollyRegs.setWriteHandler<SB_IML6ERR_addr>(Write_SB_IML6ERR<false>);
			//4
			hollyRegs.setWriteHandler<SB_IML4ERR_addr>(Write_SB_IML4ERR<false>);
			//2
			hollyRegs.setWriteHandler<SB_IML2ERR_addr>(Write_SB_IML2ERR<false>);
		}
	}
}


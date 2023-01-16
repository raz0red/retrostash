#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "config.h"
#include <string.h>	/* memcpy, memset */

#include "atari.h"

#define dGetByte(x)			(memory[x])
#define dPutByte(x, y)			(memory[x] = y)

#ifdef MSB_FIRST
/* can't do any word optimizations for big endian machines */
#define dGetWord(x)		(memory[x] + (memory[(x) + 1] << 8))
#define dPutWord(x, y)		(memory[x] = (UBYTE) (y), memory[(x) + 1] = (UBYTE) ((y) >> 8))
#define dGetWordAligned(x)	dGetWord(x)
#define dPutWordAligned(x, y)	dPutWord(x, y)
#else
#ifdef WORDS_UNALIGNED_OK
#define dGetWord(x)		UNALIGNED_GET_WORD(&memory[x], memory_read_word_stat)
#define dPutWord(x, y)		UNALIGNED_PUT_WORD(&memory[x], (y), memory_write_word_stat)
#define dGetWordAligned(x)	UNALIGNED_GET_WORD(&memory[x], memory_read_aligned_word_stat)
#define dPutWordAligned(x, y)	UNALIGNED_PUT_WORD(&memory[x], (y), memory_write_aligned_word_stat)
#else	/* WORDS_UNALIGNED_OK */
#define dGetWord(x)		(memory[x] + (memory[(x) + 1] << 8))
#define dPutWord(x, y)		(memory[x] = (UBYTE) (y), memory[(x) + 1] = (UBYTE) ((y) >> 8))
/* faster versions of dGetWord and dPutWord for even addresses */
/* TODO: guarantee that memory is UWORD-aligned and use UWORD access */
#define dGetWordAligned(x)	dGetWord(x)
#define dPutWordAligned(x, y)	dPutWord(x, y)
#endif	/* WORDS_UNALIGNED_OK */
#endif

#define dCopyFromMem(from, to, size)	memcpy(to, memory + (from), size)
#define dCopyToMem(from, to, size)		memcpy(memory + (to), from, size)
#define dFillMem(addr1, value, length)	memset(memory + (addr1), value, length)

extern UBYTE memory[65536 + 2];

#define RAM       0
#define ROM       1
#define HARDWARE  2

extern UBYTE attrib[65536];
#define GetByte(addr)		(attrib[addr] == HARDWARE ? Atari800_GetByte(addr) : memory[addr])
#define PutByte(addr, byte)	 do { if (attrib[addr] == RAM) memory[addr] = byte; else if (attrib[addr] == HARDWARE) Atari800_PutByte(addr, byte); } while (0)
#define SetRAM(addr1, addr2) memset(attrib + (addr1), RAM, (addr2) - (addr1) + 1)
#define SetROM(addr1, addr2) memset(attrib + (addr1), ROM, (addr2) - (addr1) + 1)
#define SetHARDWARE(addr1, addr2) memset(attrib + (addr1), HARDWARE, (addr2) - (addr1) + 1)

extern int cartA0BF_enabled;

void MEMORY_InitialiseMachine(void);
void MemStateSave(UBYTE SaveVerbose);
void MemStateRead(UBYTE SaveVerbose);
void CopyFromMem(UWORD from, UBYTE *to, int size);
void CopyToMem(const UBYTE *from, UWORD to, int size);
#define CopyROM(addr1, addr2, src) memcpy(memory + (addr1), src, (addr2) - (addr1) + 1)

#endif /* _MEMORY_H_ */

// ---------------------------------------------------------------------------------------
//  SRAM.C - SRAM (16kb) �ΰ�
// ---------------------------------------------------------------------------------------

#include	"common.h"
#include	"fileio.h"
#include	"prop.h"
#include	"winx68k.h"
#include	"sysport.h"
#include	"x68kmemory.h"
#include	"sram.h"

uint8_t	SRAM[0x4000];
uint8_t	SRAMFILE[] = "sram.dat";

// -----------------------------------------------------------------------
//   ���Ω���ʤ������뤹�����å�
// -----------------------------------------------------------------------
void SRAM_VirusCheck(void)
{
	if (!Config.SRAMWarning) return;				// Warningȯ���⡼�ɤǤʤ���е���

	if ( (cpu_readmem24_dword(0xed3f60)==0x60000002)
	   &&(cpu_readmem24_dword(0xed0010)==0x00ed3f60) )		// ���ꤦ���뤹�ˤ��������ʤ��菢�
	{
		SRAM_Cleanup();
		SRAM_Init();			// Virus���꡼�󥢥å׸�Υǡ�����񤭹���Ǥ���
	}
}


// -----------------------------------------------------------------------
//   �����
// -----------------------------------------------------------------------
void SRAM_Init(void)
{
	int i;
	uint8_t tmp;
	FILEH fp;

	for (i=0; i<0x4000; i++)
		SRAM[i] = 0xFF;

	fp = File_OpenCurDir(SRAMFILE);
	if (fp)
	{
		File_Read(fp, SRAM, 0x4000);
		File_Close(fp);
		for (i=0; i<0x4000; i+=2)
		{
			tmp = SRAM[i];
			SRAM[i] = SRAM[i+1];
			SRAM[i+1] = tmp;
		}
	}
}


// -----------------------------------------------------------------------
//   ű�����
// -----------------------------------------------------------------------
void SRAM_Cleanup(void)
{
	int i;
	uint8_t tmp;
	FILEH fp;

	for (i=0; i<0x4000; i+=2)
	{
		tmp = SRAM[i];
		SRAM[i] = SRAM[i+1];
		SRAM[i+1] = tmp;
	}

	fp = File_OpenCurDir(SRAMFILE);
	if (!fp)
		fp = File_CreateCurDir(SRAMFILE, FTYPE_SRAM);
	if (fp)
	{
		File_Write(fp, SRAM, 0x4000);
		File_Close(fp);
	}
}


// -----------------------------------------------------------------------
//   �꡼��
// -----------------------------------------------------------------------
uint8_t FASTCALL SRAM_Read(DWORD adr)
{
	adr &= 0xffff;
	adr ^= 1;
	if (adr<0x4000)
		return SRAM[adr];
	return 0xff;
}


// -----------------------------------------------------------------------
//   �餤��
// -----------------------------------------------------------------------
void FASTCALL SRAM_Write(DWORD adr, uint8_t data)
{
	if ( (SysPort[5]==0x31)&&(adr<0xed4000) )
	{
		adr &= 0xffff;
		adr ^= 1;
		SRAM[adr] = data;
	}
}

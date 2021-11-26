// ---------------------------------------------------------------------------------------
//  MERCURY.C - �ޡ���������ˤä�
// ---------------------------------------------------------------------------------------

#include "common.h"
#include "dswin.h"
#include "fmg_wrap.h"
#include "dmac.h"
#include "m68000.h"
#include "irqh.h"
#include "mercury.h"
#include <math.h>

#define MCRY_IRQ 4
#define Mcry_BufSize		48000*2

long	Mcry_WrPtr = 0;
long	Mcry_RdPtr = 0;
long	Mcry_SampleRate = 44100;
long	Mcry_ClockRate = 44100;
long	Mcry_Count = 0;
uint8_t	Mcry_Status = 0;
uint8_t	Mcry_LRTiming = 0;
int16_t	Mcry_OutDataL = 0;
int16_t	Mcry_OutDataR = 0;
int16_t	Mcry_BufL[Mcry_BufSize];
int16_t	Mcry_BufR[Mcry_BufSize];
long	Mcry_PreCounter = 0;

int16_t	Mcry_OldR, Mcry_OldL;
int	Mcry_DMABytes = 0;
static double Mcry_VolumeShift = 65536;
static int Mcry_SampleCnt = 0;
static uint8_t Mcry_Vector = 255;

extern DWORD BusErrFlag;
extern	m68k_regs regs;


DWORD FASTCALL Mcry_IntCB(uint8_t irq)
{
	DWORD ret = 0xffffffff;
	IRQH_IRQCallBack(irq);
	if ( irq==MCRY_IRQ ) {
		ret = (DWORD)Mcry_Vector;
	}
	return ret;
}

void FASTCALL Mcry_Int(void)
{
	IRQH_Int(MCRY_IRQ, &Mcry_IntCB);
}


static long Mcry_Clocks[8] = {
	22050, 16000, 22050, 24000
};


int Mcry_IsReady(void)
{
	return (Mcry_SampleCnt>0);
}


// -----------------------------------------------------------------------
//   MPU�в᥯��å�����ʬ�����ǡ�����Хåե���ί���
// -----------------------------------------------------------------------
void FASTCALL Mcry_PreUpdate(DWORD clock)
{
	Mcry_PreCounter += (Mcry_ClockRate*clock);
	while(Mcry_PreCounter>=10000000L)
	{
		Mcry_SampleCnt++;
		Mcry_PreCounter -= 10000000L;
	}
	M288_Timer(clock);
}


// -----------------------------------------------------------------------
//   DSound������׵�ʬ�����Хåե�������
// -----------------------------------------------------------------------
void FASTCALL Mcry_Update(int16_t *buffer, DWORD length)
{
	int data;

	if (!length) return;

	M288_Update(buffer, length);
	while (length)
	{
		if ( Mcry_WrPtr==Mcry_RdPtr ) {
			Mcry_SampleCnt = 1;
			DMA_Exec(2); DMA_Exec(2);
		}

		if (Mcry_WrPtr!=Mcry_RdPtr)
		{
			Mcry_OldL = Mcry_BufL[Mcry_RdPtr];
			Mcry_OldR = Mcry_BufR[Mcry_RdPtr];
			Mcry_RdPtr++;
			if (Mcry_RdPtr>=Mcry_BufSize) Mcry_RdPtr=0;
		}

		data = *buffer;
		data += Mcry_OldL;
		if (data>32767) data = 32767;
		else if (data<(-32768)) data = -32768;
		*(buffer++) = (int16_t)data;

		data = *buffer;
		data += Mcry_OldR;
		if (data>32767) data = 32767;
		else if (data<(-32768)) data = -32768;
		*(buffer++) = (int16_t)data;

		length--;
	}
}


// -----------------------------------------------------------------------
//   1��ʬ��1Word x 2ch�ˤΥǡ�����Хåե��˽񤭽Ф�
// -----------------------------------------------------------------------
INLINE void Mcry_WriteOne(void)
{
	while (Mcry_Count<Mcry_SampleRate)
	{
		Mcry_BufL[Mcry_WrPtr] = (int16_t)(Mcry_OutDataL/Mcry_VolumeShift);
		Mcry_BufR[Mcry_WrPtr] = (int16_t)(Mcry_OutDataR/Mcry_VolumeShift);
		Mcry_Count += Mcry_ClockRate;
		Mcry_WrPtr++;
		if (Mcry_WrPtr>=Mcry_BufSize) Mcry_WrPtr=0;
	}
	Mcry_Count -= Mcry_SampleRate;
	Mcry_SampleCnt--;
}


// -----------------------------------------------------------------------
//   I/O Write
// -----------------------------------------------------------------------
void FASTCALL Mcry_Write(DWORD adr, uint8_t data)
{
	if ((adr == 0xecc080)||(adr == 0xecc081)||(adr == 0xecc000)||(adr == 0xecc001))	// Data Port
	{
		if ( Mcry_SampleCnt<=0 ) return;
		if ( Mcry_Status&2 ) {		// Stereo
			if (Mcry_LRTiming)		// ��
			{
				if (!(Mcry_Status&8)) data=0;	// R Mute
				if (adr&1)			// Low Byte
				{
					Mcry_OutDataR = (Mcry_OutDataR&0xff00)|data;
					Mcry_LRTiming ^= 1;
					Mcry_WriteOne();
				}
				else				// High Byte
				{
					Mcry_OutDataR = (Mcry_OutDataR&0x00ff)|((WORD)data<<8);
				}
			}
			else				// ��
			{
				if (!(Mcry_Status&4)) data=0;	// L Mute
				if (adr&1)			// Low Byte
				{
					Mcry_OutDataL = (Mcry_OutDataL&0xff00)|data;
					Mcry_LRTiming ^= 1;
				}
				else				// High Byte
				{
					Mcry_OutDataL = (Mcry_OutDataL&0x00ff)|((WORD)data<<8);
				}
			}
		} else {			// Mono
			if (adr&1)			// Low Byte
			{
				Mcry_OutDataR = ((Mcry_Status&8)?((Mcry_OutDataR&0xff00)|data):0);
				Mcry_OutDataL = ((Mcry_Status&4)?((Mcry_OutDataL&0xff00)|data):0);
				Mcry_LRTiming ^= 1;
				Mcry_WriteOne();
			}
			else				// High Byte
			{
				Mcry_OutDataR = ((Mcry_Status&8)?((Mcry_OutDataR&0x00ff)|((WORD)data<<8)):0);
				Mcry_OutDataL = ((Mcry_Status&4)?((Mcry_OutDataL&0x00ff)|((WORD)data<<8)):0);
			}
		}
	}
	else if ((adr == 0xecc091)||(adr == 0xecc011))
	{
		if (Mcry_Status != data)
		{
			Mcry_Status = data;
			Mcry_SetClock();
		}
	}
	else if ( adr== 0xecc0b1 )						// Int Vector
	{
		Mcry_Vector = data;
	}
	else if ( (adr>=0xecc0c0)&&(adr<=0xecc0c7)&&(adr&1) )	// �����Ǥޡ�����꡼ OPN
	{
		M288_Write((uint8_t)((adr>>1)&3), data);
	}
}


// -----------------------------------------------------------------------
//   I/O Read
// -----------------------------------------------------------------------
uint8_t FASTCALL Mcry_Read(DWORD adr)
{
	uint8_t ret = 0;
	if ((adr == 0xecc080)||(adr == 0xecc081)||(adr == 0xecc000)||(adr == 0xecc001))
	{
	}
	else if ((adr == 0xecc0a1)||(adr == 0xecc021))	// Status Port
	{
		ret = ((Mcry_Status&0xf0)|0x0f);
	}
	else if ((adr == 0xecc091)||(adr == 0xecc011))	//
	{
	}
	else if ((adr == 0xecc090)||(adr == 0xecc010))	//
	{
		ret = (Mcry_LRTiming<<3);
		Mcry_LRTiming ^= 1;
	}
	else if ( adr== 0xecc0b1 )						// Int Vector
	{
		ret = Mcry_Vector;
	}
	else if ( (adr>=0xecc0c0)&&(adr<=0xecc0c7)&&(adr&1) )	// �����Ǥޡ�����꡼ OPN
	{
		ret = M288_Read((uint8_t)((adr>>1)&3));
	}
	else if ( adr>=0xecc100 )
	{				// Bus Error?
		BusErrFlag = 1;
	}
	return ret;
}


// -----------------------------------------------------------------------
//   ��������å�����
// -----------------------------------------------------------------------
void Mcry_SetClock(void)
{
	Mcry_ClockRate = Mcry_Clocks[(Mcry_Status>>4)&3];
	if (Mcry_Status&0x80) Mcry_ClockRate *= 2;
	Mcry_Count = 0;
	Mcry_PreCounter = 0;
//	Mcry_RdPtr = 0;
//	Mcry_WrPtr = 0;
}


// -----------------------------------------------------------------------
//   �ܤ�塼������
// -----------------------------------------------------------------------
void Mcry_SetVolume(uint8_t vol)
{
	if (vol>16) vol=16;
//	if (vol<0) vol=0;

	if (vol)
		Mcry_VolumeShift = pow(1.189207115, (16-vol));
	else
		Mcry_VolumeShift = 65536;		// Mute
	M288_SetVolume(vol);
}


// -----------------------------------------------------------------------
//   �������
// -----------------------------------------------------------------------
void Mcry_Init(DWORD samplerate, const char* path)
{
	memset(Mcry_BufL, 0, Mcry_BufSize*2);
	memset(Mcry_BufR, 0, Mcry_BufSize*2);

	Mcry_WrPtr = 0;
	Mcry_RdPtr = 0;
	Mcry_OutDataL = 0;
	Mcry_OutDataR = 0;
	Mcry_Status = 0;
	Mcry_SampleRate = (long)samplerate;
	Mcry_LRTiming = 0;
	Mcry_PreCounter = 0;

	Mcry_SetClock();

	M288_Init(8000000, samplerate, path);
}


void Mcry_Cleanup(void)
{
	M288_Cleanup();
}

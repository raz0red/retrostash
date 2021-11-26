// ---------------------------------------------------------------------------------------
//  MFP.C - MFP (Multi-Function Periferal)
// ---------------------------------------------------------------------------------------

#include "mfp.h"
#include "irqh.h"
#include "crtc.h"
#include "m68000.h"
#include "winx68k.h"
#include "keyboard.h"

uint8_t testflag=0;
uint8_t LastKey = 0;

uint8_t MFP[24];
uint8_t Timer_TBO = 0;
static uint8_t Timer_Reload[4] = {0, 0, 0, 0};
static int Timer_Tick[4] = {0, 0, 0, 0};
static const int Timer_Prescaler[8] = {1, 10, 25, 40, 125, 160, 250, 500};

// -----------------------------------------------------------------------
//   ͥ������ߤΥ����å��򤷡������٥������֤�
// -----------------------------------------------------------------------
DWORD FASTCALL MFP_IntCallback(uint8_t irq)
{
	uint8_t flag;
	DWORD vect;
	int offset = 0;
	IRQH_IRQCallBack(irq);
	if (irq!=6) return (DWORD)-1;
	for (flag=0x80, vect=15; flag; flag>>=1, vect--)
	{
		if ((MFP[MFP_IPRA]&flag)&&(MFP[MFP_IMRA]&flag)&&(!(MFP[MFP_ISRA]&flag)))
			break;
	}
	if (!flag)
	{
		offset = 1;
		for (flag=0x80, vect=7; flag; flag>>=1, vect--)
		{
			if ((MFP[MFP_IPRB]&flag)&&(MFP[MFP_IMRB]&flag)&&(!(MFP[MFP_ISRB]&flag)))
				break;
		}
	}
	if (!flag)
	{
		Error("MFP Int w/o Request. Default Vector(-1) has been returned.");
		return (DWORD)-1;
	}

	MFP[MFP_IPRA+offset] &= (~flag);
	if (MFP[MFP_VR]&8)
		MFP[MFP_ISRA+offset] |= flag;
	vect |= (MFP[MFP_VR]&0xf0);
	for (flag=0x80; flag; flag>>=1)
	{
		if ((MFP[MFP_IPRA]&flag)&&(MFP[MFP_IMRA]&flag)&&(!(MFP[MFP_ISRA]&flag)))
		{
			IRQH_Int(6, &MFP_IntCallback);
			break;
		}
		if ((MFP[MFP_IPRB]&flag)&&(MFP[MFP_IMRB]&flag)&&(!(MFP[MFP_ISRB]&flag)))
		{
			IRQH_Int(6, &MFP_IntCallback);
			break;
		}
	}
	return vect;
}


// -----------------------------------------------------------------------
//   �����ߤ����ä��ˤʤäƤʤ���Ĵ�٤ޤ�
// -----------------------------------------------------------------------
void MFP_RecheckInt(void)
{
	uint8_t flag;
	IRQH_IRQCallBack(6);
	for (flag=0x80; flag; flag>>=1)
	{
		if ((MFP[MFP_IPRA]&flag)&&(MFP[MFP_IMRA]&flag)&&(!(MFP[MFP_ISRA]&flag)))
		{
			IRQH_Int(6, &MFP_IntCallback);
			break;
		}
		if ((MFP[MFP_IPRB]&flag)&&(MFP[MFP_IMRB]&flag)&&(!(MFP[MFP_ISRB]&flag)))
		{
			IRQH_Int(6, &MFP_IntCallback);
			break;
		}
	}
}


// -----------------------------------------------------------------------
//   ������ȯ��
// -----------------------------------------------------------------------
void MFP_Int(int irq)		// 'irq' �� 0����ͥ���HSYNC/GPIP7�ˡ�15���ǲ��̡�ALARM��
{				// �٥����Ȥ��ֹ�ο��������դˤʤ�Τ���ա�
	uint8_t flag = 0x80;
	if (irq<8)
	{
		flag >>= irq;
		if (MFP[MFP_IERA]&flag)
		{
			MFP[MFP_IPRA] |= flag;
			if ((MFP[MFP_IMRA]&flag)&&(!(MFP[MFP_ISRA]&flag)))
			{
				IRQH_Int(6, &MFP_IntCallback);
			}
		}
	}
	else
	{
		irq -= 8;
		flag >>= irq;
		if (MFP[MFP_IERB]&flag)
		{
			MFP[MFP_IPRB] |= flag;
			if ((MFP[MFP_IMRB]&flag)&&(!(MFP[MFP_ISRB]&flag)))
			{
				IRQH_Int(6, &MFP_IntCallback);
			}
		}
	}
}


// -----------------------------------------------------------------------
//   �����
// -----------------------------------------------------------------------
void MFP_Init(void)
{
	int i;
	static const uint8_t initregs[24] = {
		0x7b, 0x06, 0x00, 0x18, 0x3e, 0x00, 0x00, 0x00,
		0x00, 0x18, 0x3e, 0x40, 0x08, 0x01, 0x77, 0x01,
		0x0d, 0xc8, 0x14, 0x00, 0x88, 0x01, 0x81, 0x00
	};
	memcpy(MFP, initregs, 24);
	for (i=0; i<4; i++) Timer_Tick[i] = 0;
}


// -----------------------------------------------------------------------
//   I/O Read
// -----------------------------------------------------------------------
uint8_t FASTCALL MFP_Read(DWORD adr)
{
	uint8_t reg;
	uint8_t ret = 0;
	int hpos;

	if (adr>0xe8802f) return ret;		// �Ф����顼��

	if (adr&1)
	{
		reg=(uint8_t)((adr&0x3f)>>1);
		switch(reg)
		{
		case MFP_GPIP:
			if ( (vline>=CRTC_VSTART)&&(vline<CRTC_VEND) )
				ret = 0x13;
			else
				ret = 0x03;
			hpos = (int)(ICount%HSYNC_CLK);
			if ( (hpos>=((int)CRTC_Regs[5]*HSYNC_CLK/CRTC_Regs[1]))&&(hpos<((int)CRTC_Regs[7]*HSYNC_CLK/CRTC_Regs[1])) )
				ret &= 0x7f;
			else
				ret |= 0x80;
			if (vline!=CRTC_IntLine)
				ret |= 0x40;
			break;
		case MFP_UDR:
			ret = LastKey;
			KeyIntFlag = 0;
			break;
		case MFP_RSR:
			if (KeyBufRP!=KeyBufWP)
				ret = MFP[reg] & 0x7f;
			else
				ret = MFP[reg] | 0x80;
			break;
		default:
			ret = MFP[reg];
		}
		return ret;
	}
	else
		return 0xff;
}


// -----------------------------------------------------------------------
//   I/O Write
// -----------------------------------------------------------------------
void FASTCALL MFP_Write(DWORD adr, uint8_t data)
{
	uint8_t reg;
	if (adr>0xe8802f) return;
	if (adr&1)
	{
		reg=(uint8_t)((adr&0x3f)>>1);

		switch(reg)
		{
		case MFP_IERA:
		case MFP_IERB:
			MFP[reg] = data;
			MFP[reg+2] &= data;  // �ػߤ��줿��Τ�IPRA/B����Ȥ�
			MFP_RecheckInt();
			break;
		case MFP_IPRA:
		case MFP_IPRB:
		case MFP_ISRA:
		case MFP_ISRB:
			MFP[reg] &= data;
			MFP_RecheckInt();
			break;
		case MFP_IMRA:
		case MFP_IMRB:
			MFP[reg] = data;
			MFP_RecheckInt();
			break;
		case MFP_TSR:
			MFP[reg] = data|0x80; // Tx�Ͼ��Enable��
			break;
		case MFP_TADR:
			Timer_Reload[0] = MFP[reg] = data;
			break;
		case MFP_TACR:
			MFP[reg] = data;
			break;
		case MFP_TBDR:
			Timer_Reload[1] = MFP[reg] = data;
			break;
		case MFP_TBCR:
			MFP[reg] = data;
			if ( MFP[reg]&0x10 ) Timer_TBO = 0;
			break;
		case MFP_TCDR:
			Timer_Reload[2] = MFP[reg] = data;
			break;
		case MFP_TDDR:
			Timer_Reload[3] = MFP[reg] = data;
			break;
		case MFP_TCDCR:
			MFP[reg] = data;
			break;
		case MFP_UDR:
			break;
		default:
			MFP[reg] = data;
		}
	}
}


// -----------------------------------------------------------------------
//   �����ޤλ��֤�ʤ��ʤ⾯������˽�ľ�����ġġ�
// -----------------------------------------------------------------------
void FASTCALL MFP_Timer(long clock)
{
	if ( (!(MFP[MFP_TACR]&8))&&(MFP[MFP_TACR]&7) ) {
		int t = Timer_Prescaler[MFP[MFP_TACR]&7];
		Timer_Tick[0] += clock;
		while ( Timer_Tick[0]>=t ) {
			Timer_Tick[0] -= t;
			MFP[MFP_TADR]--;
			if ( !MFP[MFP_TADR] ) {
				MFP[MFP_TADR] = Timer_Reload[0];
				MFP_Int(2);
			}
		}
	}

	if ( MFP[MFP_TBCR]&7 ) {
		int t = Timer_Prescaler[MFP[MFP_TBCR]&7];
		Timer_Tick[1] += clock;
		while ( Timer_Tick[1]>=t ) {
			Timer_Tick[1] -= t;
			MFP[MFP_TBDR]--;
			if ( !MFP[MFP_TBDR] ) {
				MFP[MFP_TBDR] = Timer_Reload[1];
				MFP_Int(7);
			}
		}
	}

	if ( MFP[MFP_TCDCR]&0x70 ) {
		int t = Timer_Prescaler[(MFP[MFP_TCDCR]&0x70)>>4];
		Timer_Tick[2] += clock;
		while ( Timer_Tick[2]>=t ) {
			Timer_Tick[2] -= t;
			MFP[MFP_TCDR]--;
			if ( !MFP[MFP_TCDR] ) {
				MFP[MFP_TCDR] = Timer_Reload[2];
				MFP_Int(10);
			}
		}
	}

	if ( MFP[MFP_TCDCR]&7 ) {
		int t = Timer_Prescaler[MFP[MFP_TCDCR]&7];
		Timer_Tick[3] += clock;
		while ( Timer_Tick[3]>=t ) {
			Timer_Tick[3] -= t;
			MFP[MFP_TDDR]--;
			if ( !MFP[MFP_TDDR] ) {
				MFP[MFP_TDDR] = Timer_Reload[3];
				MFP_Int(11);
			}
		}
	}
}


void FASTCALL MFP_TimerA(void)
{
	if ( (MFP[MFP_TACR]&15)==8 ) {					// ���٤�Ȥ�����Ȥ⡼�ɡ�VDisp�ǥ�����ȡ�
		if ( MFP[MFP_AER]&0x10 ) {				// VDisp�����ߤȥ����ߥ󥰤���äƤ�Τ����ˤʤ�Ȥ����е��ˤʤ�ʤ���
			if (vline==CRTC_VSTART) MFP[MFP_TADR]--;	// �����Ʊ�����Ȼפ�������ɤʤ��ġĤ��줸��ư���󤷡ʴ�
		} else {
			if ( CRTC_VEND>=VLINE_TOTAL ) {
				if ( (long)vline==(VLINE_TOTAL-1) ) MFP[MFP_TADR]--;	// ɽ�����֤ν����ǥ�����Ȥ餷�ҡġʤ��ɤ���
			} else {
				if ( vline==CRTC_VEND ) MFP[MFP_TADR]--;
			}
		}
		if ( !MFP[MFP_TADR] ) {
			MFP[MFP_TADR] = Timer_Reload[0];
			MFP_Int(2);
		}
	}
}

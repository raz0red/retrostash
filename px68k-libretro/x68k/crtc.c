// ---------------------------------------------------------------------------------------
//  CRTC.C - CRT Controller / Video Controller
// ---------------------------------------------------------------------------------------

#include	"common.h"
#include	"windraw.h"
#include	"winx68k.h"
#include	"tvram.h"
#include	"gvram.h"
#include	"bg.h"
#include	"m68000.h"
#include	"crtc.h"


static WORD FastClearMask[16] = {
	0xffff, 0xfff0, 0xff0f, 0xff00, 0xf0ff, 0xf0f0, 0xf00f, 0xf000,
	0x0fff, 0x0ff0, 0x0f0f, 0x0f00, 0x00ff, 0x00f0, 0x000f, 0x0000
};

	uint8_t	CRTC_Regs[24*2];
	uint8_t	CRTC_Mode = 0;
	DWORD	TextDotX = 768, TextDotY = 512;
	WORD	CRTC_VSTART, CRTC_VEND;
	WORD	CRTC_HSTART, CRTC_HEND;
	DWORD	TextScrollX = 0, TextScrollY = 0;
	DWORD	GrphScrollX[4] = {0, 0, 0, 0};		// ����ˤ�����ä���
	DWORD	GrphScrollY[4] = {0, 0, 0, 0};

	uint8_t	CRTC_FastClr = 0;
	uint8_t	CRTC_SispScan = 0;
	DWORD	CRTC_FastClrLine = 0;
	WORD	CRTC_FastClrMask = 0;
	WORD	CRTC_IntLine = 0;
	uint8_t	CRTC_VStep = 2;

	uint8_t	VCReg0[2] = {0, 0};
	uint8_t	VCReg1[2] = {0, 0};
	uint8_t	VCReg2[2] = {0, 0};

	uint8_t	CRTC_RCFlag[2] = {0, 0};
	int HSYNC_CLK = 324;
	extern int VID_MODE, CHANGEAV_TIMING;


// -----------------------------------------------------------------------
//   �餹�������ԡ�
// -----------------------------------------------------------------------
void CRTC_RasterCopy(void)
{
	DWORD line = (((DWORD)CRTC_Regs[0x2d])<<2);
	DWORD src  = (((DWORD)CRTC_Regs[0x2c])<<9);
	DWORD dst  = (((DWORD)CRTC_Regs[0x2d])<<9);

{
	static const DWORD off[4] = { 0, 0x20000, 0x40000, 0x60000 };
	int i, bit;

	for (bit = 0; bit < 4; bit++) {
		if (CRTC_Regs[0x2b] & (1 << bit)) {
			memmove(&TVRAM[dst + off[bit]], &TVRAM[src + off[bit]],
			    sizeof(DWORD) * 128);
		}
	}

	line = (line - TextScrollY) & 0x3ff;
	for (i = 0; i < 4; i++) {
		TextDirtyLine[line] = 1;
		line = (line + 1) & 0x3ff;
	}
}

	TVRAM_RCUpdate();
}


// -----------------------------------------------------------------------
//   �ӤǤ�����Ȥ���줸����
// -----------------------------------------------------------------------
// Reg0�ο��⡼�ɤϡ��Ѥäȸ�CRTC��Ʊ�����������㤦�Τ���ա�
// CRTC��GVRAM�ؤΥ���������ˡ�ʥ���ޥå׾�Ǥθ������ˤ��Ѥ��Τ��Ф���
// VCtrl�ϡ�GVRAM�����̤�Ÿ����ˡ�����椹�롣
// �Ĥޤꡢ����������ˡ��CRTC�ˤ�16bit�⡼�ɤǡ�ɽ����256���⡼�ɤäƤʻȤ�
// ����������Τ줹��
// ���åȥ�ư����Ys�������ǡ�OP�ʤɤǻȤ��Ƥޤա�

uint8_t FASTCALL VCtrl_Read(DWORD adr)
{
	uint8_t ret = 0xff;
	switch(adr&0x701)
	{
	case 0x400:
	case 0x401:
		ret = VCReg0[adr&1];
		break;
	case 0x500:
	case 0x501:
		ret = VCReg1[adr&1];
		break;
	case 0x600:
	case 0x601:
		ret = VCReg2[adr&1];
		break;
	}
	return ret;
}


void FASTCALL VCtrl_Write(DWORD adr, uint8_t data)
{
	uint8_t old;
	switch(adr&0x701)
	{
	case 0x401:
		if (VCReg0[adr&1] != data)
		{
			VCReg0[adr&1] = data;
			TVRAM_SetAllDirty();
		}
		break;
	case 0x500:
	case 0x501:
		if (VCReg1[adr&1] != data)
		{
			VCReg1[adr&1] = data;
			TVRAM_SetAllDirty();
		}
		break;
	case 0x600:
	case 0x601:
		if (VCReg2[adr&1] != data)
		{
			old = VCReg2[adr&1];
			VCReg2[adr&1] = data;
			TVRAM_SetAllDirty();
		}
		break;
	}
}


// -----------------------------------------------------------------------
//   CRTC�줸����
// -----------------------------------------------------------------------
// �쥸�������������Υ����ɤ����� ^^;

void CRTC_Init(void)
{
	memset(CRTC_Regs, 0, 48);
	TextScrollX = 0, TextScrollY = 0;
	memset(GrphScrollX, 0, sizeof(GrphScrollX));
	memset(GrphScrollY, 0, sizeof(GrphScrollY));
}


uint8_t FASTCALL CRTC_Read(DWORD adr)
{
	uint8_t ret;
	if (adr<0xe803ff) {
		int reg = adr&0x3f;
		if ( (reg>=0x28)&&(reg<=0x2b) ) return CRTC_Regs[reg];
		else return 0;
	} else if ( adr==0xe80481 ) {
// FastClear���������
// FastClr�ӥåȤ�1��񤭹���ȡ����λ����Ǥ�ReadBack���Ƥ�1�ϸ����ʤ���
// 1�񤭹��߸�κǽ�ο�ľ�������֤�1��Ω�����õ�򳫻Ϥ��롣
// 1��ľƱ�����֤Ǿõ����ꡢ0�����ġĤ餷�ҡ�PITAPAT��
		if (CRTC_FastClr)
			ret = CRTC_Mode | 0x02;
		else
			ret = CRTC_Mode & 0xfd;

		return ret;
	}
	else
		return 0x00;
}


void FASTCALL CRTC_Write(DWORD adr, uint8_t data)
{
	uint8_t old;
	uint8_t reg = (uint8_t)(adr&0x3f);
	int old_vidmode = VID_MODE;
	if (adr<0xe80400)
	{
		if ( reg>=0x30 ) return;
		if (CRTC_Regs[reg]==data) return;
		old = CRTC_Regs[reg];
		CRTC_Regs[reg] = data;
		TVRAM_SetAllDirty();
		switch(reg)
		{
		case 0x04:
		case 0x05:
			CRTC_HSTART = (((WORD)CRTC_Regs[0x4]<<8)+CRTC_Regs[0x5]);
			TextDotX = (CRTC_HEND-CRTC_HSTART)*8;
			BG_HAdjust = ((long)BG_Regs[0x0d]-(CRTC_HSTART+4))*8;				// ��ʿ�����ϲ����٤ˤ��1/2�Ϥ���ʤ�����Tetris��
			WinDraw_ChangeSize();
			break;
		case 0x06:
		case 0x07:
			CRTC_HEND = (((WORD)CRTC_Regs[0x6]<<8)+CRTC_Regs[0x7]);
			TextDotX = (CRTC_HEND-CRTC_HSTART)*8;
			WinDraw_ChangeSize();
			break;
		case 0x08:
		case 0x09:
			VLINE_TOTAL = (((WORD)CRTC_Regs[8]<<8)+CRTC_Regs[9]);
			HSYNC_CLK = ((CRTC_Regs[0x29]&0x10)?VSYNC_HIGH:VSYNC_NORM)/VLINE_TOTAL;
			break;
		case 0x0c:
		case 0x0d:
			CRTC_VSTART = (((WORD)CRTC_Regs[0xc]<<8)+CRTC_Regs[0xd]);
			BG_VLINE = ((long)BG_Regs[0x0f]-CRTC_VSTART)/((BG_Regs[0x11]&4)?1:2);	// BG�Ȥ���¾������Ƥ���κ�ʬ
			TextDotY = CRTC_VEND-CRTC_VSTART;
			if ((CRTC_Regs[0x29]&0x14)==0x10)
			{
				TextDotY/=2;
				CRTC_VStep = 1;
			}
			else if ((CRTC_Regs[0x29]&0x14)==0x04)
			{
				TextDotY*=2;
				CRTC_VStep = 4;
			}
			else
				CRTC_VStep = 2;
			WinDraw_ChangeSize();
			break;
		case 0x0e:
		case 0x0f:
			CRTC_VEND = (((WORD)CRTC_Regs[0xe]<<8)+CRTC_Regs[0xf]);
			TextDotY = CRTC_VEND-CRTC_VSTART;
			if ((CRTC_Regs[0x29]&0x14)==0x10)
			{
				TextDotY/=2;
				CRTC_VStep = 1;
			}
			else if ((CRTC_Regs[0x29]&0x14)==0x04)
			{
				TextDotY*=2;
				CRTC_VStep = 4;
			}
			else
				CRTC_VStep = 2;
			WinDraw_ChangeSize();
			break;
		case 0x28:
			TVRAM_SetAllDirty();
			break;
		case 0x29:
			HSYNC_CLK = ((CRTC_Regs[0x29]&0x10)?VSYNC_HIGH:VSYNC_NORM)/VLINE_TOTAL;
			VID_MODE = !!(CRTC_Regs[0x29]&0x10);
			TextDotY = CRTC_VEND-CRTC_VSTART;
			if ((CRTC_Regs[0x29]&0x14)==0x10)
			{
				TextDotY/=2;
				CRTC_VStep = 1;
			}
			else if ((CRTC_Regs[0x29]&0x14)==0x04)
			{
				TextDotY*=2;
				CRTC_VStep = 4;
			}
			else
				CRTC_VStep = 2;
			if (VID_MODE != old_vidmode)
			{
				old_vidmode = VID_MODE;
				CHANGEAV_TIMING=1;
			}
			WinDraw_ChangeSize();
			break;
		case 0x12:
		case 0x13:
			CRTC_IntLine = (((WORD)CRTC_Regs[0x12]<<8)+CRTC_Regs[0x13])&1023;
			break;
		case 0x14:
		case 0x15:
			TextScrollX = (((DWORD)CRTC_Regs[0x14]<<8)+CRTC_Regs[0x15])&1023;
			break;
		case 0x16:
		case 0x17:
			TextScrollY = (((DWORD)CRTC_Regs[0x16]<<8)+CRTC_Regs[0x17])&1023;
			break;
		case 0x18:
		case 0x19:
			GrphScrollX[0] = (((DWORD)CRTC_Regs[0x18]<<8)+CRTC_Regs[0x19])&1023;
			break;
		case 0x1a:
		case 0x1b:
			GrphScrollY[0] = (((DWORD)CRTC_Regs[0x1a]<<8)+CRTC_Regs[0x1b])&1023;
			break;
		case 0x1c:
		case 0x1d:
			GrphScrollX[1] = (((DWORD)CRTC_Regs[0x1c]<<8)+CRTC_Regs[0x1d])&511;
			break;
		case 0x1e:
		case 0x1f:
			GrphScrollY[1] = (((DWORD)CRTC_Regs[0x1e]<<8)+CRTC_Regs[0x1f])&511;
			break;
		case 0x20:
		case 0x21:
			GrphScrollX[2] = (((DWORD)CRTC_Regs[0x20]<<8)+CRTC_Regs[0x21])&511;
			break;
		case 0x22:
		case 0x23:
			GrphScrollY[2] = (((DWORD)CRTC_Regs[0x22]<<8)+CRTC_Regs[0x23])&511;
			break;
		case 0x24:
		case 0x25:
			GrphScrollX[3] = (((DWORD)CRTC_Regs[0x24]<<8)+CRTC_Regs[0x25])&511;
			break;
		case 0x26:
		case 0x27:
			GrphScrollY[3] = (((DWORD)CRTC_Regs[0x26]<<8)+CRTC_Regs[0x27])&511;
			break;
		case 0x2a:
		case 0x2b:
			break;
		case 0x2c:				// CRTCư��ݡ��ȤΥ饹�����ԡ���ON�ˤ��Ƥ����ơʤ��Ƥ������ޤޡˡ�
		case 0x2d:				// Src/Dst���������Ѥ��Ƥ����Τ�������餷���ʥɥ饭���Ȥ���
			CRTC_RCFlag[reg-0x2c] = 1;	// Dst�ѹ���˼¹Ԥ���롩
			if ((CRTC_Mode&8)&&/*(CRTC_RCFlag[0])&&*/(CRTC_RCFlag[1]))
			{
				CRTC_RasterCopy();
				CRTC_RCFlag[0] = 0;
				CRTC_RCFlag[1] = 0;
			}
			break;
		}
	}
	else if (adr==0xe80481)
	{					// CRTCư��ݡ���
		CRTC_Mode = (data|(CRTC_Mode&2));
		if (CRTC_Mode&8)
		{				// Raster Copy
			CRTC_RasterCopy();
			CRTC_RCFlag[0] = 0;
			CRTC_RCFlag[1] = 0;
		}
		if (CRTC_Mode&2)		// ��®���ꥢ
		{
			CRTC_FastClrLine = vline;
						// ���λ����Υޥ�����ͭ���餷���ʥ���������
			CRTC_FastClrMask = FastClearMask[CRTC_Regs[0x2b]&15];
		}
	}
}

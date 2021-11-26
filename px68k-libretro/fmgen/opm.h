// ---------------------------------------------------------------------------
//	OPM-like Sound Generator
//	Copyright (C) cisc 1998, 2003.
// ---------------------------------------------------------------------------

#ifndef FM_OPM_H
#define FM_OPM_H

#include <stdint.h>

#include "fmgen.h"
#include "fmtimer.h"
#include "psg.h"

// ---------------------------------------------------------------------------
//	class OPM
//	OPM ���ɤ�����(?)�����������벻����˥å�
//	
//	interface:
//	bool Init(uint32_t clock, uint32_t rate, bool);
//		����������Υ��饹����Ѥ������ˤ��ʤ餺�Ƥ�Ǥ������ȡ�
//		���: �����䴰�⡼�ɤ��ѻߤ���ޤ���
//
//		clock:	OPM �Υ���å����ȿ�(Hz)
//
//		rate:	�������� PCM ��ɸ�ܼ��ȿ�(Hz)
//
//				
//		����	���������������� true
//
//	bool SetRate(uint32_t clock, uint32_t rate, bool)
//		����å��� PCM �졼�Ȥ��ѹ�����
//		�������� Init ��Ʊ�͡�
//	
//	void Mix(Sample* dest, int nsamples)
//		Stereo PCM �ǡ����� nsamples ʬ�������� dest �ǻϤޤ������
//		�ä���(�û�����)
//		��dest �ˤ� sample*2 ��ʬ���ΰ褬ɬ��
//		����Ǽ������ L, R, L, R... �Ȥʤ롥
//		�������ޤǲû��ʤΤǡ����餫��������򥼥��ꥢ����ɬ�פ�����
//		��FM_SAMPLETYPE �� short ���ξ�祯��åԥ󥰤��Ԥ���.
//		�����δؿ��ϲ��������Υ����ޡ��Ȥ���Ω���Ƥ��롥
//		  Timer �� Count �� GetNextEvent ������ɬ�פ����롥
//	
//	void Reset()
//		������ꥻ�å�(�����)����
//
//	void SetReg(uint32_t reg, uint32_t data)
//		�����Υ쥸���� reg �� data ��񤭹���
//	
//	uint32_t ReadStatus()
//		�����Υ��ơ������쥸�������ɤ߽Ф�
//		busy �ե饰�Ͼ�� 0
//	
//	bool Count(uint32_t t)
//		�����Υ����ޡ��� t [10^(-6) ��] �ʤ�롥
//		�������������֤��Ѳ������ä���(timer �����С��ե�)
//		true ���֤�
//
//	uint32_t GetNextEvent()
//		�����Υ����ޡ��Τɤ��餫�������С��ե�����ޤǤ�ɬ�פ�
//		����[����]���֤�
//		�����ޡ�����ߤ��Ƥ������ 0 ���֤���
//	
//	void SetVolume(int db)
//		�Ʋ����β��̤�ܡ�������Ĵ�᤹�롥ɸ���ͤ� 0.
//		ñ�̤��� 1/2 dB��ͭ���ϰϤξ�¤� 20 (10dB)
//
//	���۴ؿ�:
//	virtual void Intr(bool irq)
//		IRQ ���Ϥ��Ѳ������ä����ƤФ�롥
//		irq = true:  IRQ �׵᤬ȯ��
//		irq = false: IRQ �׵᤬�ä���
//
namespace FM
{
	//	YM2151(OPM) ----------------------------------------------------
	class OPM : public Timer
	{
	public:
		OPM();
		virtual ~OPM() {}

		bool	Init(uint32_t c, uint32_t r, bool=false);
		bool	SetRate(uint32_t c, uint32_t r, bool);
		void	SetLPFCutoff(uint32_t freq);
		void	Reset();
		
		void 	SetReg(uint32_t addr, uint32_t data);
		uint32_t	GetReg(uint32_t addr);
		uint32_t	ReadStatus() { return status & 0x03; }
		
		void 	Mix(Sample* buffer, int nsamples, int rate, uint8_t* pbsp, uint8_t* pbep);
		
		void	SetVolume(int db);
		void	SetChannelMask(uint32_t mask);
		
	private:
		virtual void Intr(bool) {}
	
	private:
		enum
		{
			OPM_LFOENTS = 512,
		};
		
		void	SetStatus(uint32_t bit);
		void	ResetStatus(uint32_t bit);
		void	SetParameter(uint32_t addr, uint32_t data);
		void	TimerA();
		void	RebuildTimeTable();
		void	MixSub(int activech, ISample**);
		void	MixSubL(int activech, ISample**);
		void	LFO();
		uint32_t	Noise();
		
		int		fmvolume;

		uint32_t	clock;
		uint32_t	rate;
		uint32_t	pcmrate;

		uint32_t	pmd;
		uint32_t	amd;
		uint32_t	lfocount;
		uint32_t	lfodcount;

		uint32_t	lfo_count_;
		uint32_t	lfo_count_diff_;
		uint32_t	lfo_step_;
		uint32_t	lfo_count_prev_;

		uint32_t	lfowaveform;
		uint32_t	rateratio;
		uint32_t	noise;
		int32_t	noisecount;
		uint32_t	noisedelta;
		
		bool	interpolation;
		uint8_t	lfofreq;
		uint8_t	status;
		uint8_t	reg01;

		uint8_t	kc[8];
		uint8_t	kf[8];
		uint8_t	pan[8];

		Channel4 ch[8];
		Chip	chip;

		static void	BuildLFOTable();
		static int amtable[4][OPM_LFOENTS];
		static int pmtable[4][OPM_LFOENTS];

	public:
		int		dbgGetOpOut(int c, int s) { return ch[c].op[s].dbgopout_; }
		Channel4* dbgGetCh(int c) { return &ch[c]; }

	};
}

#endif // FM_OPM_H

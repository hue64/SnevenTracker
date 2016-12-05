/*
** FamiTracker - NES/Famicom sound tracker
** Copyright (C) 2005-2014  Jonathan Liss
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
** Library General Public License for more details.  To obtain a 
** copy of the GNU Library General Public License, write to the Free 
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/

#ifndef APU_H
#define APU_H

//#define LOGGING

#include "../Common.h"
#include "Mixer.h"
#include "../VGM/Constants.h"		// // //

// External classes
class CSN76489;		// // //

class CVGMWriterBase;		// // //

#ifdef LOGGING
class CFile;
#endif

class CAPU {
public:
	CAPU(IAudioCallback *pCallback);		// // //
	~CAPU();

	void	Reset();
	void	Process();
	void	AddTime(int32 Cycles);

	// // //
	void	Write(uint16 Address, uint8 Value);

	void	SetExternalSound(uint8 Chip);
	void	ExternalWrite(uint16 Address, uint8 Value);
	uint8	ExternalRead(uint16 Address);
	
	void	ChangeMachine(int Machine);
	bool	SetupSound(int SampleRate, int NrChannels, int Speed);
	void	SetupMixer(int LowCut, int HighCut, int HighDamp, int Volume) const;

	int32	GetVol(uint8 Chan) const;
	// // //
	uint8	GetReg(int Chip, int Reg) const;

	void	SetChipLevel(chip_level_t Chip, float Level) const;
	void	SetStereoSeparation(float Sep) const;		// // //

	void	SetVGMWriter(VGMChip Chip, const CVGMWriterBase *pWrite);		// // //

#ifdef LOGGING
	void	Log();
#endif

public:
	static const uint32	BASE_FREQ_NTSC;
	static const uint32	BASE_FREQ_PAL;
	static const uint8	FRAME_RATE_NTSC;
	static const uint8	FRAME_RATE_PAL;

private:
	inline void RunSN(uint32 Time);		// // //

	void EndFrame();

private:
	CMixer		*m_pMixer;
	IAudioCallback *m_pParent;

	// Internal channels
	CSN76489	*m_pSN76489;		// // //

	// // //

	uint8		m_iExternalSoundChip;				// External sound chip, if used

	uint32		m_iFramePeriod;						// Cycles per frame
	uint32		m_iFrameCycles;						// Cycles emulated from start of frame
	// // //
	uint8		m_iFrameSequence;					// Frame sequence
	// // //

	uint32		m_iFrameCycleCount;
	uint32		m_iFrameClock;
	uint32		m_iCyclesToRun;						// Number of cycles to process

	uint32		m_iSoundBufferSamples;				// Size of buffer, in samples
	bool		m_bStereoEnabled;					// If stereo is enabled

	uint32		m_iSampleSizeShift;					// To convert samples to bytes
	uint32		m_iSoundBufferSize;					// Size of buffer, in samples
	uint32		m_iBufferPointer;					// Fill pos in buffer
	int16		*m_pSoundBuffer;					// Sound transfer buffer

	uint8		m_iRegs[0x20];
	// // //

#ifdef LOGGING
	CFile		  *m_pLog;
	int			  m_iFrame;
//	unsigned char m_iRegs[32];
#endif

};

#endif /* APU_H */

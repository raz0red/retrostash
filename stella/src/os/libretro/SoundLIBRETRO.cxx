//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2023 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#ifdef SOUND_SUPPORT
#ifdef WRC
// WRC: New file - SoundLIBRETRO.cxx
//   Implements open() and dequeue() outside the header.
//   dequeue() synchronously drains AudioQueue, then linearly resamples
//   the TIA native rate (~31440 Hz) to 48000 Hz (800 stereo pairs/frame NTSC,
//   960 PAL).  The original code was inline in SoundLIBRETRO.hxx and passed
//   audio through with no resampling, causing audible popping on iOS.

#include <cmath>
#include "SoundLIBRETRO.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SoundLIBRETRO::open(shared_ptr<AudioQueue> audioQueue,
                         EmulationTiming* emulationTiming)
{
  myEmulationTiming = emulationTiming;

  audioQueue->ignoreOverflows(!myAudioSettings.enabled());

  myAudioQueue = audioQueue;
  myCurrentFragment = nullptr;
  myInputPos = 0.0;

  myIsInitializedFlag = true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SoundLIBRETRO::dequeue(Int16* stream, uInt32* samples)
{
  // 800 stereo pairs/frame at 60 fps (NTSC), 960 at 50 fps (PAL)
  const uInt32 outSamples = myEmulationTiming->linesPerFrame() == 262 ? 800 : 960;

  const uInt32 fragSize    = myAudioQueue->fragmentSize();
  const bool   isStereo    = myAudioQueue->isStereo();

  // Drain all available fragments from the queue into myInputBuf.
  // We collect raw input samples (mono or stereo) synchronously,
  // then linearly interpolate to the required output count.
  uInt32 inputPairs = 0;

  while (myAudioQueue->size() > 0) {
    Int16* frag = myAudioQueue->dequeue(myCurrentFragment);
    if (!frag) break;
    myCurrentFragment = frag;

    for (uInt32 i = 0; i < fragSize && inputPairs < INPUT_BUF_MAX; ++i) {
      if (isStereo) {
        myInputBuf[inputPairs * 2]     = myCurrentFragment[i * 2];
        myInputBuf[inputPairs * 2 + 1] = myCurrentFragment[i * 2 + 1];
      } else {
        myInputBuf[inputPairs * 2]     = myCurrentFragment[i];
        myInputBuf[inputPairs * 2 + 1] = myCurrentFragment[i];
      }
      ++inputPairs;
    }
  }

  if (inputPairs == 0) {
    // Queue empty (e.g. right after autodetectFrameLayout recreates the queue).
    // Output silence; audio resumes on the next frame.
    std::fill_n(stream, outSamples * 2, Int16(0));
    *samples = outSamples;
    return;
  }

  // Linear-interpolate inputPairs stereo pairs -> outSamples stereo pairs.
  // myInputPos tracks fractional position across frames so we don't drift.
  const float step = static_cast<float>(inputPairs) / static_cast<float>(outSamples);

  for (uInt32 i = 0; i < outSamples; ++i) {
    const uInt32 idx  = static_cast<uInt32>(myInputPos);
    const float  frac = myInputPos - static_cast<float>(idx);

    if (idx + 1 < inputPairs) {
      stream[i * 2]     = static_cast<Int16>(myInputBuf[idx * 2]     * (1.f - frac) + myInputBuf[(idx + 1) * 2]     * frac);
      stream[i * 2 + 1] = static_cast<Int16>(myInputBuf[idx * 2 + 1] * (1.f - frac) + myInputBuf[(idx + 1) * 2 + 1] * frac);
    } else {
      stream[i * 2]     = myInputBuf[idx * 2];
      stream[i * 2 + 1] = myInputBuf[idx * 2 + 1];
    }

    myInputPos += step;
  }

  // Reset position for next frame (we consumed all collected samples).
  myInputPos = 0.0;

  *samples = outSamples;
}

#endif // WRC
#endif  // SOUND_SUPPORT

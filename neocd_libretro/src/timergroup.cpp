#include <algorithm>
#include <limits>

#include "3rdparty/ym/ym2610.h"
#include "3rdparty/z80/z80.h"
#include "libretro_common.h"
#include "libretro_log.h"
#include "neogeocd.h"
#include "round.h"
#include "timergroup.h"

extern "C"
{
    #include "3rdparty/musashi/m68kcpu.h"
}

void watchdogTimerCallback(Timer* timer, uint32_t userData)
{
    Libretro::Log::message(RETRO_LOG_ERROR, "WARNING: Watchdog timer triggered (PC=%06X, SR=%04X); Machine reset.\n",
        m68k_get_reg(NULL, M68K_REG_PPC),
        m68k_get_reg(NULL, M68K_REG_SR));

    m68k_pulse_reset();
}

void vblIrqTimerCallback(Timer* timer, uint32_t userData)
{
    // Set VBL IRQ to run
    if (neocd->isVBLEnabled())
    {
        neocd->setInterrupt(NeoGeoCD::VerticalBlank);
        neocd->updateInterrupts();
    }

    // Update auto animation counter
    if (!neocd->video.autoAnimationFrameCounter)
    {
        neocd->video.autoAnimationFrameCounter = neocd->video.autoAnimationSpeed;
        neocd->video.autoAnimationCounter++;
    }
    else
        neocd->video.autoAnimationFrameCounter--;

    timer->armRelative(Timer::pixelToMaster(Timer::SCREEN_WIDTH * Timer::SCREEN_HEIGHT));
}

void hirqTimerCallback(Timer* timer, uint32_t userData)
{
    // Trigger horizontal interrupt if enabled
    if ((neocd->video.hirqControl & Video::HIRQ_CTRL_ENABLE)
        && neocd->isHBLEnabled())
    {
/*		Libretro::Log::message(RETRO_LOG_DEBUG, "Horizontal IRQ.@ (%d,%d), next drawline in : %d\n",
            neocd->getScreenX(),
            neocd->getScreenY(),
            Timer::masterToPixel(neocd->timers.drawlineTimer->delay()));*/

        neocd->setInterrupt(NeoGeoCD::Raster);
        neocd->updateInterrupts();
    }

    // Neo Drift Out sets HIRQ_reg = 0xFFFFFFFF and auto-repeat, we need to check for it
    // I don't know the exact reason for the need to add 1 to hirqRegister + 1 here and several other parts of the code,
    // but it's important for line effects in Karnov's Revenge to work correctly.
    if ((neocd->video.hirqControl & Video::HIRQ_CTRL_AUTOREPEAT) && (neocd->video.hirqRegister != 0xFFFFFFFF))
    {
        constexpr uint32_t MAX_PIXELS = Timer::masterToPixel(std::numeric_limits<int32_t>::max() - 4);
        timer->armRelative(Timer::pixelToMaster(std::max(uint32_t(1), std::min(MAX_PIXELS, neocd->video.hirqRegister + 1))));
    }
}

void vblReloadTimerCallback(Timer* timer, uint32_t userData)
{
    // Handle HIRQ_CTRL_VBLANK_LOAD
    if (neocd->video.hirqControl & Video::HIRQ_CTRL_VBLANK_LOAD)
        neocd->timers.timer<TimerGroup::Hbl>().arm(timer->delay() + Timer::pixelToMaster(neocd->video.hirqRegister + 1));

    timer->armRelative(Timer::pixelToMaster(Timer::SCREEN_WIDTH * Timer::SCREEN_HEIGHT));
}

void ym2610TimerCallback(Timer* Timer, uint32_t data)
{
    YM2610TimerOver(data);
}

void drawlineTimerCallback(Timer* timer, uint32_t userData)
{
//	Libretro::Log::message(RETRO_LOG_DEBUG, "Drawline.@ (%d,%d)\n", neocd->getScreenX(), neocd->getScreenY());

    const uint32_t scanline = neocd->getScreenY();

    // Video content is generated between scanlines 16 and 240, see timer.h
    if ((scanline >= Timer::ACTIVE_AREA_TOP) && (scanline < Timer::ACTIVE_AREA_BOTTOM))
    {
        if (!neocd->fastForward)
        {
            if (neocd->video.videoEnable)
            {
                neocd->video.drawEmptyLine(scanline);

                if (!neocd->video.sprDisable)
                {
                    const size_t address = (scanline & 1) ? 0x8680 : 0x8600;
                    uint16_t activeSprites = neocd->video.createSpriteList(scanline, &neocd->memory.videoRam[address]);
                    neocd->video.drawSprites(scanline, &neocd->memory.videoRam[address], activeSprites);
                }

                if (!neocd->video.fixDisable)
                    neocd->video.drawFix(scanline);
            }
            else
                neocd->video.drawBlackLine(scanline);
        }
    }

    timer->armRelative(Timer::pixelToMaster(Timer::SCREEN_WIDTH));
}

void cdromTimerCallback(Timer* timer, uint32_t userData)
{
    // Shortcuts
    const bool isCDZ = neocd->isCDZ();
    const bool isPlaying = neocd->cdrom.isPlaying();
    const bool isData = neocd->cdrom.isData();
    const bool isDecoderEnabled = neocd->lc8951.isCdDecoderEnabled();
    const bool isDecoderIRQEnabled = neocd->lc8951.isCdDecoderIRQEnabled();
    const bool isDecoderIRQPending = neocd->lc8951.isCdDecoderIRQPending();

    int32_t delay;

    if (isPlaying)
    {
        if (isData)
            delay = isCDZ ? (Timer::CDROM_150HZ_DELAY) : Timer::CDROM_75HZ_DELAY;
        else
            delay = Timer::CDROM_75HZ_DELAY;
    }
    else
        delay = Timer::CDROM_64HZ_DELAY;

    timer->armRelative(delay);

    if (isPlaying)
    {
        neocd->lc8951.sectorDecoded();

        // Trigger the CD IRQ1 if needed, this is used when reading data sectors
        if (isDecoderEnabled // CD Decoder is enabled
            && neocd->isCdDecoderIRQEnabled() // CD Decoder IRQ is not masked
            && isDecoderIRQEnabled // CD Decoder IRQ is enabled
            && isDecoderIRQPending) // CD Decoder IRQ is pending
        {
            // This is used to detect CD activity in the main loop
            neocd->cdSectorDecodedThisFrame = true;

            // Trigger the CD Decoder IRQ
            neocd->setInterrupt(NeoGeoCD::CdromDecoder);
        }

        // Update the read position
        neocd->cdrom.increasePosition();
    }

    // Trigger the CDROM IRQ2, this is used to send commands packets / receive answer packets.
    if (neocd->isCdCommunicationIRQEnabled())
        neocd->setInterrupt(NeoGeoCD::CdromCommunication);

    // Update interrupts
    neocd->updateInterrupts();
}

void audioCommandTimerCallback(Timer* timer, uint32_t userData)
{
    // Post the audio command to the Z80
    neocd->audioCommand = userData;

    // If the NMI is not disabled
    if (!neocd->z80NMIDisable)
    {
        // Trigger it
        z80_set_irq_line(INPUT_LINE_NMI, ASSERT_LINE);
        z80_set_irq_line(INPUT_LINE_NMI, CLEAR_LINE);
    }
}

TimerGroup::TimerGroup() :
    m_timers()
{
    timer<TimerGroup::Watchdog>().setDelay(Timer::WATCHDOG_DELAY);
    timer<TimerGroup::Watchdog>().setCallback(watchdogTimerCallback);

    timer<TimerGroup::Drawline>().setCallback(drawlineTimerCallback);

    timer<TimerGroup::Vbl>().setCallback(vblIrqTimerCallback);

    timer<TimerGroup::Hbl>().setCallback(hirqTimerCallback);

    timer<TimerGroup::VblReload>().setCallback(vblReloadTimerCallback);

    timer<TimerGroup::Cdrom>().setCallback(cdromTimerCallback);

    timer<TimerGroup::AudioCommand>().setCallback(audioCommandTimerCallback);

    timer<TimerGroup::Ym2610A>().setUserData(0);
    timer<TimerGroup::Ym2610A>().setCallback(ym2610TimerCallback);

    timer<TimerGroup::Ym2610B>().setUserData(1);
    timer<TimerGroup::Ym2610B>().setCallback(ym2610TimerCallback);
}

void TimerGroup::reset()
{
    timer<TimerGroup::Watchdog>().setState(Timer::Stopped);

#ifndef ADJUST_FRAME_BOUNDARY
    timer<TimerGroup::Drawline>().arm(Timer::pixelToMaster(Timer::ACTIVE_AREA_LEFT));

    timer<TimerGroup::Vbl>().arm(Timer::pixelToMaster((Timer::VBL_IRQ_Y * Timer::SCREEN_WIDTH) + Timer::VBL_IRQ_X));

    timer<TimerGroup::VblReload>().arm(Timer::pixelToMaster((Timer::VBL_RELOAD_Y * Timer::SCREEN_WIDTH) + Timer::VBL_RELOAD_X));
#else
    timer<TimerGroup::Drawline>().arm(Timer::pixelToMaster(Timer::ACTIVE_AREA_LEFT - Timer::VBL_IRQ_X));

    timer<TimerGroup::Vbl>().arm(Timer::pixelToMaster(Timer::SCREEN_WIDTH * Timer::SCREEN_HEIGHT));

    timer<TimerGroup::VblReload>().arm(Timer::pixelToMaster((Timer::VBL_RELOAD_Y - Timer::VBL_IRQ_Y) * Timer::SCREEN_WIDTH + (Timer::VBL_RELOAD_X - Timer::VBL_IRQ_X)));
#endif

    timer<TimerGroup::Hbl>().setState(Timer::Stopped);

    timer<TimerGroup::Cdrom>().arm(Timer::CDROM_64HZ_DELAY);

    timer<TimerGroup::AudioCommand>().setState(Timer::Stopped);

    timer<TimerGroup::Ym2610A>().setState(Timer::Stopped);

    timer<TimerGroup::Ym2610B>().setState(Timer::Stopped);
}

int32_t TimerGroup::timeSlice() const
{
    int32_t timeSlice = round<int32_t>(Timer::MASTER_CLOCK / Timer::FRAME_RATE);

    for(const Timer& timer : m_timers)
    {
        if (timer.isActive())
            timeSlice = std::min(timeSlice, timer.delay());
    }

    return timeSlice;
}

void TimerGroup::advanceTime(const int32_t time)
{
    for(Timer& timer : m_timers)
        timer.advanceTime(time);
}

DataPacker& operator<<(DataPacker& out, const TimerGroup& timerGroup)
{
    for(const Timer& timer : timerGroup.m_timers)
        out << timer;

    return out;
}

DataPacker& operator>>(DataPacker& in, TimerGroup& timerGroup)
{
    for(Timer& timer : timerGroup.m_timers)
        in >> timer;

    return in;
}

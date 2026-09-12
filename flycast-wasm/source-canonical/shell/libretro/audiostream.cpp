/*
    This file is part of Flycast.

    Flycast is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Flycast is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Flycast.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "types.h"
#include "cfg/option.h"
#include "audio/audiostream.h"
#include "emulator.h"

#include <libretro.h>

#include <vector>
#include <mutex>

/* Detect output refresh rate changes by monitoring
 * the last 'VSYNC_SWAP_INTERVAL_FRAMES' frames:
 * - Measure average (mean) audio samples per upload
 *   operation
 * - Determine vsync swap interval based on
 *   expected samples at 60 (or 50) Hz
 * - Check that vsync swap interval remains
 *   'stable' for at least 'VSYNC_SWAP_INTERVAL_FRAMES' */
#define VSYNC_SWAP_INTERVAL_FRAMES 6
/* Calculated swap interval is 'valid' if it is
 * within 'VSYNC_SWAP_INTERVAL_THRESHOLD' of an integer
 * value */
#define VSYNC_SWAP_INTERVAL_THRESHOLD 0.05f

extern bool setAVInfo(retro_system_av_info& avinfo);

extern retro_environment_t        environ_cb;
extern retro_audio_sample_batch_t audio_batch_cb;

extern float libretro_expected_audio_samples_per_run;
extern unsigned libretro_vsync_swap_interval;
extern bool libretro_detect_vsync_swap_interval;

// WRC (2026-09-11): flip to 0 to go back to the original fixed-size
// (~200ms) audio_buffer with overflow-drop-and-mute, for comparison or
// regression testing. See retro_audio_init()'s comment (in the #if 1
// branch below) for why the unbounded version replaced it as the
// default - short version: the fixed cap existed to avoid hanging
// retro_run() via a blocking audio_batch_cb() call, which never applied
// to our __EMSCRIPTEN__ path (calls straight into JS, non-blocking)
// but was still silently dropping+muting audio on any main-thread
// stall over ~200ms (JIT-compile spikes, scene transitions).
#define WRC_AUDIO_UNBOUNDED_BUFFER 1

static float audio_samples_per_frame_avg;
static unsigned vsync_swap_interval_last;
static unsigned vsync_swap_interval_conter;

static std::mutex audio_buffer_mutex;
static std::vector<int16_t> audio_buffer;
#if WRC_AUDIO_UNBOUNDED_BUFFER
static std::vector<int16_t> audio_out_buffer;
#else
static size_t audio_buffer_idx;
static bool drop_samples = true;
static int16_t *audio_out_buffer = nullptr;
#endif
static size_t audio_batch_frames_max;

#if WRC_AUDIO_UNBOUNDED_BUFFER
void retro_audio_init(void)
{
	const std::lock_guard<std::mutex> lock(audio_buffer_mutex);

	/* WRC (2026-09-11): audio_buffer used to be a FIXED-size vector
	 * (10 frames' worth, ~200ms at 44100Hz - see the #else branch below
	 * for the original comment), with WriteSample() dropping every
	 * buffered sample and muting until the next retro_audio_upload() the
	 * moment it filled. That cap existed to avoid hanging retro_run()
	 * via a blocking audio_batch_cb() call to a native frontend if too
	 * much got queued in one go. On the __EMSCRIPTEN__ path below,
	 * retro_audio_upload() bypasses audio_batch_cb entirely and calls
	 * straight into JS instead (non-blocking) - the failure mode the
	 * cap defended against doesn't apply here, but the buffer was still
	 * paying its cost: any main-thread stall long enough to accumulate
	 * more than ~200ms of audio (a heavy JIT-compile spike, a scene
	 * transition, several such frames back to back) would silently drop
	 * everything and mute - a real, confirmed source of audible pops.
	 * audio_buffer now just grows via push_back() as needed - no cap,
	 * no overflow-drop, no mute. Tradeoff: if something ever stopped
	 * draining it for a very long time, it would grow unbounded rather
	 * than dropping - accepted deliberately, not an oversight. */
	audio_buffer.clear();
	audio_out_buffer.clear();
	audio_batch_frames_max = std::numeric_limits<size_t>::max();

	audio_samples_per_frame_avg = 0.0f;
	vsync_swap_interval_last = 1;
	vsync_swap_interval_conter = 0;
}
#else
void retro_audio_init(void)
{
	const std::lock_guard<std::mutex> lock(audio_buffer_mutex);

	/* Worst case is 25 fps content with an audio sample rate
	 * of 44.1 kHz -> 1764 stereo samples
	 * But flycast can stop rendering for arbitrary lengths of
	 * time, leading to multiple 'frames' worth of audio being
	 * uploaded in retro_run(). We therefore require some leniency,
	 * but must limit the total number of samples that can be
	 * uploaded since the libretro frontend can 'hang' if too
	 * many samples are sent during a single call of retro_run().
	 * We therefore (arbitrarily) choose to allow up to 10 frames
	 * worth of 'worst case' stereo samples... */
	size_t audio_buffer_size = (44100 / 25) * 2 * 10;

	audio_buffer.resize(audio_buffer_size);
	audio_buffer_idx = 0;
	audio_batch_frames_max = std::numeric_limits<size_t>::max();

	audio_out_buffer = (int16_t*)malloc(audio_buffer_size * sizeof(int16_t));

	drop_samples = false;

	audio_samples_per_frame_avg = 0.0f;
	vsync_swap_interval_last = 1;
	vsync_swap_interval_conter = 0;
}
#endif

#if WRC_AUDIO_UNBOUNDED_BUFFER
void retro_audio_deinit(void)
{
	const std::lock_guard<std::mutex> lock(audio_buffer_mutex);

	audio_buffer.clear();
	audio_buffer.shrink_to_fit();
	audio_out_buffer.clear();
	audio_out_buffer.shrink_to_fit();

	audio_samples_per_frame_avg = 0.0f;
	vsync_swap_interval_last = 1;
	vsync_swap_interval_conter = 0;
}
#else
void retro_audio_deinit(void)
{
	const std::lock_guard<std::mutex> lock(audio_buffer_mutex);

	audio_buffer.clear();
	audio_buffer_idx = 0;

	if (audio_out_buffer != nullptr)
		free(audio_out_buffer);

	audio_out_buffer = nullptr;

	drop_samples = true;

	audio_samples_per_frame_avg = 0.0f;
	vsync_swap_interval_last = 1;
	vsync_swap_interval_conter = 0;
}
#endif

#if WRC_AUDIO_UNBOUNDED_BUFFER
void retro_audio_flush_buffer(void)
{
	const std::lock_guard<std::mutex> lock(audio_buffer_mutex);
	audio_buffer.clear();
}
#else
void retro_audio_flush_buffer(void)
{
	const std::lock_guard<std::mutex> lock(audio_buffer_mutex);
	audio_buffer_idx = 0;

	/* We are manually 'resetting' the audio buffer
	 * -> any 'drop samples' lock can be released */
	drop_samples = false;
}
#endif

size_t retro_audio_buffer_fill(void)
{
	/* Returns current buffer fill in stereo frames (sample pairs).
	 * Used by the frame pacer to sync emulation speed to audio
	 * consumption rate. Lock-free read — approximate value is fine. */
#if WRC_AUDIO_UNBOUNDED_BUFFER
	return audio_buffer.size() >> 1;
#else
	return audio_buffer_idx >> 1;
#endif
}

/* Audio-loss telemetry (2026-07-17): originally tracked two silent
 * sample-loss points. WriteSample overflow (dropped the whole buffer +
 * muted until next upload) is gone now that audio_buffer is unbounded
 * (2026-09-11) - g_aud_overflow_events stays declared (the HUD in
 * libretro.cpp still reads it) but can no longer increment, which is
 * correct: zero overflow events is now always true. audio_batch_cb
 * shortfalls (unwritten tail discarded) is a real, separate, native-
 * platform-only concern (audio_batch_cb itself reporting it wrote fewer
 * frames than asked) - unrelated to the buffer-size issue, still live
 * for native builds. */
u32 g_aud_overflow_events = 0;   /* no longer incrementable - kept for the HUD reader */
u32 g_aud_shortfall_frames = 0;  /* frames the frontend refused (discarded) */
u32 g_aud_produced_frames = 0;   /* frames handed to audio_batch_cb */

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
/* ★ AudioWorklet ring-buffer sink (2026-07-17). Replaces the RetroArch
 * OpenAL path on WASM. Why: the OpenAL→WebAudio shim schedules each ~6ms
 * chunk as its own AudioBufferSourceNode and clamps late buffers to
 * currentTime — a click-length seam on EVERY late main-thread frame — and
 * RetroArch's write side SPIN-WAITS the main thread when the queue is full
 * (a built-in per-frame stall). The worklet consumes from a ring buffer on
 * the AUDIO thread: gapless under main-thread jank, non-blocking writes,
 * underruns become counted silence instead of clicks. Stats (queue depth,
 * underrun frames, dropped frames) surface in the HUD via Module._flyAud. */
EM_JS(int, fly_worklet_push, (const short* ptr, unsigned frames), {
	var A = Module._flyAud;
	if (A && A.failed)
		return 1;   // worklet unavailable — caller falls back to audio_batch_cb
	if (!A) {
		A = Module._flyAud = { ready: false, failed: false, pend: [], drop: 0, lastVol: -1, stats: { avail: 0, under: 0, drop: 0 } };
		var ctx = new (window.AudioContext || window.webkitAudioContext)({ sampleRate: 44100 });
		A.ctx = ctx;
		var src = ""
			+ "class FlyAudio extends AudioWorkletProcessor {"
			+ "  constructor() { super();"
			+ "    this.cap = 32768;"
			+ "    this.rbL = new Float32Array(this.cap);"
			+ "    this.rbR = new Float32Array(this.cap);"
			+ "    this.r = 0; this.w = 0; this.avail = 0;"
			+ "    this.under = 0; this.drop = 0; this.tick = 0;"
			+ "    this.primed = false;"
			+ "    this.port.onmessage = (e) => {"
			+ "      var d = e.data; var n = d.length >> 1;"
			+ "      if (this.avail + n > this.cap) { this.drop += n; return; }"
			+ "      for (var i = 0; i < n; i++) {"
			+ "        this.rbL[this.w] = d[2*i] / 32768;"
			+ "        this.rbR[this.w] = d[2*i+1] / 32768;"
			+ "        this.w = (this.w + 1) % this.cap;"
			+ "      }"
			+ "      this.avail += n;"
			+ "    };"
			+ "  }"
			+ "  process(inputs, outputs) {"
			+ "    var out = outputs[0]; var L = out[0]; var R = out[1] || out[0];"
			+ "    if (!this.primed) {"
			+ "      if (this.avail >= 2048) { this.primed = true; }"
			+ "      else { for (var j = 0; j < L.length; j++) { L[j] = 0; R[j] = 0; } return true; }"
			+ "    }"
			+ "    for (var i = 0; i < L.length; i++) {"
			+ "      if (this.avail > 0) {"
			+ "        L[i] = this.rbL[this.r]; R[i] = this.rbR[this.r];"
			+ "        this.r = (this.r + 1) % this.cap; this.avail--;"
			+ "      } else { L[i] = 0; R[i] = 0; this.under++; this.primed = false; }"
			+ "    }"
			+ "    if (++this.tick >= 16) {"
			+ "      this.tick = 0;"
			+ "      this.port.postMessage({ avail: this.avail, under: this.under, drop: this.drop });"
			+ "    }"
			+ "    return true;"
			+ "  }"
			+ "}"
			+ "registerProcessor('fly-audio', FlyAudio);";
		var url = URL.createObjectURL(new Blob([src], { type: "application/javascript" }));
		ctx.audioWorklet.addModule(url).then(function() {
			var node = new AudioWorkletNode(ctx, "fly-audio", { outputChannelCount: [2] });
			// Volume: route through a GainNode driven by the EmulatorJS
			// volume/mute state (players expect the slider to work).
			A.gain = ctx.createGain();
			node.connect(A.gain);
			A.gain.connect(ctx.destination);
			node.port.onmessage = function(e) { A.stats = e.data; };
			A.node = node;
			A.ready = true;
			for (var i = 0; i < A.pend.length; i++)
				node.port.postMessage(A.pend[i], [A.pend[i].buffer]);
			A.pend = [];
			console.log("[fly-audio] AudioWorklet sink active @" + ctx.sampleRate + "Hz");
		}, function(e) {
			console.error("[fly-audio] worklet init failed, falling back to audio_batch_cb: " + (e && e.message));
			A.failed = true;
		});
	}
	if (A.ctx.state !== "running")
		A.ctx.resume();
	// Track EmulatorJS volume/mute (guarded — property names may evolve)
	if (A.ready) {
		var vol = 1;
		try {
			var E = window.EJS_emulator;
			if (E) {
				if (E.muted) vol = 0;
				else if (typeof E.volume === "number") vol = E.volume;
			}
		} catch (e) {}
		if (A.lastVol !== vol) {
			A.gain.gain.value = vol;
			A.lastVol = vol;
		}
	}
	var n = frames * 2;
	var chunk = new Int16Array(n);
	chunk.set(Module.HEAP16.subarray(ptr >> 1, (ptr >> 1) + n));
	// Backpressure (stats are ~45ms stale — leave slack): past ~0.56s queued,
	// drop instead of growing latency. Non-blocking by construction.
	if (A.stats.avail > 24576) { A.drop += frames; return; }
	if (A.ready)
		A.node.port.postMessage(chunk, [chunk.buffer]);
	else if (A.pend.length < 64)
		A.pend.push(chunk);
	return 0;
});
#endif

size_t retro_audio_buffer_capacity(void)
{
	/* No longer a fixed ceiling - audio_buffer grows as needed. Reports
	 * its current allocated capacity (not element count) as an
	 * approximation. Not called from anywhere currently, kept only
	 * because libretro.cpp still externs it. */
	return audio_buffer.capacity() >> 1;
}

void retro_audio_upload(void)
{
#if WRC_AUDIO_UNBOUNDED_BUFFER
	/* WRC (2026-09-11): swap rather than copy - O(1) pointer/size swap
	 * instead of a per-element copy loop into a separate fixed buffer,
	 * and it's how audio_buffer ends up genuinely unbounded: whatever
	 * WriteSample() accumulated (however large) becomes audio_out_buffer
	 * directly, and audio_buffer takes on audio_out_buffer's old
	 * (already-allocated, about-to-be-cleared) storage to accumulate
	 * into next. Minimizes time the mutex is held, too. */
	audio_buffer_mutex.lock();
	audio_out_buffer.swap(audio_buffer);
	audio_buffer_mutex.unlock();

	size_t num_frames = audio_out_buffer.size() >> 1;
#else
	audio_buffer_mutex.lock();

	for (size_t i = 0; i < audio_buffer_idx; i++)
		audio_out_buffer[i] = audio_buffer[i];

	size_t num_frames = audio_buffer_idx >> 1;
	audio_buffer_idx = 0;

	/* Uploading audio 'resets' the audio buffer
	 * -> any 'drop samples' lock can be released */
	drop_samples = false;

	audio_buffer_mutex.unlock();
#endif

	/* Attempt to detect changes in output refresh rate */
	if (libretro_detect_vsync_swap_interval &&
	    (num_frames > 0))
	{
		/* Simple running average (leaky-integrator) */
		audio_samples_per_frame_avg = ((1.0f / (float)VSYNC_SWAP_INTERVAL_FRAMES) * (float)num_frames) +
				((1.0f - (1.0f / (float)VSYNC_SWAP_INTERVAL_FRAMES)) * audio_samples_per_frame_avg);

		float swap_ratio = audio_samples_per_frame_avg /
				libretro_expected_audio_samples_per_run;
		unsigned swap_integer;
		float swap_remainder;

		/* If internal frame rate is equal to (within threshold)
		 * or higher than the default 60 (or 50) Hz, fall back
		 * to a swap interval of 1 */
		if (swap_ratio < (1.0f + VSYNC_SWAP_INTERVAL_THRESHOLD))
		{
			swap_integer = 1;
			swap_remainder = 0.0f;
		}
		else
		{
			swap_integer = (unsigned)(swap_ratio + 0.5f);
			swap_remainder = swap_ratio - (float)swap_integer;
			swap_remainder = (swap_remainder < 0.0f) ?
					-swap_remainder : swap_remainder;
		}

		/* > Swap interval is considered 'valid' if it is
		 *   within VSYNC_SWAP_INTERVAL_THRESHOLD of an integer
		 *   value
		 * > If valid, check if new swap interval differs from
		 *   previously logged value */
		if ((swap_remainder <= VSYNC_SWAP_INTERVAL_THRESHOLD) &&
			 (swap_integer != libretro_vsync_swap_interval))
		{
			vsync_swap_interval_conter =
					(swap_integer == vsync_swap_interval_last) ?
							(vsync_swap_interval_conter + 1) : 0;

			/* Check whether swap interval is 'stable' */
			if (vsync_swap_interval_conter >= VSYNC_SWAP_INTERVAL_FRAMES)
			{
				libretro_vsync_swap_interval = swap_integer;
				vsync_swap_interval_conter = 0;

				/* Notify frontend */
				retro_system_av_info avinfo;
				setAVInfo(avinfo);
				environ_cb(RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO, &avinfo);
			}

			vsync_swap_interval_last = swap_integer;
		}
		else
			vsync_swap_interval_conter = 0;
	}

#if WRC_AUDIO_UNBOUNDED_BUFFER
	int16_t *audio_out_buffer_ptr = audio_out_buffer.data();
#else
	int16_t *audio_out_buffer_ptr = audio_out_buffer;
#endif
	g_aud_produced_frames += (u32)num_frames;
#ifdef __EMSCRIPTEN__
	/* WRC (2026-09-08): switched from the AudioWorklet ring-buffer sink
	 * (fly_worklet_push(), still defined above but no longer called - left
	 * in place rather than deleted) to the same pattern used by every
	 * other WRC libretro core (PPSSPP, fceumm, snes9x, beetle-*, etc): call
	 * straight into window.emulator.audioCallback(), which feeds
	 * @webrcade/app-common's ScriptAudioProcessor circular queue. Real
	 * iOS testing (Crazy Taxi, heavy 3D scenes) showed audible dropped
	 * chunks with the worklet that this shared, well-tested path doesn't
	 * have elsewhere - despite AudioWorkletNode's playback running on a
	 * separate real-time thread in theory, so main-thread stalls
	 * shouldn't touch it as directly as they do here in practice.
	 * Re-tried once more 2026-09-10 (after the auto frame-skip feature
	 * landed, on the theory that less main-thread work per second might
	 * fix the worklet's dropped-chunk issue) - confirmed worse, reverted
	 * back to this. See upload_output_audio_buffer() in
	 * ppsspp-wasm/libretro/libretro.cpp for the reference this mirrors. */
	if (num_frames > 0) {
		EM_ASM({ window.emulator.audioCallback($0, $1); }, audio_out_buffer_ptr, num_frames);
	}
#if WRC_AUDIO_UNBOUNDED_BUFFER
	// audioCallback() above reads synchronously (copies out of the WASM
	// heap before returning), so it's safe to clear right after it.
	audio_out_buffer.clear();
#endif
	return;
#endif
	while (num_frames > 0)
	{
		size_t frames_to_write = (num_frames > audio_batch_frames_max) ?
				audio_batch_frames_max : num_frames;
		size_t frames_written = audio_batch_cb(audio_out_buffer_ptr,
				frames_to_write);

		if ((frames_written < frames_to_write) &&
			 (frames_written > 0))
		{
			audio_batch_frames_max = frames_written;
			g_aud_shortfall_frames += (u32)(frames_to_write - frames_written);
		}

		num_frames -= frames_to_write;
		audio_out_buffer_ptr += frames_to_write << 1;
	}
#if WRC_AUDIO_UNBOUNDED_BUFFER
	audio_out_buffer.clear();
#endif
}

#if WRC_AUDIO_UNBOUNDED_BUFFER
void WriteSample(s16 r, s16 l)
{
	const std::lock_guard<std::mutex> lock(audio_buffer_mutex);
	audio_buffer.push_back(l);
	audio_buffer.push_back(r);
}
#else
void WriteSample(s16 r, s16 l)
{
	const std::lock_guard<std::mutex> lock(audio_buffer_mutex);

	if (drop_samples)
		return;

	if (audio_buffer.size() < audio_buffer_idx + 2)
	{
		/* Audio buffer overflow...
		 * > Drop any existing samples
		 * > Drop any future samples until the next
		 *   call of retro_audio_upload() */
		audio_buffer_idx = 0;
		drop_samples = true;
		g_aud_overflow_events++;
		return;
	}

	audio_buffer[audio_buffer_idx++] = l;
	audio_buffer[audio_buffer_idx++] = r;
}
#endif

void InitAudio()
{
}

void TermAudio()
{
}

void StartAudioRecording(bool eight_khz)
{
}

u32 RecordAudio(void *buffer, u32 samples)
{
	return 0;
}

void StopAudioRecording()
{
}

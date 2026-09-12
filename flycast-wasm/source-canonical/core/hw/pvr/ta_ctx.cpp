#include "ta_ctx.h"
#include "spg.h"
#include "cfg/option.h"
#include "Renderer_if.h"
#include "serialize.h"
#include "stdclass.h"

#include <mutex>
#include <vector>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

extern u32 fskip;
static int RenderCount;

TA_context* ta_ctx;
tad_context ta_tad;

static void tactx_Recycle(TA_context* ctx);
static TA_context *tactx_Find(u32 addr, bool allocnew = false);

void SetCurrentTARC(u32 addr)
{
	if (addr != TACTX_NONE)
	{
		if (ta_ctx)
			SetCurrentTARC(TACTX_NONE);

		verify(ta_ctx == 0);
		//set new context
		ta_ctx = tactx_Find(addr,true);

		//copy cached params
		ta_tad = ta_ctx->tad;
	}
	else
	{
		//Flush cache to context
		verify(ta_ctx != 0);
		ta_ctx->tad=ta_tad;
		
		//clear context
		ta_ctx=0;
		ta_tad.Reset(0);
	}
}

static TA_context* rqueue;
static cResetEvent frame_finished;

bool QueueRender(TA_context* ctx)
{
	verify(ctx != 0);
	
	bool skipFrame = !rend_is_enabled();
	if (!skipFrame)
	{
		// WRC (2026-09-10): render-to-texture sub-renders (ctx->rend.isRTT)
		// must never be independently skipped by the frame-skip setting.
		// A game using RTT-based post-processing (render scene to a
		// texture, then a separate draw using that texture as the actual
		// displayed frame) issues these as a matched PAIR per visible
		// frame. RenderCount was a single global counter treating every
		// TA render request as fungible; the skip modulo is deterministic,
		// so once its phase falls out of sync with an RTT/display pair it
		// stays out of sync forever - silently starving one half of the
		// pair every single cycle, with no error (just a permanently
		// black or stale-looking screen). Confirmed live: Rez went
		// permanently black during gameplay with any frame-skip level
		// enabled, fixed instantly by disabling frame-skip. Only count/
		// skip real displayable frames; always let RTT sub-renders
		// through untouched by the skip counter.
		if (!ctx->rend.isRTT)
		{
			RenderCount++;
			if (RenderCount % (config::SkipFrame + 1) != 0)
				skipFrame = true;
		}
		if (!skipFrame && config::ThreadedRendering && rqueue != nullptr
				&& (config::AutoSkipFrame == 0 || (config::AutoSkipFrame == 1 && SH4FastEnough)))
			// The previous render hasn't completed yet so we wait.
			// If autoskipframe is enabled (normal level), we only do so if the CPU is running
			// fast enough over the last frames
			frame_finished.Wait();
	}

	if (skipFrame || rqueue)
	{
		tactx_Recycle(ctx);
		if (rend_is_enabled())
			fskip++;
		return false;
	}
	// disable net rollbacks until the render thread has processed the frame
	rend_disable_rollback();
	frame_finished.Reset();
	verify(rqueue == nullptr);
	rqueue = ctx;


	return true;
}

TA_context* DequeueRender()
{
	if (rqueue != nullptr)
		FrameCount++;

	return rqueue;
}

void FinishRender(TA_context* ctx)
{
	if (ctx != nullptr)
	{
		verify(rqueue == ctx);
		rqueue = nullptr;
		tactx_Recycle(ctx);
	}
	frame_finished.Set();
}

static std::mutex mtx_pool;
using Lock = std::lock_guard<std::mutex>;

static std::vector<TA_context*> ctx_pool;
static std::vector<TA_context*> ctx_list;

TA_context *tactx_Alloc()
{
	TA_context *ctx = nullptr;
	{
		Lock _(mtx_pool);
		if (!ctx_pool.empty()) {
			ctx = ctx_pool.back();
			ctx_pool.pop_back();
		}
	}

	if (ctx == nullptr) {
		ctx = new TA_context();
		ctx->Alloc();
	}
	return ctx;
}

static void tactx_Recycle(TA_context* ctx)
{
	if (ctx->nextContext != nullptr)
		tactx_Recycle(ctx->nextContext);
	Lock _(mtx_pool);
	if (ctx_pool.size() > 3) {
		delete ctx;
	}
	else {
		ctx->Reset();
		ctx_pool.push_back(ctx);
	}
}

static TA_context *tactx_Find(u32 addr, bool allocnew)
{
	TA_context *oldCtx = nullptr;
	for (TA_context *ctx : ctx_list)
	{
		if (ctx->Address == addr) {
			ctx->lastFrameUsed = FrameCount;
			return ctx;
		}
		if (FrameCount - ctx->lastFrameUsed > 60)
			oldCtx = ctx;
	}

	if (allocnew)
	{
		TA_context *ctx;
		if (oldCtx != nullptr)
		{
			ctx = oldCtx;
			ctx->Reset();
		}
		else
		{
			ctx = tactx_Alloc();
			ctx_list.push_back(ctx);
			// WRC - diagnostic: logs ctx_list growth for the iOS/Xbox memory
			// investigation. Commented out 2026-08-29 (paused, not resolved) -
			// uncomment (change to "#if defined(__EMSCRIPTEN__)") to resume.
#if 0 && defined(__EMSCRIPTEN__)
			EM_ASM({
				console.log('[WRC-DEBUG] tactx_Find: NEW context allocated, ctx_list.size()=' + $0 + ' addr=0x' + ($1>>>0).toString(16) + ' (~' + ($0 * 11) + 'MB est. cumulative TA context memory)');
			}, (int)ctx_list.size(), addr);
#endif
		}
		ctx->Address = addr;
		ctx->lastFrameUsed = FrameCount;

		return ctx;
	}
	return nullptr;
}

TA_context *tactx_Pop(u32 addr)
{
	for (size_t i = 0; i < ctx_list.size(); i++)
	{
		if (ctx_list[i]->Address == addr)
		{
			TA_context *ctx = ctx_list[i];
			
			if (::ta_ctx == ctx)
				SetCurrentTARC(TACTX_NONE);

			ctx_list.erase(ctx_list.begin() + i);

			return ctx;
		}
	}
	return nullptr;
}

void tactx_Term()
{
	if (ta_ctx != nullptr)
		SetCurrentTARC(TACTX_NONE);

	for (TA_context *ctx : ctx_list)
		delete ctx;
	ctx_list.clear();

	Lock _(mtx_pool);
	for (TA_context *ctx : ctx_pool)
		delete ctx;
	ctx_pool.clear();
}

const u32 NULL_CONTEXT = ~0u;

static void serializeContext(Serializer& ser, const TA_context *ctx)
{
	if (ser.dryrun())
	{
		// Maximum size: address, size, data
		ser.skip(4 + 4 + TA_DATA_SIZE);
		return;
	}
	if (ctx == nullptr)
	{
		ser << NULL_CONTEXT;
		return;
	}
	ser << ctx->Address;
	const tad_context& tad = ctx == ::ta_ctx ? ta_tad : ctx->tad;
	const u32 taSize = tad.thd_data - tad.thd_root;
	ser << taSize;
	ser.serialize(tad.thd_root, taSize);
}

static void deserializeContext(Deserializer& deser, TA_context **pctx)
{
	u32 address;
	deser >> address;
	if (address == NULL_CONTEXT)
	{
		*pctx = nullptr;
		return;
	}
	*pctx = tactx_Find(address, true);
	u32 size;
	deser >> size;
	tad_context& tad = (*pctx)->tad;
	deser.deserialize(tad.thd_root, size);
	tad.thd_data = tad.thd_root + size;
	if (deser.version() < Deserializer::V26)
	{
		u32 render_pass_count;
		deser >> render_pass_count;
		deser.skip(sizeof(u32) * render_pass_count);
	}
}

void SerializeTAContext(Serializer& ser)
{
	ser << (u32)ctx_list.size();
	int curCtx = -1;
	for (const auto& ctx : ctx_list)
	{
		if (ctx == ::ta_ctx)
			curCtx = (int)(&ctx - &ctx_list[0]);
		serializeContext(ser, ctx);
	}
	ser << curCtx;
}

void DeserializeTAContext(Deserializer& deser)
{
	if (::ta_ctx != nullptr)
		SetCurrentTARC(TACTX_NONE);
	if (deser.version() >= Deserializer::V25)
	{
		u32 listSize;
		deser >> listSize;
		for (const auto& ctx : ctx_list)
			tactx_Recycle(ctx);
		ctx_list.clear();
		for (u32 i = 0; i < listSize; i++)
		{
			TA_context *ctx;
			deserializeContext(deser, &ctx);
		}
		int curCtx;
		deser >> curCtx;
		if (curCtx >= 0 && curCtx < (int)ctx_list.size())
			SetCurrentTARC(ctx_list[curCtx]->Address);
	}
	else
	{
		TA_context *ta_cur_ctx;
		deserializeContext(deser, &ta_cur_ctx);
		if (ta_cur_ctx != nullptr)
			SetCurrentTARC(ta_cur_ctx->Address);
		if (deser.version() >= Deserializer::V20)
			deserializeContext(deser, &ta_cur_ctx);
	}
}

/* WRC stub for gfx_widget_achievement_popup
 * The real widget draws an on-screen popup via RetroArch's display system.
 * In WRC we suppress all in-engine rendering; achievement notifications are
 * forwarded to the JS/React UI layer via EM_ASM.
 */

#include "../gfx_widgets.h"
#include <emscripten.h>

/* --- gfx_widget_t interface stubs --- */

static bool gfx_widget_achievement_popup_init(
      gfx_display_t *p_disp,
      gfx_animation_t *p_anim,
      bool video_is_threaded, bool fullscreen)
{
   return true;
}

static void gfx_widget_achievement_popup_free(void) { }
static void gfx_widget_achievement_popup_context_destroy(void) { }
static void gfx_widget_achievement_popup_frame(void *data, void *userdata) { }

const gfx_widget_t gfx_widget_achievement_popup = {
   &gfx_widget_achievement_popup_init,
   &gfx_widget_achievement_popup_free,
   NULL, /* context_reset */
   &gfx_widget_achievement_popup_context_destroy,
   NULL, /* layout */
   NULL, /* iterate */
   &gfx_widget_achievement_popup_frame
};

/* --- Public API called by cheevos.c (guarded by HAVE_GFX_WIDGETS) --- */

void gfx_widgets_push_achievement(const char *title,
      const char *subtitle, const char *badge)
{
   EM_ASM({
      var title    = UTF8ToString($0);
      var subtitle = UTF8ToString($1);
      var badge    = UTF8ToString($2);
      if (window.emulator && window.emulator.onAchievementUnlocked)
         window.emulator.onAchievementUnlocked(title, subtitle, badge);
   }, title ? title : "", subtitle ? subtitle : "", badge ? badge : "");
}

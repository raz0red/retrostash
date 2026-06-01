/* WRC stub for gfx_widget_leaderboard_display
 * The real widget draws leaderboard values on screen via RetroArch's display
 * system.  In WRC we suppress all in-engine rendering; leaderboard updates are
 * handled by the JS/React UI layer instead (phase 2: JS bridge callbacks).
 */

#include "../gfx_widgets.h"

/* --- gfx_widget_t interface stubs --- */

static bool gfx_widget_leaderboard_display_init(
      gfx_display_t *p_disp,
      gfx_animation_t *p_anim,
      bool video_is_threaded, bool fullscreen)
{
   return true;
}

static void gfx_widget_leaderboard_display_free(void) { }
static void gfx_widget_leaderboard_display_context_destroy(void) { }
static void gfx_widget_leaderboard_display_frame(void *data, void *userdata) { }

const gfx_widget_t gfx_widget_leaderboard_display = {
   &gfx_widget_leaderboard_display_init,
   &gfx_widget_leaderboard_display_free,
   NULL, /* context_reset */
   &gfx_widget_leaderboard_display_context_destroy,
   NULL, /* layout */
   NULL, /* iterate */
   &gfx_widget_leaderboard_display_frame
};

/* --- Public API called by cheevos.c (guarded by HAVE_GFX_WIDGETS) --- */

void gfx_widgets_set_leaderboard_display(unsigned id, const char *value)
{
   /* TODO (phase 2): call JS bridge to update leaderboard display in React UI */
   (void)id;
   (void)value;
}

void gfx_widgets_set_challenge_display(unsigned id, const char *badge)
{
   /* TODO (phase 2): call JS bridge to show/hide challenge indicator in React UI */
   (void)id;
   (void)badge;
}

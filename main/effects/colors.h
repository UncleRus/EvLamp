/**
 * Blending colors
 */
#ifndef __EFFECTS_COLORS_H__
#define __EFFECTS_COLORS_H__

#include "effect.h"

extern EFFECT_PARAMS(colors, 3);

esp_err_t effect_colors_prepare(framebuffer_t *fb);

esp_err_t effect_colors_run(framebuffer_t *fb);

#define DESCR_EFFECT_COLORS DECL_EFFECT_FULL(colors, "Colors")

#endif /* __EFFECTS_COLORS_H__ */

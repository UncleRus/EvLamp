/**
 * Simple rainbow effect
 */
#ifndef __EFFECTS_RAINBOW_H__
#define __EFFECTS_RAINBOW_H__

#include "effect.h"

extern EFFECT_PARAMS(rainbow, 3);

esp_err_t effect_rainbow_run(framebuffer_t *fb);

#define DESCR_EFFECT_RAINBOW DECL_EFFECT_SHORT(rainbow, "Rainbow")

#endif /* __EFFECTS_RAINBOW_H__ */

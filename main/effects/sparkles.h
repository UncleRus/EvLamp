/**
 * Colored sparkles effect
 */
#ifndef __EFFECTS_SPARKLES_H__
#define __EFFECTS_SPARKLES_H__

#include "effect.h"

extern EFFECT_PARAMS(sparkles, 2);

esp_err_t effect_sparkles_run(framebuffer_t *fb);

#define DESCR_EFFECT_SPARKLES DECL_EFFECT_SHORT(sparkles, "Sparkles")

#endif /* __EFFECTS_SPARKLES_H__ */

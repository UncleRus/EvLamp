/**
 * Perlin noise effect
 *
 * Author: Chuck Sommerville
 */
#ifndef __EFFECTS_LAVA_H__
#define __EFFECTS_LAVA_H__

#include "effect.h"

extern EFFECT_PARAMS(lava, 4);

esp_err_t effect_lava_run(framebuffer_t *fb);

#define DESCR_EFFECT_LAVA DECL_EFFECT_SHORT(lava, "Lava")

#endif /* __EFFECTS_LAVA_H__ */

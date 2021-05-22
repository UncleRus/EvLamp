/**
 * Rolling rays effect
 */
#ifndef __EFFECTS_ROLLRAYS_H__
#define __EFFECTS_ROLLRAYS_H__

#include "effect.h"

extern EFFECT_PARAMS(rollrays, 5);

esp_err_t effect_rollrays_run(framebuffer_t *fb);

#define DESCR_EFFECT_ROLLRAYS DECL_EFFECT_SHORT(rollrays, "Rolling rays")

#endif /* __EFFECTS_ROLLRAYS_H__ */

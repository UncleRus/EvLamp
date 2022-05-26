/**
 * Radiant wheel effect
 */
#ifndef __EFFECTS_WHEEL_H__
#define __EFFECTS_WHEEL_H__

#include "effect.h"

extern EFFECT_PARAMS(wheel, 6);

esp_err_t effect_wheel_run(framebuffer_t *fb);

#define DESCR_EFFECT_WHEEL DECL_EFFECT_SHORT(wheel, "Radiant wheel")

#endif /* __EFFECTS_WHEEL_H__ */

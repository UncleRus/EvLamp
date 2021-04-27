/**
 * Rain effect by Shaitan
 */
#ifndef __EFFECTS_RAIN_H__
#define __EFFECTS_RAIN_H__

#include "effect.h"

extern EFFECT_PARAMS(rain, 4);

esp_err_t effect_rain_prepare(framebuffer_t *fb);

esp_err_t effect_rain_run(framebuffer_t *fb);

#define DESCR_EFFECT_RAIN DECL_EFFECT_FULL(rain, "Rain")

#endif /* __EFFECTS_RAIN_H__ */

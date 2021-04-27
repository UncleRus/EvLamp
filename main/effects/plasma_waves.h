/**
 * Plasma waves effect
 *
 * Author: Edmund "Skorn" Horn
 */
#ifndef __EFFECTS_PLASMA_WAVES_H__
#define __EFFECTS_PLASMA_WAVES_H__

#include "effect.h"

extern EFFECT_PARAMS(plasma_waves, 1);

esp_err_t effect_plasma_waves_prepare(framebuffer_t *fb);

esp_err_t effect_plasma_waves_run(framebuffer_t *fb);

#define DESCR_EFFECT_PLASMA_WAVES DECL_EFFECT_FULL(plasma_waves, "Plasma waves")

#endif /* __EFFECTS_PLASMA_WAVES_H__ */

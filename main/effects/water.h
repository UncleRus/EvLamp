/**
 * Water surface effect
 *
 * https://wokwi.com/arduino/projects/287787908971102732
 */
#ifndef __EFFECTS_WATER_H__
#define __EFFECTS_WATER_H__

#include "effect.h"

extern EFFECT_PARAMS(water, 3);

esp_err_t effect_water_prepare(framebuffer_t *fb);

esp_err_t effect_water_run(framebuffer_t *fb);

#define DESCR_EFFECT_WATER DECL_EFFECT_FULL(water, "Water")

#endif /* __EFFECTS_WATER_H__ */

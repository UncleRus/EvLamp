/**
 * Waterfall/Fire effect
 *
 * Parameters:
 *     - hue:      Basic hue for waterfall palette. Ignored when mode = WATERFALL_FIRE
 *     - cooling:  How much does the air cool as it rises. Less cooling = taller flames,
 *                 more cooling = shorter flames. Suggested range 20-100.
 *     - sparking: Chance (out of 255) that a new spark will light up. Suggested range 50-200.
 *
 * Recommended parameters for fire mode: cooling = 90, sparking = 80
 */
#ifndef __EFFECTS_WATERFALL_H__
#define __EFFECTS_WATERFALL_H__

#include "effect.h"

extern EFFECT_PARAMS(waterfall, 4);

esp_err_t effect_waterfall_prepare(framebuffer_t *fb);

esp_err_t effect_waterfall_run(framebuffer_t *fb);

#define DESCR_EFFECT_WATERFALL DECL_EFFECT_FULL(waterfall, "Waterfall")

#endif /* __EFFECTS_WATERFALL_H__ */

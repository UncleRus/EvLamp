/**
 * Polar lights effect
 *
 * Author: Kostyantyn Matviyevskyy kostyamat 05.02.2021
 *
 * https://editor.soulmatelights.com/gallery/762-polar-lights
 */
#ifndef __EFFECTS_POLAR_LIGHTS_H__
#define __EFFECTS_POLAR_LIGHTS_H__

#include "effect.h"

extern EFFECT_PARAMS(polar_lights, 4);

esp_err_t effect_polar_lights_prepare(framebuffer_t *fb);

esp_err_t effect_polar_lights_run(framebuffer_t *fb);

#define DESCR_EFFECT_POLAR_LIGHTS DECL_EFFECT_FULL(polar_lights, "Polar lights")

#endif /* __EFFECTS_POLAR_LIGHTS_H__ */

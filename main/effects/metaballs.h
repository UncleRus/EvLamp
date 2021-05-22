/**
 * Metaballs effect
 *
 * Author: Yaroslaw Turbin 02.09.2020 (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 *
 * https://wokwi.com/arduino/libraries/FastLED/Metaballs
 */
#ifndef __EFFECTS_METABALLS_H__
#define __EFFECTS_METABALLS_H__

#include "effect.h"

extern EFFECT_PARAMS(metaballs, 2);

esp_err_t effect_metaballs_prepare(framebuffer_t *fb);

esp_err_t effect_metaballs_run(framebuffer_t *fb);

#define DESCR_EFFECT_METABALLS DECL_EFFECT_FULL(metaballs, "Metaballs")

#endif /* __EFFECTS_METABALLS_H__ */

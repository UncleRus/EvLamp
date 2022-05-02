/**
 * Fireflies (wind) effect
 *
 * Author: stepko
 * https://wokwi.com/projects/289974688028819977
 */
#ifndef __EFFECTS_FIREFLIES_H__
#define __EFFECTS_FIREFLIES_H__

#include "effect.h"

extern EFFECT_PARAMS(fireflies, 4);

esp_err_t effect_fireflies_prepare(framebuffer_t *fb);

esp_err_t effect_fireflies_run(framebuffer_t *fb);

#define DESCR_EFFECT_FIREFLIES DECL_EFFECT_FULL(fireflies, "Fireflies")

#endif /* __EFFECTS_FIREFLIES_H__ */

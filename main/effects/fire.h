/**
 * Fire effect based on Perlin noise
 *
 * Author: Yaroslaw Turbin (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 *
 * https://pastebin.com/jSSVSRi6
 */
#ifndef __EFFECTS_FIRE_H__
#define __EFFECTS_FIRE_H__

#include "effect.h"

extern EFFECT_PARAMS(fire, 2);

esp_err_t effect_fire_prepare(framebuffer_t *fb);

esp_err_t effect_fire_run(framebuffer_t *fb);

#define DESCR_EFFECT_FIRE DECL_EFFECT_FULL(fire, "Fire")

#endif /* __EFFECTS_FIRE_H__ */

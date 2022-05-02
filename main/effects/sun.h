/**
 * Sun radiation effect
 *
 * Author: Yaroslaw Turbin (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 */
#ifndef __EFFECTS_SUN_H__
#define __EFFECTS_SUN_H__

#include "effect.h"

extern EFFECT_PARAMS(sun, 2);

esp_err_t effect_sun_prepare(framebuffer_t *fb);

esp_err_t effect_sun_run(framebuffer_t *fb);

#define DESCR_EFFECT_SUN DECL_EFFECT_FULL(sun, "Sun radiation")

#endif /* __EFFECTS_SUN_H__ */

/**
 * Oldschool twister demo effect
 *
 * Author: Yaroslaw Turbin (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 *
 * https://wokwi.com/arduino/projects/297732081748804105
 *
 * Max supported framebuffer size is 256x256
 */
#ifndef __EFFECTS_TWISTER_H__
#define __EFFECTS_TWISTER_H__

#include "effect.h"

extern EFFECT_PARAMS(twister, 3);

esp_err_t effect_twister_run(framebuffer_t *fb);

#define DESCR_EFFECT_TWISTER DECL_EFFECT_SHORT(twister, "Twister")

#endif /* __EFFECTS_TWISTER_H__ */

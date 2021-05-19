/**
 * Flower effect
 *
 * Author: Yaroslaw Turbin (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 */
#ifndef __EFFECTS_FLOWER_H__
#define __EFFECTS_FLOWER_H__

#include "effect.h"

extern EFFECT_PARAMS(flower, 2);

esp_err_t effect_flower_run(framebuffer_t *fb);

#define DESCR_EFFECT_FLOWER DECL_EFFECT_SHORT(flower, "Flower")

#endif /* __EFFECTS_FLOWER_H__ */

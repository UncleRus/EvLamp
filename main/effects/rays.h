/**
 * Colored rays effect
 *
 * Author: Yaroslaw Turbin (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 * https://editor.soulmatelights.com/gallery/819-colored-bursts
 *
 * Max supported framebuffer size is 256x256
 */
#ifndef __EFFECTS_RAYS_H__
#define __EFFECTS_RAYS_H__

#include "effect.h"

extern EFFECT_PARAMS(rays, 3);

esp_err_t effect_rays_run(framebuffer_t *fb);

#define DESCR_EFFECT_RAYS DECL_EFFECT_SHORT(rays, "Rays")

#endif /* __EFFECTS_RAYS_H__ */

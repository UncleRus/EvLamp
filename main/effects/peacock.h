/**
 * Peacock (sin plasma) effect
 *
 * Author: Yaroslaw Turbin (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 */
#ifndef __EFFECTS_PEACOCK_H__
#define __EFFECTS_PEACOCK_H__

#include "effect.h"

extern EFFECT_PARAMS(peacock, 3);

esp_err_t effect_peacock_run(framebuffer_t *fb);

#define DESCR_EFFECT_PEACOCK DECL_EFFECT_SHORT(peacock, "Peacock tail")

#endif /* __EFFECTS_PEACOCK_H__ */

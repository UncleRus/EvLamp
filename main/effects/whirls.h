/**
 * Whirls effect by SottNick
 */
#ifndef __EFFECTS_WHIRLS_H__
#define __EFFECTS_WHIRLS_H__

#include "effect.h"

extern EFFECT_PARAMS(whirls, 5);

esp_err_t effect_whirls_prepare(framebuffer_t *fb);

esp_err_t effect_whirls_run(framebuffer_t *fb);

#define DESCR_EFFECT_WHIRLS DECL_EFFECT_FULL(whirls, "Whirls")

#endif /* __EFFECTS_WHIRLS_H__ */

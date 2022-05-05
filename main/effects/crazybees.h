/**
 * Crazy Bees effect
 *
 * Author: Stepko
 */
#ifndef __EFFECTS_CRAZYBEES_H__
#define __EFFECTS_CRAZYBEES_H__

#include "effect.h"

extern EFFECT_PARAMS(crazybees, 3);

esp_err_t effect_crazybees_prepare(framebuffer_t *fb);

esp_err_t effect_crazybees_run(framebuffer_t *fb);

#define DESCR_EFFECT_CRAZYBEES DECL_EFFECT_FULL(crazybees, "Crazy bees")

#endif /* __LED_EFFECTS_CRAZYBEES_H__ */

/**
 * Light soft color change
 *
 * Author: Julia Uss
 */
#ifndef __EFFECTS_RELAX_H__
#define __EFFECTS_RELAX_H__

#include "effect.h"

extern EFFECT_PARAMS(relax, 4);

esp_err_t effect_relax_prepare(framebuffer_t *fb);

esp_err_t effect_relax_run(framebuffer_t *fb);

#define DESCR_EFFECT_RELAX DECL_EFFECT_FULL(relax, "Relax")

#endif /* __EFFECTS_RELAX_H__ */

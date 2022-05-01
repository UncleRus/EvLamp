/**
 * Diagonal movements effect by SottNick
 */
#ifndef __EFFECTS_DIAGONALS_H__
#define __EFFECTS_DIAGONALS_H__

#include "effect.h"

extern EFFECT_PARAMS(diagonals, 1);

esp_err_t effect_diagonals_prepare(framebuffer_t *fb);

esp_err_t effect_diagonals_run(framebuffer_t *fb);

#define DESCR_EFFECT_DIAGONALS DECL_EFFECT_FULL(diagonals, "Diagonal movements")

#endif /* __EFFECTS_DIAGONALS_H__ */

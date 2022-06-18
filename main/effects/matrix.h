#ifndef __EFFECTS_MATRIX_H__
#define __EFFECTS_MATRIX_H__

// FIXME: need to rewrite completely

#include "effect.h"

extern EFFECT_PARAMS(matrix, 1);

esp_err_t effect_matrix_run(framebuffer_t *fb);

#define DESCR_EFFECT_MATRIX DECL_EFFECT_SHORT(matrix, "Matrix")

#endif /* __EFFECTS_MATRIX_H__ */

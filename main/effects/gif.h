#ifndef __EFFECTS_GIF_H__
#define __EFFECTS_GIF_H__

#include "effect.h"

extern EFFECT_PARAMS(gif, 1);

esp_err_t effect_gif_prepare(framebuffer_t *fb);

esp_err_t effect_gif_run(framebuffer_t *fb);

#define DESCR_EFFECT_GIF DECL_EFFECT_FULL(gif, "Gif image")

#endif /* __EFFECTS_GIF_H__ */

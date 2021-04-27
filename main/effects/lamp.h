#ifndef __EFFECTS_LAMP_H__
#define __EFFECTS_LAMP_H__

#include "effect.h"

extern EFFECT_PARAMS(lamp, 3);

esp_err_t effect_lamp_run(framebuffer_t *fb);

#define DESCR_EFFECT_LAMP DECL_EFFECT_SHORT(lamp, "Color lamp")

#endif /* __EFFECTS_LAMP_H__ */

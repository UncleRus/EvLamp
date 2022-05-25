/**
 * Color hoop
 *
 * https://wokwi.com/arduino/projects/297414039372300810
 *
 * max framebuffer size 256x256
 */
#ifndef __EFFECTS_HOOP_H__
#define __EFFECTS_HOOP_H__

#include "effect.h"

extern EFFECT_PARAMS(hoop, 1);

esp_err_t effect_hoop_run(framebuffer_t *fb);

#define DESCR_EFFECT_HOOP DECL_EFFECT_SHORT(hoop, "Hoop")

#endif /* __EFFECTS_HOOP_H__ */

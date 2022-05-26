/**
 * Colored dunes (AA lines)
 *
 * Author: costyn (https://wokwi.com/projects/284541784463245837)
 */
#ifndef __EFFECTS_DUNES_H__
#define __EFFECTS_DUNES_H__

#include "effect.h"

extern EFFECT_PARAMS(dunes, 2);

esp_err_t effect_dunes_run(framebuffer_t *fb);

#define DESCR_EFFECT_DUNES DECL_EFFECT_SHORT(dunes, "Colored dunes")

#endif /* __EFFECTS_DUNES_H__ */

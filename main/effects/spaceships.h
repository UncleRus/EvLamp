/**
 * Flight of spaceships effect
 *
 * Author: stepko
 * https://wokwi.com/projects/289609636203463176
 */
#ifndef __EFFECTS_SPACESHIPS_H__
#define __EFFECTS_SPACESHIPS_H__

#include "effect.h"

extern EFFECT_PARAMS(spaceships, 2);

esp_err_t effect_spaceships_run(framebuffer_t *fb);

#define DESCR_EFFECT_SPACESHIPS DECL_EFFECT_SHORT(spaceships, "Flight of spaceships")

#endif /* __EFFECTS_SPACESHIPS_H__ */

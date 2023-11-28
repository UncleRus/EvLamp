/**
 * "Octopus"
 *
 *  Author: Stepko
 *  https://editor.soulmatelights.com/gallery/671-octopus
 *
 *  Note: only even-sized framebuffers are supported
 */
#ifndef __EFFECTS_OCTOPUS_H__
#define __EFFECTS_OCTOPUS_H__

#include "effect.h"

extern EFFECT_PARAMS(octopus, 3);

esp_err_t effect_octopus_prepare(framebuffer_t *fb);

esp_err_t effect_octopus_run(framebuffer_t *fb);

#define DESCR_EFFECT_OCTOPUS DECL_EFFECT_FULL(octopus, "Octopus")

#endif /* __EFFECTS_OCTOPUS_H__ */

/**
 * "Nuclear Noise"
 *
 *  Author: Stepko
 *  https://editor.soulmatelights.com/gallery/1569-radialnuclearnoise
 *
 *  Note: only even-sized framebuffers are supported
 */
#ifndef __EFFECTS_NUCLEAR_H__
#define __EFFECTS_NUCLEAR_H__

#include "effect.h"

extern EFFECT_PARAMS(nuclear, 1);

esp_err_t effect_nuclear_prepare(framebuffer_t *fb);

esp_err_t effect_nuclear_run(framebuffer_t *fb);

#define DESCR_EFFECT_NUCLEAR DECL_EFFECT_FULL(nuclear, "Nuclear Noise")

#endif /* __EFFECTS_NUCLEAR_H__ */

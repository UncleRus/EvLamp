/**
 * DNA helix effect
 *
 * Author: Yaroslaw Turbin (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 *
 * Max supported framebuffer size is 256x256
 */
#ifndef __EFFECTS_DNA_H__
#define __EFFECTS_DNA_H__

#include "effect.h"

extern EFFECT_PARAMS(dna, 3);

esp_err_t effect_dna_run(framebuffer_t *fb);

#define DESCR_EFFECT_DNA DECL_EFFECT_SHORT(dna, "DNA helix")

#endif /* __EFFECTS_DNA_H__ */

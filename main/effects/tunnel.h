/**
 * Tunnel effect
 *
 * Original Lua/TIC-80 source was live-coded in 25 minutes by exoticorn during a 256-byte battle
 * at outlinedemoparty.nl 2021-05-15
 *
 * Author: exoticorn, Ported by: sutarobosu (https://wokwi.com/projects/332710874737279572)
 */
#ifndef __EFFECTS_TUNNEL_H__
#define __EFFECTS_TUNNEL_H__

#include "effect.h"

extern EFFECT_PARAMS(tunnel, 1);

esp_err_t effect_tunnel_prepare(framebuffer_t *fb);

esp_err_t effect_tunnel_run(framebuffer_t *fb);

#define DESCR_EFFECT_TUNNEL DECL_EFFECT_FULL(tunnel, "Tunnel")

#endif /* __EFFECTS_TUNNEL_H__ */

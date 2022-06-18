/**
 * Tunnel effect
 *
 * Original Lua/TIC-80 source was live-coded in 25 minutes by exoticorn during a 256-byte battle
 * at outlinedemoparty.nl 2021-05-15
 *
 * Author: exoticorn, Ported by: sutarobosu (https://wokwi.com/projects/332710874737279572)
 */
#include "effects/tunnel.h"

#include <lib8tion.h>
#include <math.h>

#define P_SPEED 0

EFFECT_PARAMS(tunnel, 1) = {
    DECL_PARAM_RANGE(P_SPEED, "Speed", 1, 170, 90),
};

static uint8_t half_width = 0;
static uint8_t half_height = 0;
static rgb_t *last = NULL;

static const uint32_t pal[] = {
    0x1a1c2c, 0x5d275d, 0xb13e53, 0xef7d57,
    0xffcd75, 0xa7f070, 0x38b764, 0x257179, 0x29366f, 0x3b5dc9,
    0x41a6f6, 0x73eff7, 0xf4f4f4, 0x94b0c2, 0x566c86, 0x333c57
};

esp_err_t effect_tunnel_prepare(framebuffer_t *fb)
{
    half_width = fb->width / 2;
    half_height = fb->height / 2;
    last = fb->data + fb->height * fb->width - 1;
    return ESP_OK;
}

esp_err_t effect_tunnel_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    rgb_t *pixel = last;
    uint8_t c;

    float t = ((esp_timer_get_time() / 1000) % (21 * (201 - PARAM_VAL(tunnel, P_SPEED)) * 2)) / (201.0f - PARAM_VAL(tunnel, P_SPEED)), z, q, w;
    for (int8_t y = -half_height - half_height / 8; y < half_height - half_height / 8; y++)
    {
        for (int8_t x = -half_width; x <= 0; x++)
        {
            z = 20.0f / sqrtf(x * x + y * y) + t;
            q = (fmodf(z, 21) < 15) ? z - fmodf(z, 21) + 15 : z;
            w = 9.0f / y + t;
            if (y > 0 && w < q)
                c = (powf(x * (w - t), 2) < 39 ? 14 : 6) + fmodf(w, 2);
            else if (-y * (q - t) < 99.f / (powf(x * (q - t) / 50.f, 2) + 1))
                c = (q == z) ? fmodf(z, 2) : 3;
            else
                c = 9 - y / 3 - ((0x2 >> (-y % 3)) & 1);
            *pixel-- = rgb_from_code(pal[c]);
        }
        rgb_t *src = pixel + 1;
        for (uint8_t x = 1; x < half_width; x++)
            *pixel-- = *++src;
    }

    return fb_end(fb);
}

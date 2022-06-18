/**
 * @file fire.c
 *
 * Fire effect based on Perlin noise
 *
 * Author: Yaroslaw Turbin (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 *
 * https://pastebin.com/jSSVSRi6
 */
#include "effects/fire.h"

#include <lib8tion.h>
#include <noise.h>
#include "palettes.h"

#define MAX_SCALE 100
#define HALF_SCALE (MAX_SCALE / 2)


#define P_PALETTE 0
#define P_SCALE   1

EFFECT_PARAMS(fire, 2) = {
    DECL_PARAM_RANGE(P_PALETTE, "Palette", PALETTE_FIRE, PALETTE_LITHIUM_FIRE, 0),
    DECL_PARAM_RANGE(P_SCALE, "Scale", 10, MAX_SCALE, 60),
};

static uint32_t time_scale = 1;

esp_err_t effect_fire_prepare(framebuffer_t *fb)
{
    time_scale = PARAM_VAL(fire, P_SCALE) >= HALF_SCALE ? 1 : (HALF_SCALE - PARAM_VAL(fire, P_SCALE)) / 2;

    return ESP_OK;
}

esp_err_t effect_fire_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    uint32_t time_ms = esp_timer_get_time() / 1000;

    for (size_t x = 0; x < fb->width; x++)
        for (size_t y = 0; y < fb->height; y++)
        {
            uint8_t idx = qsub8(
                inoise8_3d(x * PARAM_VAL(fire, P_SCALE), y * PARAM_VAL(fire, P_SCALE) + time_ms / time_scale, time_ms / 3),
                abs8(y - (fb->height - 1)) * 255 / (fb->height - 1)
            );
            rgb_t c = color_from_palette_rgb(palettes[PARAM_VAL(fire, P_PALETTE)].palette, palettes[PARAM_VAL(fire, P_PALETTE)].size, idx, 255, true);
            fb_set_pixel_rgb(fb, x, fb->height - y - 1, c);
        }

    return fb_end(fb);
}

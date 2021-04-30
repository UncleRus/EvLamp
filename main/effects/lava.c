/**
 * Perlin noise effect
 *
 * Author: Chuck Sommerville
 */
#include "effects/lava.h"

#include <noise.h>
#include <lib8tion.h>

#define P_SCALE 0
#define P_SPEED 1

EFFECT_PARAMS(lava, 2) = {
    DECL_PARAM(P_SCALE, "Scale", 10, 100, 40),
    DECL_PARAM(P_SPEED, "Speed", 1,  50,  15),
};

static uint16_t z_pos;
static uint16_t x_offs;
static uint8_t hue;

esp_err_t effect_lava_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    if (!(fb->frame_num % 30)) x_offs++;

    z_pos += EPARAM(lava, P_SPEED);
    hue++;

    for (int x = 0; x < fb->width; x++)
        for (int y = 0; y < fb->height; y++)
        {
            uint8_t noise = inoise8_3d(x * EPARAM(lava, P_SCALE), y * EPARAM(lava, P_SCALE), z_pos);
            fb_set_pixel_hsv(fb, x, y, hsv_from_values(hue + noise, 255, 255));
        }

    return fb_end(fb);
}

/**
 * Perlin noise effect
 *
 * Author: Chuck Sommerville
 */
#include "effects/lava.h"

#include <noise.h>
#include <lib8tion.h>

#define MODE_RAINBOW 0
#define MODE_LAVA    1

#define P_SCALE 0
#define P_SPEED 1
#define P_MODE 2

EFFECT_PARAMS(lava, 3) = {
    DECL_PARAM(P_SCALE, "Scale", 5, 100, 40),
    DECL_PARAM(P_SPEED, "Speed", 0, 50, 15),
    DECL_PARAM(P_MODE, "Mode", 0, 1, 1),
};

static const rgb_t palette[] = {
    { .r = 0,   .g = 0,   .b = 0   },
    { .r = 0,   .g = 0,   .b = 0   },
    { .r = 0,   .g = 0,   .b = 0   },
    { .r = 255, .g = 0,   .b = 0   },
    { .r = 255, .g = 255, .b = 0   },
    { .r = 255, .g = 255, .b = 255 },
};

#define PALETTE_SIZE (sizeof(palette) / sizeof(rgb_t))

static uint16_t z_pos;
static uint16_t x_offs;
static uint8_t hue;

esp_err_t effect_lava_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    if (!(fb->frame_num % 30)) x_offs++;

    z_pos += PARAM_VAL(lava, P_SPEED);
    hue++;

    for (int x = 0; x < fb->width; x++)
        for (int y = 0; y < fb->height; y++)
        {
            uint8_t noise = inoise8_3d(x * PARAM_VAL(lava, P_SCALE) + x_offs, y * PARAM_VAL(lava, P_SCALE), z_pos);

            if (PARAM_VAL(lava, P_MODE) == MODE_RAINBOW)
                fb_set_pixel_hsv(fb, x, y, hsv_from_values(hue + noise, 255, 255));
            else
                fb_set_pixel_rgb(fb, x, y, color_from_palette_rgb((rgb_t *)palette, PALETTE_SIZE, noise, 255, true));
        }

    return fb_end(fb);
}

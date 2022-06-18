/**
 * Perlin noise effect
 *
 * Author: Chuck Sommerville
 */
#include "effects/lava.h"

#include <noise.h>
#include <lib8tion.h>
#include "palettes.h"

#define MODE_RAINBOW 0
#define MODE_LAVA    1
#define MODE_PALETTE 2

#define P_SCALE 0
#define P_SPEED 1
#define P_MODE 2
#define P_PALETTE 3

EFFECT_PARAMS(lava, 4) = {
    DECL_PARAM_RANGE(P_SCALE, "Scale", 5, 100, 40),
    DECL_PARAM_RANGE(P_SPEED, "Speed", 0, 50, 15),
    DECL_PARAM_RANGE(P_MODE, "Mode", 0, MODE_PALETTE, 1),
    DECL_PARAM_RANGE(P_PALETTE, "Palette", 0, PALETTE_MAX - 1, 0),
};

static const rgb_t lava_palette[] = {
    C_BLACK,
    C_BLACK,
    C_BLACK,
    C_DARK_RED,
    C_RED,
    C_YELLOW,
    C_WHITE
};

#define LAVA_PALETTE_SIZE (sizeof(lava_palette) / sizeof(rgb_t))

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

            switch (PARAM_VAL(lava, P_MODE))
            {
                case MODE_RAINBOW:
                    fb_set_pixel_hsv(fb, x, y, hsv_from_values(hue + noise, 255, 255));
                    break;
                case MODE_LAVA:
                    fb_set_pixel_rgb(fb, x, y, color_from_palette_rgb(lava_palette, LAVA_PALETTE_SIZE, noise, 255, true));
                    break;
                default:
                    fb_set_pixel_rgb(fb, x, y, color_from_palette_rgb(palettes[PARAM_VAL(lava, P_PALETTE)].palette,
                            palettes[PARAM_VAL(lava, P_PALETTE)].size, noise, 255, true));
                    break;
            }
        }

    return fb_end(fb);
}

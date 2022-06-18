/**
 * Colored dunes (AA lines)
 *
 * Author: costyn (https://wokwi.com/projects/284541784463245837)
 */
#include "effects/dunes.h"

#include <lib8tion.h>
#include "palettes.h"

#define P_SPEED   0
#define P_PALETTE 1

EFFECT_PARAMS(dunes, 2) = {
    DECL_PARAM_RANGE(P_SPEED, "Speed", 1, 40, 5),
    DECL_PARAM_RANGE(P_PALETTE, "Palette", 0, PALETTE_MAX - 1, PALETTE_WOOD_FIRE),
};

esp_err_t effect_dunes_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    uint32_t ms = esp_timer_get_time() / 1000 * PARAM_VAL(dunes, P_SPEED) / 10;

    uint32_t y_hue_delta = (int32_t)sin16(ms * 11) / 128;
    uint32_t x_hue_delta = (int32_t)cos16(ms * 11) / 128;
    uint32_t start_hue = ms << 8;
    uint32_t line_start_hue = start_hue - (fb->height + 1) / 2 * y_hue_delta;
    int16_t yd2 = sin16(ms * 3) / 32;
    int16_t xd2 = sin16(ms * 7) / 32;
    for (size_t y = 0; y < fb->height; y++)
    {
        uint32_t pixel_hue = line_start_hue - (fb->width + 1) / 2 * x_hue_delta;
        uint32_t xhd = x_hue_delta;
        line_start_hue += y_hue_delta;
        y_hue_delta += yd2;
        for (size_t x = 0; x < fb->width; x++)
        {
            //rgb_t c = color_from_palette_rgb(palette, palette_size, pixel_hue >> 8, 255, true);
            rgb_t c = color_from_palette_rgb(
                palettes[PARAM_VAL(dunes, P_PALETTE)].palette,
                palettes[PARAM_VAL(dunes, P_PALETTE)].size,
                pixel_hue >> 8, 255, true);
            fb_set_pixel_rgb(fb, x, y, c);
            pixel_hue += xhd;
            xhd += xd2;
        }
    }

    return fb_end(fb);
}

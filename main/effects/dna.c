/**
 * DNA spiral effect
 *
 * Author: Yaroslaw Turbin (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 *
 * Max supported framebuffer size is 256x256
 */
#include "effects/dna.h"

#include <lib8tion.h>
#include "palettes.h"

#define P_SPEED  0
#define P_SIZE   1
#define P_BORDER 2

EFFECT_PARAMS(dna, 3) = {
    DECL_PARAM_RANGE(P_SPEED, "Rotation speed", 10, 100, 50),
    DECL_PARAM_RANGE(P_SIZE, "Spiral size", 1, 10, 5),
    DECL_PARAM_BOOL(P_BORDER, "Add white border", 1),
};

static uint32_t offset;

static const rgb_t dark_slate_gray = C_DARK_SLATE_GRAY;
static const rgb_t white = C_WHITE;

static void horizontal_line(framebuffer_t *fb, uint8_t x1, uint8_t x2, uint8_t y, rgb_t color, bool dot)
{
    uint8_t steps = abs8(x2 - x1) + 1;

    for (uint8_t i = 1; i <= steps; i++)
    {
        uint8_t dx = lerp8by8(x1, x2, i * 255 / steps);
        rgb_t pixel;
        fb_get_pixel_rgb(fb, dx, y, &pixel);
        pixel = rgb_scale_video(rgb_add_rgb(pixel, color), i * 255 / steps);
        fb_set_pixel_rgb(fb, dx, y, pixel);
    }

    if (dot)
    {
        //add white point at the ends of line
        rgb_t pixel;
        fb_get_pixel_rgb(fb, x1, y, &pixel);
        fb_set_pixel_rgb(fb, x1, y, rgb_add_rgb(pixel, dark_slate_gray));
        fb_set_pixel_rgb(fb, x2, y, white);
    }
}

esp_err_t effect_dna_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    offset += PARAM_VAL(dna, P_SPEED) / 10;

    fb_fade(fb, 130);

    for (uint8_t i = 0; i < fb->height; i++)
    {
        uint16_t x1 = beatsin8(PARAM_VAL(dna, P_SPEED), 0, fb->width - 1, 0, i * PARAM_VAL(dna, P_SIZE))
                + beatsin8(PARAM_VAL(dna, P_SPEED) - 7, 0, fb->width - 1, 0, i * PARAM_VAL(dna, P_SIZE) + 128);
        uint16_t x2 = beatsin8(PARAM_VAL(dna, P_SPEED), 0, fb->width - 1, 0, 128 + i * PARAM_VAL(dna, P_SIZE))
                + beatsin8(PARAM_VAL(dna, P_SPEED) - 7, 0, fb->width - 1, 0, 128 + 64 + i * PARAM_VAL(dna, P_SIZE));

        rgb_t color = hsv2rgb_rainbow(hsv_from_values(i * 128 / (fb->height - 1) + offset, 255, 255));

        if ((i + offset / 8) & 3)
            horizontal_line(fb, x1 / 2, x2 / 2, i, color, PARAM_VAL(dna, P_BORDER));
    }

    return fb_end(fb);
}

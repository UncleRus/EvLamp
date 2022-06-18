/**
 * Oldschool twister demo effect
 *
 * Author: Yaroslaw Turbin (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 *
 * https://wokwi.com/arduino/projects/297732081748804105
 *
 * Max supported framebuffer size is 256x256
 */
#include "effects/twister.h"

#include <lib8tion.h>
#include <esp_timer.h>
#include <math.h>
#include "palettes.h"

#define P_SPEED      0
#define P_BORDER     1
#define P_IRIDESCENT 2

EFFECT_PARAMS(twister, 3) = {
    DECL_PARAM_RANGE(P_SPEED, "Speed", 0, 40, 30),
    DECL_PARAM_BOOL(P_BORDER, "Add black border", 1),
    DECL_PARAM_BOOL(P_IRIDESCENT, "Iridescent colors", 0),
};

static const rgb_t c_black = C_BLACK;

static void horizontal_line(framebuffer_t *fb, uint8_t x1, uint8_t x2, uint8_t y, hsv_t color)
{
    uint8_t steps = abs8(x2 - x1) + 1;
    rgb_t rgb = hsv2rgb_rainbow(color);

    for (uint16_t i = 1; i <= steps; i++)
    {
        uint8_t dx = lerp8by8(x1, x2, i * 255 / steps);
        rgb_t pixel = rgb_scale_video(rgb, (steps - i) * 255 / steps);
        fb_set_pixel_rgb(fb, dx, y, pixel);
    }

    if (PARAM_VAL(twister, P_BORDER))
    {
        // add black points at the ends of line
        fb_set_pixel_rgb(fb, x1, y, c_black);
        fb_set_pixel_rgb(fb, x2, y, c_black);
    }
}

esp_err_t effect_twister_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    fb_clear(fb);

    uint16_t a = esp_timer_get_time() / 1000 / (41 - PARAM_VAL(twister, P_SPEED));

    for (size_t i = 0; i < fb->height; i++)
    {
        uint8_t sin_offs = sin8(i * 8 / M_PI + cos8(a / 2 + i) / 4 + a / 3);

        uint8_t x1 = sin8(sin_offs + a) * fb->width / 255;
        uint8_t x2 = sin8(sin_offs + a + 64) * fb->width / 255;
        uint8_t x3 = sin8(sin_offs + a + 128) * fb->width / 255;
        uint8_t x4 = sin8(sin_offs + a + 192) * fb->width / 255;
        x1 = x1 >= fb->width ? fb->width - 1 : x1;
        x2 = x2 >= fb->width ? fb->width - 1 : x2;
        x3 = x3 >= fb->width ? fb->width - 1 : x3;
        x4 = x4 >= fb->width ? fb->width - 1 : x4;

        uint8_t hue = sin8(PARAM_VAL(twister, P_IRIDESCENT) ? a : a / 20);
        if (x1 < x2)
            horizontal_line(fb, x1, x2, i, hsv_from_values(hue, 255, 255));
        if (x2 < x3)
            horizontal_line(fb, x2, x3, i, hsv_from_values(hue + 64, 255, 255));
        if (x3 < x4)
            horizontal_line(fb, x3, x4, i, hsv_from_values(hue + 128, 255, 255));
        if (x4 < x1)
            horizontal_line(fb, x4, x1, i, hsv_from_values(hue + 192, 255, 255));
    }

    return fb_end(fb);
}

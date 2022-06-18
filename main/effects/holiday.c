/**
 * Holiday lights
 *
 * Author: stepko
 *
 * https://wokwi.com/arduino/projects/287698321325163021
 */
#include "effects/holiday.h"
#include <lib8tion.h>

#define P_MODE  0
#define P_SPEED 1

EFFECT_PARAMS(holiday, 2) = {
    DECL_PARAM_RANGE(P_MODE, "Mode", 0, 3, 0),
    DECL_PARAM_RANGE(P_SPEED, "Speed", 1, 20, 8),
};

static uint8_t hue = 0;

static void wave(framebuffer_t *fb)
{
    fb_fade(fb, 2);

    uint8_t scale = beatsin8(1, 1, 255, 0, 0);
    for (int i = 0; i < fb->width - 1; i++)
    {
        uint16_t fx = beatsin16(i * PARAM_VAL(holiday, P_SPEED), i * 5, ((fb->width - 1) * 10 - 2) - (i * 5 + 2), 0, 0);
        fb_set_pixelf_hsv(fb, (float)fx / 10, i, hsv_from_values(hue + i * scale, 255, 100));
    }
}

static void radar(framebuffer_t *fb)
{
    fb_fade(fb, 2);

    uint8_t scale = beatsin8(1, 1, 255, 0, 0);
    for (int i = 0; i < fb->width - 1; i++)
    {
        uint16_t fx = beatsin16(PARAM_VAL(holiday, P_SPEED) * 5, i * 2.5, (fb->width - 1) * 5 - i * 2.5, 0, 0);
        fb_set_pixelf_hsv(fb, (float)fx / 5, i, hsv_from_values(hue + i * scale, 255, 255));
    }
}

static void sparks(framebuffer_t *fb)
{
    fb_fade(fb, 3);
    fb_blur2d(fb, 20);

    for (int i = 0; i < fb->width - 1; i++)
    {
        uint8_t fx = random8_between(i * 2, (fb->width - 1) * 4 - i * 2);
        rgb_t c;
        fb_get_pixel_rgb(fb, fx / 4, i, &c);
        if (rgb_is_zero(c))
            fb_set_pixelf_hsv(fb, (float)fx / 4, i, hsv_from_values(random8(), 255, 255));
    }
}

static void tree(framebuffer_t *fb)
{
    fb_fade(fb, 5);

    for (int i = 0; i < fb->width - 1; i++)
    {
        uint16_t fx = beatsin16(i * PARAM_VAL(holiday, P_SPEED), i * 2, ((fb->width - 1) * 4 - 2) - (i * 2 + 2), 0, 0);
        fb_set_pixelf_hsv(fb, (float)fx / 4, i, random8_to(10) == 0
                ? hsv_from_values(random8(), random8_between(32, 255), 255)
                : hsv_from_values(100, 255, 100));
    }
}

esp_err_t effect_holiday_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    hue++;
    switch (PARAM_VAL(holiday, P_MODE))
    {
        case HOLIDAY_RADAR:
            radar(fb);
            break;
        case HOLIDAY_SPARKS:
            sparks(fb);
            break;
        case HOLIDAY_TREE:
            tree(fb);
            break;
        default:
            wave(fb);
    }

    return fb_end(fb);
}

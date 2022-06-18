/**
 * Color hoop
 *
 * https://wokwi.com/arduino/projects/297414039372300810
 *
 * max framebuffer size 256x256
 */
#include "effects/hoop.h"

#include <noise.h>
#include <lib8tion.h>

#define P_SPEED 0

EFFECT_PARAMS(hoop, 1) = {
    DECL_PARAM_RANGE(P_SPEED, "Speed", 1,  50,  15),
};

static uint32_t m = 0;

esp_err_t effect_hoop_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    m += PARAM_VAL(hoop, P_SPEED);
    uint8_t t = m / 15;
    uint8_t u = t * 2;

    uint8_t div_x = 256 / fb->width;
    uint8_t div_y = 256 / fb->height;
    if (!div_x) div_x = 1;
    if (!div_y) div_y = 1;

    for (size_t i = 0; i < fb->width / 4 * 3; i++)
        fb_set_pixel_hsv(fb, sin8(t + i * 20) / div_x, sin8(u + i * 20) / div_y, hsv_from_values(i * 19, 255, 255));

    fb_blur2d(fb, 32);

    return fb_end(fb);
}

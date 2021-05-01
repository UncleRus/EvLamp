/**
 * Color hoop
 *
 * https://wokwi.com/arduino/projects/297414039372300810
 */
#include "effects/hoop.h"

#include <noise.h>
#include <lib8tion.h>

#define P_SPEED 0

EFFECT_PARAMS(hoop, 1) = {
    DECL_PARAM(P_SPEED, "Speed", 1,  50,  15),
};

static uint32_t m = 0;

esp_err_t effect_hoop_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    m += EPARAM(hoop, P_SPEED);
    uint8_t t = m / 15;
    uint8_t u = t * 2;

    for (size_t i = 0; i < fb->width / 4 * 3; i++)
        fb_set_pixel_hsv(fb, sin8(t + i * 20) >> 4, sin8(u + i * 20) >> 4, hsv_from_values(i * 19, 255, 255));

    fb_blur2d(fb, 32);

    return fb_end(fb);
}

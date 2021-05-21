/**
 * Light soft color change
 *
 * Author: Julia Uss
 */
#include "effects/relax.h"

#include <lib8tion.h> // Надо ли?

#define P_SPEED 0

EFFECT_PARAMS(relax, 1) = {
    DECL_PARAM(P_SPEED, "Speed", 1,  100,  15),
};

static uint8_t hue;
static uint16_t step;

esp_err_t effect_relax_prepare(framebuffer_t *fb);
{
    step = 1200 - 10 * PARAM(relax, P_SPEED);
    hue++;
    for (size_t x = 0; x < fb->width; x++)
        for (size_t y = 0; y < fb->height; y++)
        {
            fb_set_pixel_hsv(fb, x, y, hsv_from_values(hue, 255, 255));
        }
    return ESP_OK;
}

esp_err_t effect_relax_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    if (!(fb->frame_num % step)) hue++;

    uint16_t x = random16_to(fb->width);
    uint16_t y = random16_to(fb->height);

    fb_set_pixel_hsv(fb, x, y, hsv_from_values(hue, 255, 255));

    return fb_end(fb);
}

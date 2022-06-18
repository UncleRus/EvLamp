/**
 * Light soft color change
 *
 * Author: Julia Uss
 */
#include "effects/relax.h"

#include <lib8tion.h>

#define P_SPEED  0
#define P_HUE    1
#define P_SAT    2
#define P_PIXELS 3

EFFECT_PARAMS(relax, 4) = {
    DECL_PARAM_RANGE(P_SPEED, "Speed", 1, 100, 15),
    DECL_PARAM_RANGE(P_HUE, "Start hue", 0, 255, 100),
    DECL_PARAM_RANGE(P_SAT, "Saturation", 0, 255, 255),
    DECL_PARAM_RANGE(P_PIXELS, "Pixels to update", 1, 50, 1),
};

static uint8_t hue = 0;
static uint16_t step = 0;
static bool reset = false;

esp_err_t effect_relax_prepare(framebuffer_t *fb)
{
    step = 1200 - 10 * PARAM_VAL(relax, P_SPEED);
    reset = true;
    hue = PARAM_VAL(relax, P_HUE);
    return ESP_OK;
}

esp_err_t effect_relax_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    rgb_t c = hsv2rgb_rainbow(hsv_from_values(hue, PARAM_VAL(relax, P_SAT), 255));
    if (reset)
    {
        for (size_t i = 0; i < fb->width * fb->height; i++)
            fb->data[i] = c;
        reset = false;
    }
    else
    {
        if (!(fb->frame_num % step)) hue++;

        for (uint8_t i = 0; i < PARAM_VAL(relax, P_PIXELS); i++)
        {
            uint16_t x = random16_to(fb->width);
            uint16_t y = random16_to(fb->height);

            fb_set_pixel_rgb(fb, x, y, c);
        }
    }

    return fb_end(fb);
}

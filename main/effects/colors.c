/**
 * Blending colors
 */
#include "effects/colors.h"

#include <lib8tion.h>

#define P_SPEED   0
#define P_MIN_SAT 1
#define P_MAX_SAT 2

EFFECT_PARAMS(colors, 3) = {
    DECL_PARAM(P_SPEED, "Speed", 128, 254, 192),
    DECL_PARAM(P_MIN_SAT, "Minimal saturation", 1, 255, 128),
    DECL_PARAM(P_MAX_SAT, "Maximal saturation", 1, 255, 255),
};

static rgb_t prev_color, next_color;
static uint8_t step = 0;

esp_err_t effect_colors_prepare(framebuffer_t *fb)
{
    prev_color = hsv2rgb_rainbow(hsv_from_values(
        random8(), random8_between(PARAM_VAL(colors, P_MIN_SAT), PARAM_VAL(colors, P_MAX_SAT)), 255
    ));
    next_color = hsv2rgb_rainbow(hsv_from_values(
        random8(), random8_between(PARAM_VAL(colors, P_MIN_SAT), PARAM_VAL(colors, P_MAX_SAT)), 255
    ));
    step = 0;

    return ESP_OK;
}

esp_err_t effect_colors_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    rgb_t c = rgb_blend(prev_color, next_color, step);
    for (size_t i = 0; i < fb->width * fb->height; i++)
        fb->data[i] = c;

    if (!(fb->frame_num % (255 - PARAM_VAL(colors, P_SPEED))))
    {
        step = qadd8(step, 1);
        if (step == 255)
        {
            prev_color = next_color;
            next_color = hsv2rgb_rainbow(hsv_from_values(
                random8(), random8_between(PARAM_VAL(colors, P_MIN_SAT), PARAM_VAL(colors, P_MAX_SAT)), 255
            ));
            step = 0;
        }
    }

    return fb_end(fb);
}

/**
 * Simple rainbow effect
 */
#include "effects/rainbow.h"

#include <lib8tion.h>

#define P_DIRECTION 0
#define P_SCALE     1
#define P_SPEED     2

EFFECT_PARAMS(rainbow, 3) = {
    DECL_PARAM_RANGE(P_DIRECTION, "Direction", 0, 2, 0),
    DECL_PARAM_RANGE(P_SCALE, "Scale", 1, 50, 20),
    DECL_PARAM_RANGE(P_SPEED, "Speed", 1, 40, 5),
};

enum {
    RAINBOW_HORIZONTAL = 0,
    RAINBOW_VERTICAL,
    RAINBOW_DIAGONAL,
};

esp_err_t effect_rainbow_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    if (PARAM_VAL(rainbow, P_DIRECTION) == RAINBOW_DIAGONAL)
    {
        for (size_t x = 0; x < fb->width; x++)
            for (size_t y = 0; y < fb->height; y++)
            {
                float twirl = 3.0f * PARAM_VAL(rainbow, P_SCALE) / 100.0f;
                hsv_t color = {
                    .hue = fb->frame_num / 2 * PARAM_VAL(rainbow, P_SPEED)
                        + (fb->width / fb->height * x + y * twirl) * PARAM_VAL(rainbow, P_SCALE),
                    .sat = 255,
                    .val = 255
                };
                fb_set_pixel_hsv(fb, x, y, color);
            }
    }
    else
    {
        size_t outer = PARAM_VAL(rainbow, P_DIRECTION) == RAINBOW_HORIZONTAL ? fb->width : fb->height;
        size_t inner = PARAM_VAL(rainbow, P_DIRECTION) == RAINBOW_HORIZONTAL ? fb->height : fb->width;

        for (size_t i = 0; i < outer; i++)
        {
            hsv_t color = {
                .hue = fb->frame_num / 2 * PARAM_VAL(rainbow, P_SPEED) + i * PARAM_VAL(rainbow, P_SCALE),
                .sat = 255,
                .val = 255
            };
            for (size_t j = 0; j < inner; j++)
                if (PARAM_VAL(rainbow, P_DIRECTION) == RAINBOW_HORIZONTAL)
                    fb_set_pixel_hsv(fb, i, j, color);
                else
                    fb_set_pixel_hsv(fb, j, i, color);
        }
    }

    return fb_end(fb);
}

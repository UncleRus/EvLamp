/**
 * Colored sparkles effect
 */
#include "effects/sparkles.h"

#include <lib8tion.h>

#define P_COUNT   0
#define P_FADEOUT 1

EFFECT_PARAMS(sparkles, 2) = {
    DECL_PARAM_RANGE(P_COUNT, "Number of sparkles", 1, 50, 10),
    DECL_PARAM_RANGE(P_FADEOUT, "Fadeout speed", 10, 150, 80),
};

esp_err_t effect_sparkles_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    fb_blur2d(fb, 8);
    for (uint8_t i = 0; i < PARAM_VAL(sparkles, P_COUNT); i++)
    {
        uint16_t x = random16_to(fb->width);
        uint16_t y = random16_to(fb->height);

        rgb_t c;
        fb_get_pixel_rgb(fb, x, y, &c);
        if (rgb_luma(c) < 5)
            fb_set_pixel_hsv(fb, x, y, hsv_from_values(random8(), 255, 255));
    }
    fb_fade(fb, PARAM_VAL(sparkles, P_FADEOUT));

    return fb_end(fb);
}

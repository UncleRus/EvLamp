/**
 * Rain effect by Shaitan
 */
#include "effects/rain.h"

#include <lib8tion.h>

#define MODE_SINGLE_COLOR 0

#define P_MODE    0
#define P_HUE     1
#define P_DENSITY 2
#define P_TAIL    3

EFFECT_PARAMS(rain, 4) = {
    DECL_PARAM_BOOL(P_MODE, "Colored", 0),
    DECL_PARAM_RANGE(P_HUE, "Hue", 0, 255, HUE_AQUA),
    DECL_PARAM_RANGE(P_DENSITY, "Density", 0, 100, 50),
    DECL_PARAM_RANGE(P_TAIL, "Tail length", 100, 200, 150),
};

static uint8_t density;

esp_err_t effect_rain_prepare(framebuffer_t *fb)
{
    density = 255 - qadd8(PARAM_VAL(rain, P_DENSITY), 155);
    return ESP_OK;
}

esp_err_t effect_rain_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    for (size_t x = 0; x < fb->width; x++)
    {
        rgb_t c;
        fb_get_pixel_rgb(fb, x, fb->height - 1, &c);
        if (!rgb_luma(c) && random8_to(density) == 0)
            fb_set_pixel_hsv(fb, x, fb->height - 1,
                    hsv_from_values(PARAM_VAL(rain, P_MODE) == MODE_SINGLE_COLOR ? PARAM_VAL(rain, P_HUE) : random8(), 255, 255));
        else
        {
            c = rgb_scale(c, PARAM_VAL(rain, P_TAIL) + random8_to(100) - 50);
            fb_set_pixel_rgb(fb, x, fb->height - 1, rgb_luma(c) < 3 ? rgb_from_values(0, 0, 0) : c);
        }
    }
    fb_shift(fb, 1, FB_SHIFT_DOWN);

    return fb_end(fb);
}

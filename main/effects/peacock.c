/**
 * Peacock (sin plasma) effect
 *
 * Author: Yaroslaw Turbin (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 */
#include "effects/peacock.h"

#include <lib8tion.h>

#define P_SPEED      0
#define P_SCALE      1
#define P_SATURATION 2

EFFECT_PARAMS(peacock, 3) = {
    DECL_PARAM_RANGE(P_SPEED, "Speed", 1, 255, 200),
    DECL_PARAM_RANGE(P_SCALE, "Scale", 1, 10, 4),
    DECL_PARAM_RANGE(P_SATURATION, "Saturation", 0, 255, 255),
};

esp_err_t effect_peacock_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    float t = (float)(esp_timer_get_time() / 1000) / (1000.0f - PARAM_VAL(peacock, P_SPEED) * 4);

    for (size_t x = 0; x < fb->width; x++)
        for (size_t y = 0; y < fb->height; y++)
        {
            uint8_t hue = sin8(x * 8 + sin8(x * PARAM_VAL(peacock, P_SCALE) + t * 6)) / 2
                        + sin8(y * 8 + sin8(y * PARAM_VAL(peacock, P_SCALE) + t * 7) / 2);
            fb_set_pixel_hsv(fb, x, y, hsv_from_values(hue, PARAM_VAL(peacock, P_SATURATION), 255));
        }

    return fb_end(fb);
}

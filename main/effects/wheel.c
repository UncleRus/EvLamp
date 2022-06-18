/**
 * Radiant wheel effect
 */
#include <lib8tion.h>
#include <math.h>
#include "wheel.h"

#define P_SPEED 0
#define P_HUE1  1
#define P_SAT1  2
#define P_HUE2  3
#define P_SAT2  4
#define P_RAYS  5

EFFECT_PARAMS(wheel, 6) = {
    DECL_PARAM_RANGE(P_SPEED, "Rotation speed", 0, 255, 60),
    DECL_PARAM_RANGE(P_HUE1, "Hue 1", 0, 255, 0),
    DECL_PARAM_RANGE(P_SAT1, "Saturation 1", 0, 255, 255),
    DECL_PARAM_RANGE(P_HUE2, "Hue 2", 0, 255, 150),
    DECL_PARAM_RANGE(P_SAT2, "Saturation 2", 0, 255, 0),
    DECL_PARAM_RANGE(P_RAYS, "Number of rays", 2, 6, 3),
};

#define BLACK_TH 40
#define BRIGHT_TH 200

esp_err_t effect_wheel_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    float t = (float)esp_timer_get_time() / 1000000.0f;

    size_t cx = fb->width / 2;
    size_t cy = fb->height / 2;
    float speed = (float)(PARAM_VAL(wheel, P_SPEED)) / 20.0f;

    float offs_y = sinf(t / 2) * fb->height / 3;
    float offs_x = sinf(t / 3) * fb->width / 3;

    for (int x = 0; x < fb->width; x++)
        for (int y = 0; y < fb->height; y++)
        {
            int16_t v = (int16_t)(sinf(PARAM_VAL(wheel, P_RAYS) * atan2f((float)x - cx + offs_x, (float)y - cy + offs_y) + t * speed) * 255);

            hsv_t color;
            color.val = abs(v);
            color.val = color.val > BRIGHT_TH ? color.val : color.val / 2;
            color.val = color.val < BLACK_TH ? 0 : color.val;
            if (v > 0)
            {
                color.hue = PARAM_VAL(wheel, P_HUE1);
                color.sat = PARAM_VAL(wheel, P_SAT1);
            }
            else
            {
                color.hue = PARAM_VAL(wheel, P_HUE2);
                color.sat = PARAM_VAL(wheel, P_SAT2);
            }
            fb_set_pixel_hsv(fb, x, fb->height - 1 - y, color);
        }

    return fb_end(fb);
}

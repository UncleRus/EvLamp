/**
 * Flower effect
 *
 * Author: Yaroslaw Turbin (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 */
#include "effects/flower.h"

#include <lib8tion.h>
#include <math.h>

#define P_SPEED 0
#define P_DIAMETER 1

EFFECT_PARAMS(flower, 2) = {
    DECL_PARAM(P_SPEED, "Speed", 1, 255, 100),
    DECL_PARAM(P_DIAMETER, "Diameter", 50, 150, 128),
};

static uint32_t last_update_ms = 0;
static float counter = 0;
static float angle = 1;

esp_err_t effect_flower_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    uint32_t time_ms = esp_timer_get_time() / 1000;
    if ((time_ms - last_update_ms) > 80000 / EPARAM(flower, P_SPEED))
    {
        last_update_ms = time_ms;
        angle += 0.1;
    }

    float n = triwave8(counter) * 2.2;
    float a = n * angle;
    float r = EPARAM(flower, P_DIAMETER) / 255.0 * sqrt16(n);
    float x = r * cosf(a) + (float)fb->width / 2.0;
    float y = r * sinf(a) + (float)fb->height / 2.0;

    fb_set_pixelf_hsv(fb, x, y, hsv_from_values(time_ms >> 1, 250, 255));

    fb_fade(fb, EPARAM(flower, P_SPEED) / 10);
    counter += EPARAM(flower, P_SPEED) / 100.0;

    return fb_end(fb);
}

/**
 * Colored rays effect
 *
 * Author: Yaroslaw Turbin (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 * https://editor.soulmatelights.com/gallery/819-colored-bursts
 *
 * Max supported framebuffer size is 256x256
 */
#include "effects/rays.h"

#include <lib8tion.h>

#define P_SPEED    0
#define P_MIN_RAYS 1
#define P_MAX_RAYS 2

EFFECT_PARAMS(rays, 3) = {
    DECL_PARAM_RANGE(P_SPEED, "Speed", 0, 50, 10),
    DECL_PARAM_RANGE(P_MIN_RAYS, "Min rays", 1, 10, 3),
    DECL_PARAM_RANGE(P_MAX_RAYS, "Max rays", 1, 20, 6),
};

static uint8_t hue;
static uint8_t num_rays;

static void line(framebuffer_t *fb, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, rgb_t color)
{
    uint8_t x_steps = abs8((int8_t)(x1 - x2)) + 1;
    uint8_t y_steps = abs8((int8_t)(y1 - y2)) + 1;
    uint8_t steps = x_steps >= y_steps ? x_steps : y_steps;

    for (uint8_t i = 1; i <= steps; i++)
    {
        uint8_t dx = lerp8by8(x1, x2, i * 255 / steps);
        uint8_t dy = lerp8by8(y1, y2, i * 255 / steps);

        rgb_t pixel;
        fb_get_pixel_rgb(fb, dx, dy, &pixel);
        fb_set_pixel_rgb(fb, dx, dy, rgb_scale_video(rgb_add_rgb(pixel, color), i * 255 / steps));
    }
}

esp_err_t effect_rays_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    // change number of rays
    if (PARAM_VAL(rays, P_MAX_RAYS) > PARAM_VAL(rays, P_MIN_RAYS) && !(fb->frame_num % 10))
    {
        if (random8_to(2))
            num_rays++;
        else
            num_rays--;
    }
    if (num_rays < PARAM_VAL(rays, P_MIN_RAYS)) num_rays = PARAM_VAL(rays, P_MIN_RAYS);
    if (num_rays > PARAM_VAL(rays, P_MAX_RAYS)) num_rays = PARAM_VAL(rays, P_MAX_RAYS);

    hue += 5;
    fb_fade(fb, 40);
    for (uint8_t i = 0; i < num_rays; i++)
    {
        uint8_t x1 = beatsin8(4 + PARAM_VAL(rays, P_SPEED), 0, (fb->width - 1), 0, 0);
        uint8_t y1 = beatsin8(2 + PARAM_VAL(rays, P_SPEED), 0, (fb->height - 1), 0, 0);
        uint8_t x2 = beatsin8(8 + PARAM_VAL(rays, P_SPEED), 0, (fb->width - 1), 0, i * 24);
        uint8_t y2 = beatsin8(10 + PARAM_VAL(rays, P_SPEED), 0, (fb->height - 1), 0, i * 48 + 64);

        line(fb, x1, y1, x2, y2, hsv2rgb_rainbow(hsv_from_values(i * 255 / num_rays + hue, 255, 255)));
    }
    fb_blur2d(fb, 8);

    return fb_end(fb);
}

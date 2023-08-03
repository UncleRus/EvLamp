/**
 * "Nuclear Noise"
 *
 *  Author: Stepko
 *  https://editor.soulmatelights.com/gallery/1569-radialnuclearnoise
 *
 *  Note: only even-sized framebuffers are supported
 */
#include "effects/nuclear.h"

#include <lib8tion.h>
#include <stdlib.h>
#include <math.h>
#include <noise.h>

#define P_SPEED 0

EFFECT_PARAMS(nuclear, 1) = {
    DECL_PARAM_RANGE(P_SPEED, "Speed", 1, 10, 3),
};

typedef struct {
    uint8_t angle;
    uint8_t radius;
} map_item_t;

#define MAP_XY(x, y) (map[((y) * fb->width + (x))])

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

static map_item_t *map = NULL;
static float mmap = 0.0f;
static uint16_t t = 0;

esp_err_t effect_nuclear_prepare(framebuffer_t *fb)
{
    CHECK_ARG(fb);

    if (!map) map = calloc(fb->width * fb->height, sizeof(map_item_t));
    if (!map) return ESP_ERR_NO_MEM;

    int center_x = (int)fb->width / 2;
    int center_y = (int)fb->height / 2;

    mmap = 255.0f / MAX(fb->width, fb->height);

    for (int x = -center_x; x < center_x; x++)
        for (int y = -center_y; y < center_y; y++)
        {
            float xf = (float)x;
            float yf = (float)y;
            MAP_XY(x + center_x, y + center_y).angle = (uint8_t)(128 * (atan2f(yf, xf) / (float)M_PI));
            MAP_XY(x + center_x, y + center_y).radius = (uint8_t)(hypotf(xf, yf) * mmap);
        }

    t = 0;

    return ESP_OK;
}

esp_err_t effect_nuclear_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    t += PARAM_VAL(nuclear, P_SPEED);

    uint16_t t1 = t / 2;
    for (size_t x = 0; x < fb->width; x++)
        for (size_t y = 0; y < fb->height; y++)
        {
            map_item_t i = MAP_XY(x, y);
            uint8_t angle = sin8(t1 / 2 + i.angle * 3);
            uint8_t radius = (i.radius) * 2 - t;

            uint8_t noise[3] = {
                inoise8_3d(angle, radius, t1),
                inoise8_3d(angle, 12032 + t1, radius),
                inoise8_3d(radius, 120021 + t1, angle)
            };
            for (uint8_t c = 0; c < 3; c++)
                noise[c] = noise[c] < 128
                    ? 0
                    : constrain((noise[c] - 128) * 3, 0, 255);
            fb_set_pixel_rgb(fb, x, y, rgb_from_values(noise[0], noise[1], noise[2]));
        }

    return fb_end(fb);
}

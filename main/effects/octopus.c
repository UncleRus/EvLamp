/**
 * "Octopus"
 *
 *  Author: Stepko
 *  https://editor.soulmatelights.com/gallery/671-octopus
 *
 *  Note: only even-sized framebuffers are supported
 */
#include "effects/octopus.h"

#include <lib8tion.h>
#include <stdlib.h>
#include <math.h>

#define P_SPEED 0
#define P_LEGS 1
#define P_CLOCKWISE 2

EFFECT_PARAMS(octopus, 3) = {
    DECL_PARAM_RANGE(P_SPEED, "Speed", 1, 10, 3),
    DECL_PARAM_RANGE(P_LEGS, "Legs", 2, 10, 3),
    DECL_PARAM_BOOL(P_CLOCKWISE, "Clockwise", 0),
};

typedef struct {
    uint8_t angle;
    uint8_t radius;
} map_item_t;

#define MAP_XY(x, y) (map[((y) * fb->width + (x))])

static map_item_t *map = NULL;
static float mmap = 0.0f;
static uint16_t t = 0;

esp_err_t effect_octopus_prepare(framebuffer_t *fb)
{
    CHECK_ARG(fb);

    if (fb->width % 2 || fb->height % 2)
    {
        ESP_LOGE("octopus", "Only even-sized framebuffers are supported");
        return ESP_FAIL;
    }

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

esp_err_t effect_octopus_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    t += PARAM_VAL(octopus, P_SPEED);

    for (size_t x = 0; x < fb->width; x++)
        for (size_t y = 0; y < fb->height; y++)
        {
            size_t px = !PARAM_VAL(octopus, P_CLOCKWISE) ? x : fb->width - x - 1;
            map_item_t i = MAP_XY(x, y);
            fb_set_pixel_hsv(fb, px, y,
                hsv_from_values(
                    t / 2 - i.radius,
                    255,
                    sin8(sin8((i.angle * 4 - i.radius) / 4 + t) + i.radius - t * 2 + i.angle * PARAM_VAL(octopus, P_LEGS))
                )
            );
        }

    return fb_end(fb);
}

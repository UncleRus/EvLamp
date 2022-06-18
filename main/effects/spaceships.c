/**
 * Flight of spaceships effect
 *
 * Author: stepko
 * https://wokwi.com/projects/289609636203463176
 */
#include "effects/spaceships.h"

#include <lib8tion.h>

#define P_COUNT 0
#define P_DIRECTION_TIME 1

EFFECT_PARAMS(spaceships, 2) = {
    DECL_PARAM_RANGE(P_COUNT, "Number of ships", 2, 16, 8),
    DECL_PARAM_RANGE(P_DIRECTION_TIME, "Direction change time", 1, 60, 5),
};

typedef enum {
    DIR_UP = 0,
    DIR_UP_RIGHT,
    DIR_RIGHT,
    DIR_DOWN_RIGHT,
    DIR_DOWN,
    DIR_DOWN_LEFT,
    DIR_LEFT,
    DIR_UP_LEFT,

    DIR_MAX
} direction_t;

static direction_t dir = DIR_UP_RIGHT;
static uint32_t last_change = 0;

esp_err_t effect_spaceships_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    uint32_t t = esp_timer_get_time() / 1000000; // current time in seconds
    if (!last_change) last_change = t; // seconds

    // shift framebuffer in opposite direction
    switch (dir)
    {
        case DIR_UP:
            fb_shift(fb, 1, FB_SHIFT_DOWN);
            break;
        case DIR_UP_RIGHT:
            fb_shift(fb, 1, FB_SHIFT_DOWN);
            fb_shift(fb, 1, FB_SHIFT_LEFT);
            break;
        case DIR_RIGHT:
            fb_shift(fb, 1, FB_SHIFT_LEFT);
            break;
        case DIR_DOWN_RIGHT:
            fb_shift(fb, 1, FB_SHIFT_UP);
            fb_shift(fb, 1, FB_SHIFT_LEFT);
            break;
        case DIR_DOWN:
            fb_shift(fb, 1, FB_SHIFT_UP);
            break;
        case DIR_DOWN_LEFT:
            fb_shift(fb, 1, FB_SHIFT_UP);
            fb_shift(fb, 1, FB_SHIFT_RIGHT);
            break;
        case DIR_LEFT:
            fb_shift(fb, 1, FB_SHIFT_RIGHT);
            break;
        case DIR_UP_LEFT:
            fb_shift(fb, 1, FB_SHIFT_DOWN);
            fb_shift(fb, 1, FB_SHIFT_RIGHT);
            break;
        default:
            break;
    }

    for (uint8_t i = 0; i < PARAM_VAL(spaceships, P_COUNT); i++)
        fb_set_pixelf_hsv(fb,
            beatsin8(12 + i, 2, fb->width - 3, 0, 0),
            beatsin8(15 + i, 2, fb->height - 3, 0, 0),
            hsv_from_values(beatsin8(12 + i, 0, 255, 0, 0), 255, 255));
    fb_blur2d(fb, 32);

    if (t - last_change >= PARAM_VAL(spaceships, P_DIRECTION_TIME))
    {
        last_change = t;
        dir += random8_to(2) ? 1 : -1;
        if (dir >= DIR_MAX)
            dir = DIR_UP;
    }

    return fb_end(fb);
}

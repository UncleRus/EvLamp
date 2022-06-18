/**
 * Diagonal movements effect by SottNick
 */
#include "effects/diagonals.h"

#include <lib8tion.h>

#define MAX_SPEED 10
#define CHANGE_DIR 3

#define P_SPEED 0

EFFECT_PARAMS(diagonals, 1) = {
    DECL_PARAM_RANGE(P_SPEED, "Speed", 1, MAX_SPEED, MAX_SPEED / 2),
};

static uint8_t slowdown;
static uint8_t pixels;
static int8_t dx = 1;
static int8_t dy = 1;
static size_t x = 0;
static size_t y = 0;
static uint8_t hue = 0;

esp_err_t effect_diagonals_prepare(framebuffer_t *fb)
{
    int r = PARAM_VAL(diagonals, P_SPEED) - MAX_SPEED / 2;

    slowdown = r < 0 ? -r + 1 : 1;
    pixels   = r > 0 ? r + 1 : 1;

    return ESP_OK;
}

esp_err_t effect_diagonals_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    if (!(fb->frame_num % slowdown))
    {
        for (uint8_t i = 0; i < pixels; i++)
        {
            fb_fade(fb, 2);

            if (random8_to(CHANGE_DIR)) dx = (int8_t) (-dx);
            if (random8_to(CHANGE_DIR)) dy = (int8_t) (-dy);
            x = (fb->width + x + dx) % fb->width;
            y = (fb->height + y + dy) % fb->height;

            fb_set_pixel_hsv(fb, x, y, hsv_from_values(hue++, 255, 255));
        }
    }

    return fb_end(fb);
}

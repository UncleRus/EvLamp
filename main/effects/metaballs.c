/**
 * Metaballs effect
 *
 * Author: Yaroslaw Turbin 02.09.2020 (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 *
 * https://wokwi.com/arduino/libraries/FastLED/Metaballs
 */
#include "effects/metaballs.h"
#include "palettes.h"

#include <lib8tion.h>

#define MAX_BALLS 6
#define MAX_PALETTES 3

#define P_PALETTE 0
#define P_COUNT   1

EFFECT_PARAMS(metaballs, 2) = {
    DECL_PARAM_RANGE(P_PALETTE, "Palette", 0, MAX_PALETTES - 1, 0),
    DECL_PARAM_RANGE(P_COUNT, "Balls", 2, MAX_BALLS, 3),
};

enum {
    PAL_FIRE = 0,
    PAL_BLUE,
    PAL_GREEN,
};

#define PALETTE_SIZE 6

static const rgb_t _palettes[MAX_PALETTES][PALETTE_SIZE] = {
    [PAL_FIRE] =  { C_BLACK, C_BLACK, C_RED, C_YELLOW, C_YELLOW, C_WHITE },
    [PAL_BLUE] =  { C_BLACK, C_BLACK, C_DARK_BLUE, C_CYAN, C_CYAN, C_WHITE },
    [PAL_GREEN] = { C_BLACK, C_BLACK, C_DARK_GREEN, C_SPRING_GREEN, C_SPRING_GREEN, C_WHITE },
};

typedef struct {
    uint8_t bpm_x, bpm_y;
    uint8_t phase;
    uint8_t x, y;
} ball_t;

ball_t balls[MAX_BALLS];

static uint8_t dist(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    int a = y2 - y1;
    int b = x2 - x1;
    uint16_t s = sqrt16(a * a + b * b);
    return s ? 255 / s : 255;
}

esp_err_t effect_metaballs_prepare(framebuffer_t *fb)
{
    for (size_t i = 0; i < PARAM_VAL(metaballs, P_COUNT); i++)
    {
        balls[i].bpm_x = random8_between(10, 40);
        balls[i].bpm_y = random8_between(10, 40);
        balls[i].phase = 256 / MAX_BALLS * i;
    }

    return ESP_OK;
}

esp_err_t effect_metaballs_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    for (size_t i = 0; i < PARAM_VAL(metaballs, P_COUNT); i++)
    {
        balls[i].x = beatsin8(balls[i].bpm_x, 0, fb->width - 1, 0, balls[i].phase);
        balls[i].y = beatsin8(balls[i].bpm_y, 0, fb->height - 1, 0, balls[i].phase);
    }

    for (int i = 0; i < fb->width; i++)
    {
        for (int j = 0; j < fb->height; j++)
        {
            uint8_t sum = 0;
            for (size_t b = 0; b < PARAM_VAL(metaballs, P_COUNT); b++)
                sum = qadd8(sum, dist(i, j, balls[b].x, balls[b].y));

            fb_set_pixel_rgb(fb, i, j, color_from_palette_rgb(_palettes[PARAM_VAL(metaballs, P_PALETTE)], PALETTE_SIZE, qsub8(sum, 32), 255, true));
        }
    }

    fb_blur2d(fb, 32);

    return fb_end(fb);
}

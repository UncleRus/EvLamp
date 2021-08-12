/**
 * Metaballs effect
 *
 * Author: Yaroslaw Turbin 02.09.2020 (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 *
 * https://wokwi.com/arduino/libraries/FastLED/Metaballs
 */
#include "effects/metaballs.h"

#include <lib8tion.h>

#define MAX_BALLS 6

#define P_PALETTE 0
#define P_COUNT   1

EFFECT_PARAMS(metaballs, 2) = {
    DECL_PARAM(P_PALETTE, "Palette", 0, 2, 0),
    DECL_PARAM(P_COUNT, "Balls", 2, MAX_BALLS, 3),
};

#define PALETTE_FIRE  0
#define PALETTE_BLUE  1
#define PALETTE_GREEN 2

#define PALETTE_SIZE 6

static rgb_t palette[PALETTE_SIZE];

static const rgb_t C_BLACK  = { 0 };
static const rgb_t C_WHITE  = { .r = 255, .g = 255, .b = 255 };
static const rgb_t C_DBLUE  = { .r = 0,   .g = 0,   .b = 100 };
static const rgb_t C_CYAN   = { .r = 0,   .g = 200, .b = 255 };
static const rgb_t C_RED    = { .r = 255, .g = 0,   .b = 0 };
static const rgb_t C_YELLOW = { .r = 255, .g = 255, .b = 0 };
static const rgb_t C_DGREEN = { .r = 0,   .g = 100, .b = 0 };
static const rgb_t C_BGREEN = { .r = 155, .g = 255, .b = 155 };

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
    palette[0] = C_BLACK;
    palette[1] = C_BLACK;
    palette[5] = C_WHITE;
    switch (PARAM_VAL(metaballs, P_PALETTE))
    {
        case PALETTE_BLUE:
            palette[2] = C_DBLUE;
            palette[3] = C_CYAN;
            palette[4] = C_CYAN;
            break;
        case PALETTE_GREEN:
            palette[2] = C_DGREEN;
            palette[3] = C_BGREEN;
            palette[4] = C_BGREEN;
            break;
        default:
            palette[2] = C_RED;
            palette[3] = C_YELLOW;
            palette[4] = C_YELLOW;
    }

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

            //fb_set_pixel_rgb(fb, i, j, rgb_from_values(sum, sum, sum));
            fb_set_pixel_rgb(fb, i, j, color_from_palette_rgb(palette, PALETTE_SIZE, qsub8(sum, 32), 255, true));
        }
    }

    fb_blur2d(fb, 32);

    return fb_end(fb);
}

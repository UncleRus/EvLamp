/**
 * Metaballs effect
 *
 * Author: Yaroslaw Turbin 02.09.2020 (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 *
 * https://wokwi.com/arduino/libraries/FastLED/Metaballs
 */
#include "effects/metaballs.h"

#include <lib8tion.h>

#define P_PALETTE 0

EFFECT_PARAMS(metaballs, 1) = {
    DECL_PARAM(P_PALETTE, "Palette", 0, 2, 0),
};

#define PALETTE_FIRE  0
#define PALETTE_BLUE  1
#define PALETTE_GREEN 2

#define PALETTE_SIZE 5

static rgb_t palette[PALETTE_SIZE];

static const rgb_t C_BLACK  = { 0 };
static const rgb_t C_WHITE  = { .r = 255, .g = 255, .b = 255 };
static const rgb_t C_DBLUE  = { .r = 0,   .g = 0,   .b = 100 };
static const rgb_t C_CYAN   = { .r = 0,   .g = 200, .b = 255 };
static const rgb_t C_RED    = { .r = 255, .g = 0,   .b = 0 };
static const rgb_t C_YELLOW = { .r = 255, .g = 255, .b = 0 };
static const rgb_t C_DGREEN = { .r = 0,   .g = 100, .b = 0 };
static const rgb_t C_BGREEN = { .r = 155, .g = 255, .b = 155 };

static uint8_t dist(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    int a = y2 - y1;
    int b = x2 - x1;
    uint16_t s = sqrt16(a * a + b * b);
    return s ? 220 / s : 220;
}

esp_err_t effect_metaballs_prepare(framebuffer_t *fb)
{
    palette[0] = C_BLACK;
    palette[1] = C_BLACK;
    palette[4] = C_WHITE;
    switch (EPARAM(metaballs, P_PALETTE))
    {
        case PALETTE_BLUE:
            palette[2] = C_DBLUE;
            palette[3] = C_CYAN;
            break;
        case PALETTE_GREEN:
            palette[2] = C_DGREEN;
            palette[3] = C_BGREEN;
            break;
        default:
            palette[2] = C_RED;
            palette[3] = C_YELLOW;
    }

    return ESP_OK;
}

esp_err_t effect_metaballs_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    uint8_t bx1 = beatsin8(15, 0, fb->width - 1,  0, 0);
    uint8_t by1 = beatsin8(18, 0, fb->height - 1, 0, 0);
    uint8_t bx2 = beatsin8(28, 0, fb->width - 1,  0, 32);
    uint8_t by2 = beatsin8(23, 0, fb->height - 1, 0, 32);
    uint8_t bx3 = beatsin8(30, 0, fb->width - 1,  0, 64);
    uint8_t by3 = beatsin8(24, 0, fb->height - 1, 0, 64);
    uint8_t bx4 = beatsin8(17, 0, fb->width - 1,  0, 128);
    uint8_t by4 = beatsin8(25, 0, fb->height - 1, 0, 128);
    uint8_t bx5 = beatsin8(19, 0, fb->width - 1,  0, 170);
    uint8_t by5 = beatsin8(21, 0, fb->height - 1, 0, 170);

    for (int i = 0; i < fb->width; i++)
    {
        for (int j = 0; j < fb->height; j++)
        {

            uint8_t sum = dist(i, j, bx1, by1);
            sum = qadd8(sum, dist(i, j, bx2, by2));
            sum = qadd8(sum, dist(i, j, bx3, by3));
            sum = qadd8(sum, dist(i, j, bx4, by4));
            sum = qadd8(sum, dist(i, j, bx5, by5));

            fb_set_pixel_rgb(fb, i, j, color_from_palette_rgb(palette, PALETTE_SIZE, sum + 220, 255, true));
        }
    }

    fb_blur2d(fb, 32);

    return fb_end(fb);
}

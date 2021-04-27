/**
 * @file fire.c
 *
 * Fire effect based on Perlin noise
 *
 * Author: Yaroslaw Turbin (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 *
 * https://pastebin.com/jSSVSRi6
 */
#include "effects/fire.h"

#include <lib8tion.h>
#include <noise.h>

#define P_PALETTE 0

EFFECT_PARAMS(fire, 1) = {
    DECL_PARAM(P_PALETTE, "Palette", 0, 2, 0),
};

#define PALETTE_FIRE  0
#define PALETTE_BLUE  1
#define PALETTE_GREEN 2

#define PALETTE_SIZE 16

static rgb_t palette[PALETTE_SIZE];

static const rgb_t C_BLACK  = { 0 };
static const rgb_t C_WHITE  = { .r = 255, .g = 255, .b = 255 };
static const rgb_t C_DBLUE  = { .r = 0,   .g = 0,   .b = 100 };
static const rgb_t C_CYAN   = { .r = 0,   .g = 200, .b = 255 };
static const rgb_t C_RED    = { .r = 255, .g = 0,   .b = 0 };
static const rgb_t C_YELLOW = { .r = 255, .g = 255, .b = 0 };
static const rgb_t C_DGREEN = { .r = 0,   .g = 100, .b = 0 };
static const rgb_t C_BGREEN = { .r = 155, .g = 255, .b = 155 };

esp_err_t effect_fire_prepare(framebuffer_t *fb)
{
    switch (EPARAM(fire, P_PALETTE))
    {
        case PALETTE_BLUE:
            rgb_fill_gradient4_rgb(palette, PALETTE_SIZE, C_BLACK, C_DBLUE, C_CYAN, C_WHITE);
            break;
        case PALETTE_GREEN:
            rgb_fill_gradient4_rgb(palette, PALETTE_SIZE, C_BLACK, C_DGREEN, C_BGREEN, C_WHITE);
            break;
        default:
            rgb_fill_gradient4_rgb(palette, PALETTE_SIZE, C_BLACK, C_RED, C_YELLOW, C_WHITE);
    }

    return ESP_OK;
}

esp_err_t effect_fire_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    uint32_t time_ms = esp_timer_get_time() / 1000;

    for (size_t x = 0; x < fb->width; x++)
        for (size_t y = 0; y < fb->height; y++)
        {
            uint8_t idx = qsub8(inoise8_3d(x * 60, y * 60 + time_ms, time_ms / 3), abs8(y - (fb->height - 1)) * 255 / (fb->height - 1));
            rgb_t c = color_from_palette_rgb(palette, PALETTE_SIZE, idx, 255, true);
            fb_set_pixel_rgb(fb, x, fb->height - y - 1, c);
        }

    return fb_end(fb);
}

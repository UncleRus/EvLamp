/**
 * Polar lights effect
 *
 * Author: Kostyantyn Matviyevskyy kostyamat 05.02.2021
 *
 * https://editor.soulmatelights.com/gallery/762-polar-lights
 */
#include "effects/polar_lights.h"

#include <lib8tion.h>
#include <noise.h>
#include <math.h>

#define PALETTE_BLUE  0
#define PALETTE_GREEN 1

#define P_SPEED 0
#define P_SCALE 1
#define P_PALETTE 2
#define P_HEIGHT 3

EFFECT_PARAMS(polar_lights, 4) = {
    DECL_PARAM(P_SPEED, "Speed", 1, 255, 127),
    DECL_PARAM(P_SCALE, "Scale", 1, 255, 60),
    DECL_PARAM(P_PALETTE, "Palette", 0, 1, 0),
    DECL_PARAM(P_HEIGHT, "Height adjustment", 1, 255, 50),
};

#define PALETTE_SIZE 4

static rgb_t palette[PALETTE_SIZE];

static const rgb_t C_BLACK  = { 0 };
static const rgb_t C_BLUE   = { .r = 0,   .g = 0,   .b = 255 };
static const rgb_t C_RED    = { .r = 255, .g = 0,   .b = 0 };
static const rgb_t C_YELLOW = { .r = 255, .g = 255, .b = 0 };
static const rgb_t C_GREEN  = { .r = 0,   .g = 150, .b = 0 };

static float height_adj;
static uint16_t p_scale;
static uint8_t p_speed;
static uint32_t t = 0;

static int map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (out_max - out_min) * (x - in_min) / (in_max - in_min) + out_min;
}

esp_err_t effect_polar_lights_prepare(framebuffer_t *fb)
{
    palette[0] = C_BLACK;
    palette[2] = C_YELLOW;
    palette[3] = C_RED;
    switch (PARAM_VAL(polar_lights, P_PALETTE))
    {
        case PALETTE_BLUE:
            palette[1] = C_BLUE;
            break;
        default:
            palette[1] = C_GREEN;
    }

    height_adj = PARAM_VAL(polar_lights, P_HEIGHT) / 2.0f;
    p_scale = map(PARAM_VAL(polar_lights, P_SCALE), 1, 255, 30, map(fb->width, 8, 64, 310, 63));
    p_speed = map(PARAM_VAL(polar_lights, P_SPEED), 1, 255, 128, 16);
    return ESP_OK;
}

esp_err_t effect_polar_lights_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    for (size_t x = 0; x < fb->width; x++)
        for (size_t y = 0; y < fb->height; y++)
        {
            t++;
            rgb_t color = color_from_palette_rgb(palette, PALETTE_SIZE,
                    qsub8(inoise8_3d(t % 2 + x * p_scale, y * 16 + t % 16, t / p_speed),
                            fabsf((float)fb->height / 2 - y) * height_adj),
                    255, true);
            fb_set_pixel_rgb(fb, x, y, color);
        }

    return fb_end(fb);
}

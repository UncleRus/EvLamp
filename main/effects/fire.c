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

#define MAX_SCALE 100
#define HALF_SCALE (MAX_SCALE / 2)

enum {
    PALETTE_FIRE = 0,
    PALETTE_WOOD,
    PALETTE_SODIUM,
    PALETTE_COPPER,
    PALETTE_ALCOHOL,
    PALETTE_RUBIDIUM,
    PALETTE_POTASSIUM,
    PALETTE_LITHIUM
};

#define C_BLACK          { .r = 0,    .g = 0,    .b = 0 }
#define C_WHITE          { .r = 0xff, .g = 0xff, .b = 0xff }
#define C_GREEN          { .r = 0,    .g = 0xff, .b = 0}
#define C_BLUE           { .r = 0,    .g = 0,    .b = 0xff }
#define C_DARK_BLUE      { .r = 0,    .g = 0,    .b = 0x8b }
#define C_RED            { .r = 0xff, .g = 0,    .b = 0 }
#define C_YELLOW         { .r = 0xff, .g = 0xff, .b = 0 }
#define C_ORANGE_RED     { .r = 0xff, .g = 0x45, .b = 0 }
#define C_ORANGE         { .r = 0xff, .g = 0x80, .b = 0 }
#define C_GOLD           { .r = 0xff, .g = 0xdf, .b = 0 }
#define C_GOLDENROD      { .r = 0xda, .g = 0xa5, .b = 0x20}
#define C_GREEN_YELLOW   { .r = 0xad, .g = 0xff, .b = 0x2f}
#define C_LIME_GREEN     { .r = 0x32, .g = 0xcd, .b = 0x32}
#define C_DEEP_SKY_BLUE  { .r = 0,    .g = 0xbf, .b = 0xff }
#define C_LIGHT_SKY_BLUE { .r = 0x87, .g = 0xce, .b = 0xfa }
#define C_INDIGO         { .r = 0x4b, .g = 0,    .b = 0x82}
#define C_MEDIUM_PURPLE  { .r = 0x93, .g = 0x70, .b = 0xdb }
#define C_DEEP_PINK      { .r = 0xff, .g = 0x14, .b = 0x93 }
#define C_PINK           { .r = 0xff, .g = 0xc0, .b = 0xcb }
#define C_FIRE_BRICK     { .r = 0xb2, .g = 0x22, .b = 0x22 }

#define PALETTE_SIZE 4

static const rgb_t p_fire[]      = { C_BLACK, C_RED, C_YELLOW, C_WHITE };
static const rgb_t p_wood[]      = { C_BLACK, C_ORANGE_RED, C_ORANGE, C_GOLD };
static const rgb_t p_sodium[]    = { C_BLACK, C_ORANGE, C_GOLD, C_GOLDENROD };
static const rgb_t p_copper[]    = { C_BLACK, C_GREEN, C_GREEN_YELLOW, C_LIME_GREEN };
static const rgb_t p_alcohol[]   = { C_BLACK, C_BLUE, C_DEEP_SKY_BLUE, C_LIGHT_SKY_BLUE };
static const rgb_t p_rubidium[]  = { C_BLACK, C_INDIGO, C_INDIGO, C_DARK_BLUE };
static const rgb_t p_potassium[] = { C_BLACK, C_INDIGO, C_MEDIUM_PURPLE, C_DEEP_PINK };
static const rgb_t p_lithium[]   = { C_BLACK, C_FIRE_BRICK, C_PINK, C_DEEP_PINK };

static const rgb_t * const palettes[] = {
    [PALETTE_FIRE]      = p_fire,
    [PALETTE_WOOD]      = p_wood,
    [PALETTE_SODIUM]    = p_sodium,
    [PALETTE_COPPER]    = p_copper,
    [PALETTE_ALCOHOL]   = p_alcohol,
    [PALETTE_RUBIDIUM]  = p_rubidium,
    [PALETTE_POTASSIUM] = p_potassium,
    [PALETTE_LITHIUM]   = p_lithium,
};

#define P_PALETTE 0
#define P_SCALE   1

EFFECT_PARAMS(fire, 2) = {
    DECL_PARAM(P_PALETTE, "Palette", PALETTE_FIRE, PALETTE_LITHIUM, 0),
    DECL_PARAM(P_SCALE, "Scale", 10, MAX_SCALE, 60),
};

static uint32_t time_scale = 1;

esp_err_t effect_fire_prepare(framebuffer_t *fb)
{
    time_scale = PARAM_VAL(fire, P_SCALE) >= HALF_SCALE ? 1 : (HALF_SCALE - PARAM_VAL(fire, P_SCALE)) / 2;

    return ESP_OK;
}

esp_err_t effect_fire_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    uint32_t time_ms = esp_timer_get_time() / 1000;

    for (size_t x = 0; x < fb->width; x++)
        for (size_t y = 0; y < fb->height; y++)
        {
            uint8_t idx = qsub8(
                inoise8_3d(x * PARAM_VAL(fire, P_SCALE), y * PARAM_VAL(fire, P_SCALE) + time_ms / time_scale, time_ms / 3),
                abs8(y - (fb->height - 1)) * 255 / (fb->height - 1)
            );
            rgb_t c = color_from_palette_rgb((rgb_t *)palettes[PARAM_VAL(fire, P_PALETTE)], PALETTE_SIZE, idx, 255, true);
            fb_set_pixel_rgb(fb, x, fb->height - y - 1, c);
        }

    return fb_end(fb);
}

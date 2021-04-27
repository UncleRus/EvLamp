/**
 * Waterfall/Fire effect
 *
 * Parameters:
 *     - hue:      Basic hue for waterfall palette. Ignored when mode = WATERFALL_FIRE
 *     - cooling:  How much does the air cool as it rises. Less cooling = taller flames,
 *                 more cooling = shorter flames. Suggested range 20-100.
 *     - sparking: Chance (out of 255) that a new spark will light up. Suggested range 50-200.
 *
 * Recommended parameters for fire mode: cooling = 90, sparking = 80
 */
#include "effects/waterfall.h"

#include <lib8tion.h>
#include <stdlib.h>

#define P_MODE     0
#define P_HUE      1
#define P_COOLING  2
#define P_SPARKING 3

EFFECT_PARAMS(waterfall, 4) = {
    DECL_PARAM(P_MODE, "Mode", 0, 2, 0),
    DECL_PARAM(P_HUE, "Color", 0, 255, 220),
    DECL_PARAM(P_COOLING, "Cooling", 20, 100, 60),
    DECL_PARAM(P_SPARKING, "Sparking", 20, 200, 60),
};

enum {
    WATERFALL_COLORS = 0,
    WATERFALL_FIRE,
    WATERFALL_COLD_FIRE,
};

#define PALETTE_SIZE 16

static rgb_t palette[PALETTE_SIZE];
static uint8_t *map = NULL;

esp_err_t effect_waterfall_prepare(framebuffer_t *fb)
{
    CHECK_ARG(fb);

    if (!map) map = calloc(fb->width * fb->height, 1);
    if (!map) return ESP_ERR_NO_MEM;

    switch (EPARAM(waterfall, P_MODE))
    {
        case WATERFALL_COLORS:
            rgb_fill_gradient4_hsv(palette, PALETTE_SIZE,
                    hsv_from_values(0, 0, 0),
                    hsv_from_values(EPARAM(waterfall, P_HUE), 0, 255),
                    hsv_from_values(EPARAM(waterfall, P_HUE), 128, 255),
                    hsv_from_values(255, 255, 255),
                    COLOR_SHORTEST_HUES);
            break;
        case WATERFALL_FIRE:
            rgb_fill_gradient4_rgb(palette, PALETTE_SIZE,
                    rgb_from_values(0, 0, 0),       // black
                    rgb_from_values(255, 0, 0),
                    rgb_from_values(255, 255, 0),
                    rgb_from_values(255, 255, 255)); // white
            break;
        case WATERFALL_COLD_FIRE:
            rgb_fill_gradient4_rgb(palette, PALETTE_SIZE,
                    rgb_from_values(0, 0, 0),       // black
                    rgb_from_values(0, 0, 100),
                    rgb_from_values(0, 200, 255),
                    rgb_from_values(255, 255, 255)); // white
            break;
        default:
            return ESP_ERR_NOT_SUPPORTED;
    }

    return ESP_OK;
}

#define MAP_XY(x, y) ((y) * fb->width + (x))

esp_err_t effect_waterfall_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    for (size_t x = 0; x < fb->width; x++)
    {
        size_t y;

        // Step 1.  Cool down every cell a little
        for (y = 0; y < fb->height; y++)
            map[MAP_XY(x, y)] = qsub8(map[MAP_XY(x, y)], random8_to((EPARAM(waterfall, P_COOLING) * 10 / fb->height) + 2));

        // Step 2.  Heat from each cell drifts 'up' and diffuses a little
        for (y = fb->height - 1; y >= 2; y--)
            map[MAP_XY(x, y)] =
                    (map[MAP_XY(x, y - 1)] + map[MAP_XY(x, y - 2)] + map[MAP_XY(x, y - 2)]) / 3;

        // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
        if (random8() < EPARAM(waterfall, P_SPARKING))
        {
            y = random8_to(2);
            map[MAP_XY(x, y)] = qadd8(map[MAP_XY(x, y)], random8_between(160, 255));
        }

        // Step 4.  Map from heat cells to LED colors
        for (y = 0; y < fb->height; y++)
        {
            // Scale the heat value from 0-255 down to 0-240
            // for best results with color palettes.
            uint8_t color_idx = scale8(map[MAP_XY(x, y)], 240);
            bool is_fire = (EPARAM(waterfall, P_MODE) == WATERFALL_FIRE || EPARAM(waterfall, P_MODE) == WATERFALL_COLD_FIRE);
            fb_set_pixel_rgb(fb, x, is_fire ? y : fb->height - 1 - y,
                    color_from_palette_rgb(palette, PALETTE_SIZE, color_idx, 255, true));
        }
    }

    return fb_end(fb);
}

/**
 * Water surface effect
 *
 * https://wokwi.com/arduino/projects/287787908971102732
 */
#include "effects/water.h"

#include <lib8tion.h>
#include <noise.h>

#define P_HUE   0
#define P_SAT   1
#define P_SPEED 2

EFFECT_PARAMS(water, 3) = {
    DECL_PARAM_RANGE(P_HUE, "Hue", 0, 255, 150),
    DECL_PARAM_RANGE(P_SAT, "Saturation", 0, 255, 255),
    DECL_PARAM_RANGE(P_SPEED, "Speed", 0, 54, 30),
};

#define PALETTE_SIZE 16

static rgb_t palette[PALETTE_SIZE];

esp_err_t effect_water_prepare(framebuffer_t *fb)
{
    rgb_fill_solid_hsv(palette, hsv_from_values(PARAM_VAL(water, P_HUE), PARAM_VAL(water, P_SAT), 230), PALETTE_SIZE);

    rgb_t c1 = hsv2rgb_rainbow(hsv_from_values(PARAM_VAL(water, P_HUE), PARAM_VAL(water, P_SAT) - 60, 255));
    rgb_t c2 = hsv2rgb_rainbow(hsv_from_values(PARAM_VAL(water, P_HUE), 255 - PARAM_VAL(water, P_SAT), 210));
    palette[7] = c1;
    palette[8] = c2;
    palette[9] = c2;
    palette[10] = c1;

    return ESP_OK;
}

esp_err_t effect_water_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    uint32_t time_ms = esp_timer_get_time() / 1000;

    for (size_t y = 0; y < fb->height; y++)
        for (size_t x = 0; x < fb->width; x++)
        {
            uint8_t idx = inoise8_3d(x * 30, y * 30, time_ms / (255 - (PARAM_VAL(water, P_SPEED) + 200)));
            fb_set_pixel_rgb(fb, x, y, color_from_palette_rgb(palette, PALETTE_SIZE, idx, 255, true));
        }
    fb_blur2d(fb, 32);

    return fb_end(fb);
}

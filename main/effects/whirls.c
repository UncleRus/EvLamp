/**
 * Whirls effect by SottNick
 */
#include "effects/whirls.h"

#include <lib8tion.h>
#include <noise.h>
#include "palettes.h"

#define MAX_WHIRLS 50

#define P_COUNT      0
#define P_SCALE      1
#define P_PALETTE    2
#define P_FADEOUT    3
#define P_TIME_SCALE 4

EFFECT_PARAMS(whirls, 5) = {
    DECL_PARAM_RANGE(P_COUNT, "Count", 10, MAX_WHIRLS, 20),
    DECL_PARAM_RANGE(P_SCALE, "Scale", 5, 50, 26),
    DECL_PARAM_RANGE(P_PALETTE, "Palette", PALETTE_FIRE, PALETTE_MAX - 1, 0),
    DECL_PARAM_RANGE(P_FADEOUT, "Fadeout", 10, 250, 50),
    DECL_PARAM_RANGE(P_TIME_SCALE, "Time scale", 1, 10, 10),
};

typedef struct
{
    float x, y;
} whirl_t;

static whirl_t whirls[MAX_WHIRLS] = { 0 };

static void reset_whirl(framebuffer_t *fb, size_t i)
{
    whirls[i].x = random8_to(fb->width);
    whirls[i].y = 0.0f;
}

esp_err_t effect_whirls_prepare(framebuffer_t *fb)
{
    for (size_t i = 0; i < PARAM_VAL(whirls, P_COUNT); i++)
        reset_whirl(fb, i);

    return ESP_OK;
}

esp_err_t effect_whirls_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    uint16_t scaled_time = esp_timer_get_time() / 1000 / PARAM_VAL(whirls, P_TIME_SCALE);

    fb_fade(fb, PARAM_VAL(whirls, P_FADEOUT));

    for (size_t i = 0; i < PARAM_VAL(whirls, P_COUNT); i++)
    {
        uint16_t x_offset = PARAM_VAL(whirls, P_SCALE) * whirls[i].x;
        uint16_t y_offset = PARAM_VAL(whirls, P_SCALE) * whirls[i].y;
        uint8_t angle = inoise8_3d(scaled_time + x_offset, scaled_time + y_offset, scaled_time);

        whirls[i].x += (float)sin8(angle) * 0.0078125f - 1.0f;
        whirls[i].y -= (float)cos8(angle) * 0.0078125f - 1.0f;

        rgb_t c = color_from_palette_rgb(
                palettes[PARAM_VAL(whirls, P_PALETTE)].palette,
                palettes[PARAM_VAL(whirls, P_PALETTE)].size,
                angle / 2, 255, true);
        fb_set_pixelf_rgb(fb, whirls[i].x, whirls[i].y, c);

        if (whirls[i].x < 0 || whirls[i].x >= (float)fb->width || whirls[i].y < 0 || whirls[i].y >= (float)fb->height)
            reset_whirl(fb, i);
    }

    return fb_end(fb);
}

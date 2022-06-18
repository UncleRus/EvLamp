/**
 * Sun radiation effect
 *
 * Author: Yaroslaw Turbin (https://vk.com/ldirko, https://www.reddit.com/user/ldirko/)
 */
#include "effects/sun.h"

#include <lib8tion.h>
#include <noise.h>
#include "palettes.h"

#define MODE_SINGLE_COLOR 0

#define P_SIZE    0
#define P_PALETTE 1

EFFECT_PARAMS(sun, 2) = {
    DECL_PARAM_RANGE(P_SIZE, "Size", 4, 32, 8),
    DECL_PARAM_RANGE(P_PALETTE, "Palette", 0, PALETTE_MAX - 1, PALETTE_HEAT),
};

static uint8_t *bump_map = NULL;

esp_err_t effect_sun_prepare(framebuffer_t *fb)
{
    if (!bump_map)
        bump_map = malloc((fb->width + 2) * (fb->height + 2));
    if (!bump_map)
    {
        ESP_LOGE(TAG, "Error allocating memory for bump map in sun effect");
        return ESP_ERR_NO_MEM;
    }
    return ESP_OK;
}

static void generate_bump(framebuffer_t *fb)
{
    size_t time = esp_timer_get_time() / 4000;
    size_t i = 0;
    for (size_t x = 0; x < fb->width + 2; x++)
        for (size_t y = 0; y < fb->height + 2; y++)
            bump_map[i++] = inoise8_3d_raw(x * 25, y * 25, time) / 2;
}

esp_err_t effect_sun_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    generate_bump(fb);
    
    int idx_y = (int)fb->width + 3;
    int vly = -((int)fb->height / 2 + 1);
    for (size_t y = 0; y < fb->height; y++)
    {
        vly++;
        int vlx = -((int)fb->width / 2 + 1);
        for (size_t x = 0; x < fb->width; x++)
        {
            vlx++;
            int8_t nx = (int8_t)(bump_map[x + idx_y + 1] - bump_map[x + idx_y - 1]);
            int8_t ny = (int8_t)(bump_map[x + idx_y + (fb->width + 2)] - bump_map[x + idx_y - (fb->width + 2)]);

            uint8_t dx = abs8(vlx * 7 - nx);
            uint8_t dy = abs8(vly * 7 - ny);
            int temp = dx * dx + dy * dy;
            int c = 200 - temp / PARAM_VAL(sun, P_SIZE);
            if (c < 0) c = 0;
            fb_set_pixel_rgb(fb, x, y,
                    color_from_palette_rgb(palettes[PARAM_VAL(sun, P_PALETTE)].palette, palettes[PARAM_VAL(sun, P_PALETTE)].size, c, 255, true));
        }
        idx_y += ((int)fb->width + 2);
    }

    return fb_end(fb);
}

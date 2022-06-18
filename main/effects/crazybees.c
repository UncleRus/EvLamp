/**
 * Crazy Bees effect
 *
 * Author: Stepko
 */
#include "effects/crazybees.h"
#include <lib8tion.h>

#define MAX_BEES 10

#define P_NUM_BEES 0
#define P_BEE_HUE  1
#define P_BEE_SAT  2

EFFECT_PARAMS(crazybees, 3) = {
    DECL_PARAM_RANGE(P_NUM_BEES, "Number of bees", 1, MAX_BEES, 3),
    DECL_PARAM_RANGE(P_BEE_HUE, "Bee hue", 0, 255, 100),
    DECL_PARAM_RANGE(P_BEE_SAT, "Bee saturation", 0, 255, 0),
};

typedef struct
{
    size_t x, y;
    size_t flower_x, flower_y;
    uint8_t flower_hue;
} bee_t;

static bee_t bees[MAX_BEES];

static rgb_t bee_color;

static void change_flower(framebuffer_t *fb, uint8_t bee)
{
    bees[bee].flower_x = random8_to(fb->width);
    bees[bee].flower_y = random8_to(fb->height);
    bees[bee].flower_hue = random8();
}

esp_err_t effect_crazybees_prepare(framebuffer_t *fb)
{
    for (uint8_t i = 0; i < PARAM_VAL(crazybees, P_NUM_BEES); i++)
    {
        // set bee
        bees[i].x = random8_to(fb->width);
        bees[i].y = random8_to(fb->height);
        // set flower
        change_flower(fb, i);
    }
    bee_color = hsv2rgb_rainbow(hsv_from_values(PARAM_VAL(crazybees, P_BEE_HUE), PARAM_VAL(crazybees, P_BEE_SAT), 255));
    return ESP_OK;
}

esp_err_t effect_crazybees_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    fb_fade(fb, 8);
    for (uint8_t i = 0; i < PARAM_VAL(crazybees, P_NUM_BEES); i++)
    {
        // move bee
        if (bees[i].x > bees[i].flower_x) bees[i].x--;
        if (bees[i].y > bees[i].flower_y) bees[i].y--;
        if (bees[i].x < bees[i].flower_x) bees[i].x++;
        if (bees[i].y < bees[i].flower_y) bees[i].y++;

        // bingo, change flower
        if (bees[i].x == bees[i].flower_x && bees[i].y == bees[i].flower_y)
            change_flower(fb, i);

        // draw bee
        fb_set_pixel_rgb(fb, bees[i].x, bees[i].y, bee_color);

        // draw flower
        hsv_t c = { .h = bees[i].flower_hue, .s = 255, .v = 255 };
        fb_set_pixel_hsv(fb, bees[i].flower_x - 1, bees[i].flower_y, c);
        fb_set_pixel_hsv(fb, bees[i].flower_x, bees[i].flower_y - 1, c);
        fb_set_pixel_hsv(fb, bees[i].flower_x + 1, bees[i].flower_y, c);
        fb_set_pixel_hsv(fb, bees[i].flower_x, bees[i].flower_y + 1, c);
    }
    fb_blur2d(fb, 16);

    return fb_end(fb);
}

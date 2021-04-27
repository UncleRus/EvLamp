/**
 * Crazy Bees effect
 *
 * Author: Stepko
 */
#include "effects/crazybees.h"
#include <lib8tion.h>

#define MAX_BEES 10

#define P_NUM_BEES 0

EFFECT_PARAMS(crazybees, 1) = {
    DECL_PARAM(P_NUM_BEES, "Number of bees", 1, MAX_BEES, 4),
};

typedef struct
{
    size_t x, y;
    size_t flower_x, flower_y;
    uint8_t hue;
} bee_t;

static bee_t bees[MAX_BEES];

static const rgb_t bee_color = { .r = 255, .g = 255, .b = 255 };

static void change_flower(framebuffer_t *fb, uint8_t bee)
{
    bees[bee].flower_x = random8_to(fb->width);
    bees[bee].flower_y = random8_to(fb->height);
    bees[bee].hue = random8();
}

esp_err_t effect_crazybees_prepare(framebuffer_t *fb)
{
    for (uint8_t i = 0; i < EPARAM(crazybees, P_NUM_BEES); i++)
    {
        // set bee
        bees[i].x = random8_to(fb->width);
        bees[i].y = random8_to(fb->height);
        // set flower
        change_flower(fb, i);
    }
    return ESP_OK;
}

esp_err_t effect_crazybees_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    fb_fade(fb, 8);
    for (uint8_t i = 0; i < EPARAM(crazybees, P_NUM_BEES); i++)
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
        hsv_t c = { .h = bees[i].hue, .s = 255, .v = 255 };
        fb_set_pixel_hsv(fb, bees[i].flower_x - 1, bees[i].flower_y, c);
        fb_set_pixel_hsv(fb, bees[i].flower_x, bees[i].flower_y - 1, c);
        fb_set_pixel_hsv(fb, bees[i].flower_x + 1, bees[i].flower_y, c);
        fb_set_pixel_hsv(fb, bees[i].flower_x, bees[i].flower_y + 1, c);
    }
    fb_blur2d(fb, 16);

    return fb_end(fb);
}

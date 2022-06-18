/**
 * Fireflies (wind) effect
 *
 * Author: stepko
 * https://wokwi.com/projects/289974688028819977
 */
#include "effects/fireflies.h"

#include <lib8tion.h>

#define MAX_FIREFLIES 48

#define P_COUNT 0
#define P_CHAOS 1
#define P_SPEED 2
#define P_TIME  3

EFFECT_PARAMS(fireflies, 4) = {
    DECL_PARAM_RANGE(P_COUNT, "Number of fireflies", 4, MAX_FIREFLIES, 16),
    DECL_PARAM_BOOL(P_CHAOS, "Chaotic movement", 1),
    DECL_PARAM_RANGE(P_SPEED, "Speed", 1, 255, 255),
    DECL_PARAM_RANGE(P_TIME, "Update time", 1, 10, 3),
};

typedef struct
{
    int pos_x, pos_y;
    int center_x, center_y;
    uint8_t speed_x, speed_y;
    uint8_t mass;
    uint8_t hue;
} firefly_t;

static firefly_t flies[MAX_FIREFLIES];
static uint32_t last_change = 0;

esp_err_t effect_fireflies_prepare(framebuffer_t *fb)
{
    for (size_t i = 0; i < PARAM_VAL(fireflies, P_COUNT); i++)
    {
        flies[i].center_x = random16_to(fb->width * 10);
        flies[i].center_y = random16_to(fb->height * 10);
        flies[i].speed_x = random8_between(25, 50);
        flies[i].speed_y = random8_between(25, 50);
        flies[i].mass = random8_between(30, 255);
        flies[i].hue = random8();
    }
    return ESP_OK;
}

esp_err_t effect_fireflies_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    uint32_t t = esp_timer_get_time() / 1000000; // current time in seconds
    if (!last_change) last_change = t; // seconds

    for (size_t i = 0; i < PARAM_VAL(fireflies, P_COUNT); i++)
    {
        flies[i].hue++;

        if (PARAM_VAL(fireflies, P_CHAOS))
        {
            flies[i].pos_x = beatsin16(flies[i].speed_x * PARAM_VAL(fireflies, P_SPEED), flies[i].center_x,
                flies[i].center_x + flies[i].mass * (fb->height + fb->width) / 16, 0, 0);
            flies[i].pos_y = beatsin16(flies[i].speed_y * PARAM_VAL(fireflies, P_SPEED), flies[i].center_y,
                flies[i].center_y + flies[i].mass * (fb->height + fb->width) / 16, 0, 0);
        }
        else
        {
            flies[i].pos_x = beatsin16(flies[0].speed_x * PARAM_VAL(fireflies, P_SPEED), flies[i].center_x,
                flies[i].center_x + flies[i].mass * (fb->height + fb->width) / 16, 0, 0);
            flies[i].pos_y = beatsin16(flies[0].speed_y * PARAM_VAL(fireflies, P_SPEED), flies[i].center_y,
                flies[i].center_y + flies[i].mass * (fb->height + fb->width) / 16, 0, 0);
        }

        if (flies[i].pos_x < 0)
            flies[i].pos_x = (fb->width - 1) * 10 - flies[i].pos_x - (fb->width - 1) * 10;
        if (flies[i].pos_x > (fb->width - 1) * 10)
            flies[i].pos_x = flies[i].pos_x - (fb->width - 1) * 10;
        if (flies[i].pos_y < 0)
            flies[i].pos_y = (fb->height - 1) * 10 - flies[i].pos_y - (fb->height - 1) * 10;
        if (flies[i].pos_y > (fb->height - 1) * 10)
            flies[i].pos_y = flies[i].pos_y - (fb->height - 1) * 10;

        fb_set_pixelf_hsv(fb, flies[i].pos_x / 10.0f, flies[i].pos_y / 10.0f,
            hsv_from_values(flies[i].hue, 255, beatsin8(0, 128, 255, 0, 0)));
    }

    fb_fade(fb, 50);

    if (t - last_change >= PARAM_VAL(fireflies, P_TIME))
    {
        last_change = t;
        for (size_t i = 0; i < PARAM_VAL(fireflies, P_COUNT); i++)
        {
            flies[i].speed_x = random8_between(25, 50);
            flies[i].speed_y = random8_between(25, 50);
            flies[i].mass += 25 - random8_to(50);
            flies[i].center_x -= random8_to(20);
            if (flies[i].center_x < 0)
                flies[i].center_x = (fb->width - 1) * 10;
            flies[i].center_y += random8_to(20);
            if (flies[i].center_y > (fb->height - 1) * 10)
                flies[i].center_y = 0;
        }
    }

    return fb_end(fb);
}

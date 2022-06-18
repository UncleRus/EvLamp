#include "effects/lamp.h"

#define P_RED   0
#define P_GREEN 1
#define P_BLUE  2

// 2500 K by default
EFFECT_PARAMS(lamp, 3) = {
    DECL_PARAM_RANGE(P_RED, "Red", 0, 255, 255),
    DECL_PARAM_RANGE(P_GREEN, "Green", 0, 255, 159),
    DECL_PARAM_RANGE(P_BLUE, "Blue", 0, 255, 70),
};

esp_err_t effect_lamp_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    rgb_t color = { .r = PARAM_VAL(lamp, P_RED), .g = PARAM_VAL(lamp, P_GREEN), .b = PARAM_VAL(lamp, P_BLUE) };

    for (size_t i = 0; i < fb->height * fb->width; i++)
        fb->data[i] = color;

    return fb_end(fb);
}

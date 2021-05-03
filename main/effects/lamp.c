#include "effects/lamp.h"

#define P_RED   0
#define P_GREEN 1
#define P_BLUE  2

// 2500 K by default
EFFECT_PARAMS(lamp, 3) = {
    DECL_PARAM(P_RED, "Red", 0, 255, 255),
    DECL_PARAM(P_GREEN, "Green", 0, 255, 159),
    DECL_PARAM(P_BLUE, "Blue", 0, 255, 70),
};

esp_err_t effect_lamp_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    rgb_t color = { .r = EPARAM(lamp, P_RED), .g = EPARAM(lamp, P_GREEN), .b = EPARAM(lamp, P_BLUE) };

    for (size_t i = 0; i < fb->height * fb->width; i++)
        fb->data[i] = color;

    return fb_end(fb);
}

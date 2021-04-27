#include "effects/lamp.h"

#define P_HUE 0
#define P_SAT 1
#define P_VAL 2

EFFECT_PARAMS(lamp, 3) = {
    DECL_PARAM(P_HUE, "Hue",        0, 255, 60),
    DECL_PARAM(P_SAT, "Saturation", 0, 255, 40),
    DECL_PARAM(P_VAL, "Value",      0, 255, 255),
};

esp_err_t effect_lamp_run(framebuffer_t *fb)
{
    CHECK(fb_begin(fb));

    hsv_t color = { .h = EPARAM(lamp, P_HUE), .s = EPARAM(lamp, P_SAT), .v = EPARAM(lamp, P_VAL) };

    for (size_t i = 0; i < fb->height * fb->width; i++)
        fb->data[i] = hsv2rgb_rainbow(color);

    return fb_end(fb);
}

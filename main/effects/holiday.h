/**
 * Holiday lights
 *
 * Author: stepko
 *
 * https://wokwi.com/arduino/projects/287698321325163021
 */
#ifndef __EFFECTS_HOLIDAY_H__
#define __EFFECTS_HOLIDAY_H__

#include "effect.h"

extern EFFECT_PARAMS(holiday, 2);

enum {
    HOLIDAY_WAVE = 0,
    HOLIDAY_RADAR,
    HOLIDAY_SPARKS,
    HOLIDAY_TREE,
};

esp_err_t effect_holiday_run(framebuffer_t *fb);

#define DESCR_EFFECT_HOLIDAY DECL_EFFECT_SHORT(holiday, "Holiday lights")

#endif /* __EFFECTS_HOLIDAY_H__ */

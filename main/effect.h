#ifndef _MAIN_EFFECT_H_
#define _MAIN_EFFECT_H_

#include "common.h"
#include <framebuffer.h>

#define EFFECT_PARAMS(name, count) effect_param_t effect_ ##name## _params[count]

#define DECL_PARAM(ID, NAME, MIN, MAX, DEF) [ID] = { .name = NAME, .min = MIN, .max = MAX, .def = DEF, .value = DEF }

#define EPARAM(name, X) (effect_ ##name## _params[(X)].value)

#define DECL_EFFECT_FULL(CODE, NAME) { \
        .name = NAME, \
        .prepare = effect_ ##CODE## _prepare, \
        .run = effect_ ##CODE## _run, \
        .params = effect_ ##CODE## _params, \
        .params_count = sizeof(effect_ ##CODE## _params) / sizeof(effect_param_t) }

#define DECL_EFFECT_SHORT(CODE, NAME) { \
        .name = NAME, \
        .prepare = NULL, \
        .run = effect_ ##CODE## _run, \
        .params = effect_ ##CODE## _params, \
        .params_count = sizeof(effect_ ##CODE## _params) / sizeof(effect_param_t) }

typedef struct
{
    char name[32];
    uint8_t min, max, def, value;
} effect_param_t;

typedef esp_err_t (*effect_prepare_cb)(framebuffer_t *fb);
typedef esp_err_t (*effect_run_cb)(framebuffer_t *fb);

typedef struct
{
    char name[32];
    effect_prepare_cb prepare;
    effect_run_cb run;
    effect_param_t *params;
    size_t params_count;
} effect_descriptor_t;

////////////////////////////////////////////////////////////////////////////////

extern const effect_descriptor_t effects[];

extern const size_t effects_count;

esp_err_t effects_reset();

esp_err_t effects_load();

esp_err_t effect_param_set(size_t effect, size_t param, uint8_t value);

esp_err_t effects_init();

#endif /* _MAIN_EFFECT_H_ */

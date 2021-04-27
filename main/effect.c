#include "effect.h"
#include <nvs.h>

#include "effects/crazybees.h"
#include "effects/dna.h"
#include "effects/fire.h"
#include "effects/lamp.h"
#include "effects/lava.h"
#include "effects/matrix.h"
#include "effects/plasma_waves.h"
#include "effects/rain.h"
#include "effects/rainbow.h"
#include "effects/rays.h"
#include "effects/sparkles.h"
#include "effects/waterfall.h"

#define PARAMS_STORAGE_NAME "effects"
#define NVS_KEY_FMT "e%04x:p%04x"

const effect_descriptor_t effects[] = {
    DESCR_EFFECT_LAMP,
    DESCR_EFFECT_FIRE,
    DESCR_EFFECT_SPARKLES,
    DESCR_EFFECT_LAVA,
    DESCR_EFFECT_WATERFALL,
    DESCR_EFFECT_PLASMA_WAVES,
    DESCR_EFFECT_DNA,
    DESCR_EFFECT_MATRIX,
    DESCR_EFFECT_CRAZYBEES,
    DESCR_EFFECT_RAIN,
    DESCR_EFFECT_RAINBOW,
    DESCR_EFFECT_RAYS,
};

const size_t effects_count = sizeof(effects) / sizeof(effect_descriptor_t);

#define PARAM(effect, param) effects[(effect)].params[(param)]

esp_err_t effects_reset()
{
    ESP_LOGI(TAG, "Resetting effect parameters to defaults");

    for (size_t e = 0; e < effects_count; e++)
        for (size_t p = 0; p < effects[e].params_count; p++)
            CHECK_LOGE(effect_param_set(e, p, PARAM(e, p).def),
                    "Could not set %s.%s to %d", effects[e].name, PARAM(e, p).name, PARAM(e, p).def);

    ESP_LOGI(TAG, "Effect parameters have bin reset to defaults");

    return ESP_OK;
}

esp_err_t effects_load()
{
    ESP_LOGI(TAG, "Loading effect parameters");

    char key[16];
    uint8_t val;

    nvs_handle_t nvs;
    CHECK_LOGE(nvs_open(PARAMS_STORAGE_NAME, NVS_READWRITE, &nvs),
            "Could not open NVS storage '%s'", PARAMS_STORAGE_NAME);
    for (size_t e = 0; e < effects_count; e++)
    {
        ESP_LOGI(TAG, "Effect %d: '%s'", e, effects[e].name);
        for (size_t p = 0; p < effects[e].params_count; p++)
        {
            snprintf(key, sizeof(key), NVS_KEY_FMT, e, p);

            esp_err_t res = nvs_get_u8(nvs, key, &val);
            if (res == ESP_ERR_NOT_FOUND || val < PARAM(e, p).min || val > PARAM(e, p).max)
            {
                val = PARAM(e, p).def;
                nvs_set_u8(nvs, key, val);
            }
            PARAM(e, p).value = val;

            ESP_LOGI(TAG, "    %s: %d (min %d, max %d, def %d)",
                    PARAM(e, p).name, PARAM(e, p).value, PARAM(e, p).min, PARAM(e, p).max, PARAM(e, p).def);
        }
    }
    nvs_close(nvs);

    ESP_LOGI(TAG, "Effect parameters loaded");

    return ESP_OK;
}

esp_err_t effect_param_set(size_t effect, size_t param, uint8_t value)
{
    CHECK_ARG(effect < effects_count
            && param < effects[effect].params_count
            && value >= PARAM(effect, param).min
            && value <= PARAM(effect, param).max);

    nvs_handle_t nvs;
    CHECK_LOGE(nvs_open(PARAMS_STORAGE_NAME, NVS_READWRITE, &nvs),
            "Could not open NVS storage '%s'", PARAMS_STORAGE_NAME);
    char key[16];
    snprintf(key, sizeof(key), NVS_KEY_FMT, effect, param);
    nvs_set_u8(nvs, key, value);
    nvs_close(nvs);

    PARAM(effect, param).value = value;

    return ESP_OK;
}


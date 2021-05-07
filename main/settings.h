#ifndef MAIN_SETTINGS_H_
#define MAIN_SETTINGS_H_

#include "common.h"
#include <cJSON.h>

typedef struct
{
    struct {
        uint32_t mode;
        struct {
            bool dhcp;
            char ip[16];
            char netmask[16];
            char gateway[16];
            char dns[16];
        } ip;
        wifi_ap_config_t ap;
        wifi_sta_config_t sta;
    } wifi;
    struct {
        uint8_t width;
        uint8_t height;
        uint8_t type;
        uint8_t gpio;
        uint32_t current_limit;
    } leds;
} system_settings_t;

extern system_settings_t sys_settings;

typedef struct
{
    uint8_t fps;
    uint8_t brightness;
    size_t effect;
} volatile_settings_t;

extern volatile_settings_t vol_settings;

esp_err_t settings_init();

esp_err_t sys_settings_reset();

esp_err_t sys_settings_load();

esp_err_t sys_settings_save();

esp_err_t vol_settings_reset();

esp_err_t vol_settings_load();

esp_err_t vol_settings_save();

#endif /* MAIN_SETTINGS_H_ */

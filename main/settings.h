#ifndef MAIN_SETTINGS_H_
#define MAIN_SETTINGS_H_

#include "common.h"

typedef struct
{
    struct {
        uint32_t mode;
        struct {
            bool dhcp;
            char ip[16];
            char netmask[16];
            char gateway[16];
        } ip;
        wifi_ap_config_t ap;
        wifi_sta_config_t sta;
    } wifi;
} system_settings_t;

extern system_settings_t settings;

esp_err_t settings_init();

esp_err_t settings_load();

esp_err_t settings_save();

esp_err_t settings_reset();

#endif /* MAIN_SETTINGS_H_ */

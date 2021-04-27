#include "settings.h"
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>

#define OPT_MAGIC        "magic"
#define OPT_SYS_SETTINGS "settings"
#define OPT_EFF_SETTINGS "effects"

#define SETTINGS_MAGIC 0xdead0001

system_settings_t settings = { 0 };

static system_settings_t defaults = {
    .wifi = {
        .mode = DEFAULT_WIFI_MODE,
        .ip = {
            .dhcp    = DEFAULT_WIFI_DHCP,
            .ip      = CONFIG_EL_WIFI_IP,
            .netmask = CONFIG_EL_WIFI_NETMASK,
            .gateway = CONFIG_EL_WIFI_GATEWAY,
        },
        .ap = {
            .ssid           = CONFIG_EL_WIFI_AP_SSID,
            .channel        = CONFIG_EL_WIFI_AP_CHANNEL,
            .password       = CONFIG_EL_WIFI_AP_PASSWD,
            .max_connection = DEFAULT_WIFI_AP_MAXCONN,
            .authmode       = WIFI_AUTH_WPA_WPA2_PSK
        },
        .sta = {
            .ssid               = CONFIG_EL_WIFI_STA_SSID,
            .password           = CONFIG_EL_WIFI_STA_PASSWD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    },
};

esp_err_t settings_reset()
{
    ESP_LOGI(TAG, "Resetting settings to defaults");

    if (!defaults.wifi.ap.password || !strlen((const char *)defaults.wifi.ap.password))
        defaults.wifi.ap.authmode = WIFI_AUTH_OPEN;
    if (!defaults.wifi.sta.password || !strlen((const char *)defaults.wifi.sta.password))
        defaults.wifi.sta.threshold.authmode = WIFI_AUTH_OPEN;

    memcpy(&settings, &defaults, sizeof(system_settings_t));
    CHECK(settings_save());

    ESP_LOGI(TAG, "Settings have been reset to defaults");

    return ESP_OK;
}

esp_err_t settings_save()
{
    ESP_LOGI(TAG, "Saving settings...");

    nvs_handle_t nvs;
    CHECK_LOGE(nvs_open(NVS_STORAGE_NAME, NVS_READWRITE, &nvs),
            "Could not open NVS to write");
    CHECK_LOGE(nvs_set_u32(nvs, OPT_MAGIC, SETTINGS_MAGIC),
            "Error writing NVS magic");
    CHECK_LOGE(nvs_set_blob(nvs, OPT_SYS_SETTINGS, &settings, sizeof(system_settings_t)),
            "Error writing NVS settings");
    nvs_close(nvs);

    ESP_LOGI(TAG, "Settings saved");

    return ESP_OK;
}

esp_err_t settings_load()
{
    nvs_handle_t nvs;
    esp_err_t res;

    ESP_LOGI(TAG, "Reading settings...");

    res = nvs_open(NVS_STORAGE_NAME, NVS_READONLY, &nvs);
    if (res != ESP_OK)
        return settings_reset();

    uint32_t magic = 0;
    res = nvs_get_u32(nvs, OPT_MAGIC, &magic);
    if (res != ESP_OK || magic != SETTINGS_MAGIC)
    {
        ESP_LOGE(TAG, "Invalid magic 0x%08x, expected 0x%08x", magic, SETTINGS_MAGIC);
        nvs_close(nvs);
        return settings_reset();
    }

    size_t size = sizeof(system_settings_t);
    res = nvs_get_blob(nvs, OPT_SYS_SETTINGS, &settings, &size);
    if (res != ESP_OK || size != sizeof(system_settings_t))
    {
        ESP_LOGE(TAG, "Error reading settings %d (%s)", res, esp_err_to_name(res));
        nvs_close(nvs);
        return settings_reset();
    }

    nvs_close(nvs);

    ESP_LOGI(TAG, "Settings read");

    return ESP_OK;
}

esp_err_t settings_init()
{
    esp_err_t res = nvs_flash_init();
    if (res == ESP_ERR_NVS_NO_FREE_PAGES || res == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        CHECK_LOGE(nvs_flash_erase(), "Could not erase NVS flash");
        CHECK_LOGE(nvs_flash_init(), "Could not init NVS flash");
        return settings_reset();
    }
    return res;
}

#include "settings.h"
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <led_strip.h>

static const char *STORAGE_SYSTEM_NAME   = "system";
static const char *STORAGE_VOLATILE_NAME = "volatile";

static const char *OPT_MAGIC    = "magic";
static const char *OPT_SETTINGS = "settings";

system_settings_t sys_settings = { 0 };
volatile_settings_t vol_settings = { 0 };

static system_settings_t sys_defaults = {
    .wifi = {
        .mode = DEFAULT_WIFI_MODE,
        .ip = {
            .dhcp    = DEFAULT_WIFI_DHCP,
            .ip      = CONFIG_EL_WIFI_IP,
            .netmask = CONFIG_EL_WIFI_NETMASK,
            .gateway = CONFIG_EL_WIFI_GATEWAY,
            .dns     = CONFIG_EL_WIFI_DNS,
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
    .leds = {
        .width         = CONFIG_EL_MATRIX_WIDTH,
        .height        = CONFIG_EL_MATRIX_HEIGHT,
        .type          = DEFAULT_LED_TYPE,
        .current_limit = CONFIG_EL_MATRIX_MAX_CURRENT,
        .v_mirror      = false,
        .h_mirror      = false,
    },
};

static const volatile_settings_t vol_defaults = {
    .fps        = CONFIG_EL_FPS_DEFAULT,
    .brightness = CONFIG_EL_BRIGHTNESS_DEFAULT,
    .effect     = CONFIG_EL_EFFECT_DEFAULT,
};

static esp_err_t _storage_load(const char *storage_name, void *target, size_t size)
{
    nvs_handle_t nvs;
    esp_err_t res;

    ESP_LOGD(TAG, "Reading settings from '%s'...", storage_name);

    CHECK(nvs_open(storage_name, NVS_READONLY, &nvs));

    uint32_t magic = 0;
    res = nvs_get_u32(nvs, OPT_MAGIC, &magic);
    if (magic != SETTINGS_MAGIC)
    {
        ESP_LOGW(TAG, "Invalid magic 0x%08x, expected 0x%08x", magic, SETTINGS_MAGIC);
        res = ESP_FAIL;
    }
    if (res != ESP_OK)
    {
        nvs_close(nvs);
        return res;
    }

    size_t tmp = size;
    res = nvs_get_blob(nvs, OPT_SETTINGS, target, &tmp);
    if (tmp != size)
    {
        ESP_LOGW(TAG, "Invalid settings size");
        res = ESP_FAIL;
    }
    if (res != ESP_OK)
    {
        ESP_LOGE(TAG, "Error reading settings %d (%s)", res, esp_err_to_name(res));
        nvs_close(nvs);
        return res;
    }

    nvs_close(nvs);

    ESP_LOGD(TAG, "Settings read from '%s'", storage_name);

    return ESP_OK;
}

static esp_err_t _storage_save(const char *storage_name, const void *source, size_t size)
{
    ESP_LOGD(TAG, "Saving settings to '%s'...", storage_name);

    nvs_handle_t nvs;
    CHECK_LOGE(nvs_open(storage_name, NVS_READWRITE, &nvs),
            "Could not open NVS to write");
    CHECK_LOGE(nvs_set_u32(nvs, OPT_MAGIC, SETTINGS_MAGIC),
            "Error writing NVS magic");
    CHECK_LOGE(nvs_set_blob(nvs, OPT_SETTINGS, source, size),
            "Error writing NVS settings");
    nvs_close(nvs);

    ESP_LOGD(TAG, "Settings saved to '%s'", storage_name);

    return ESP_OK;
}

////////////////////////////////////////////////////////////////////////////////

esp_err_t settings_init()
{
    esp_err_t res = nvs_flash_init();
    if (res == ESP_ERR_NVS_NO_FREE_PAGES || res == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        CHECK_LOGE(nvs_flash_erase(), "Could not erase NVS flash");
        CHECK_LOGE(nvs_flash_init(), "Could not init NVS flash");
        CHECK_LOGE(sys_settings_reset(), "Could not reset system settings");
        CHECK_LOGE(vol_settings_reset(), "Could not reset volatile settings");
    }
    return res;
}

esp_err_t sys_settings_reset()
{
    if (!strlen((const char *) sys_defaults.wifi.ap.password))
        sys_defaults.wifi.ap.authmode = WIFI_AUTH_OPEN;
    if (!strlen((const char *) sys_defaults.wifi.sta.password))
        sys_defaults.wifi.sta.threshold.authmode = WIFI_AUTH_OPEN;

    memcpy(&sys_settings, &sys_defaults, sizeof(system_settings_t));
    CHECK(sys_settings_save());

    ESP_LOGW(TAG, "System settings have been reset to defaults");

    return ESP_OK;
}

esp_err_t sys_settings_load()
{
    esp_err_t res = _storage_load(STORAGE_SYSTEM_NAME, &sys_settings, sizeof(system_settings_t));
    if (res != ESP_OK)
        return sys_settings_reset();

    return ESP_OK;
}

esp_err_t sys_settings_save()
{
    return _storage_save(STORAGE_SYSTEM_NAME, &sys_settings, sizeof(system_settings_t));
}

esp_err_t vol_settings_reset()
{
    memcpy(&vol_settings, &vol_defaults, sizeof(volatile_settings_t));
    CHECK(vol_settings_save());

    ESP_LOGW(TAG, "Volatile settings have been reset to defaults");

    return ESP_OK;
}

esp_err_t vol_settings_load()
{
    esp_err_t res = _storage_load(STORAGE_VOLATILE_NAME, &vol_settings, sizeof(volatile_settings_t));
    if (res != ESP_OK)
        return vol_settings_reset();

    return ESP_OK;
}

esp_err_t vol_settings_save()
{
    return _storage_save(STORAGE_VOLATILE_NAME, &vol_settings, sizeof(volatile_settings_t));
}

#include "common.h"
#include "settings.h"
#include "wifi.h"
#include "bus.h"
#include "effect.h"

static void main_loop(void *arg)
{
    event_t e;
    while (1)
    {
        if (bus_receive_event(&e, 1000) != ESP_OK)
            continue;

        switch (e.type)
        {
            case EVENT_NETWORK_UP:
                ESP_LOGI(TAG, "Network is up! starting webserver");
                break;
            case EVENT_NETWORK_DOWN:
                ESP_LOGI(TAG, "Network is down! stopping webserver");
                break;
            default:
                ESP_LOGI(TAG, "Event %d", e.type);
        }
    }
}

void app_main()
{
    ESP_LOGI(TAG, "Starting EvLamp");

    // Initialize NVS
    ESP_ERROR_CHECK(settings_init());
    // Load settings from NVS
    ESP_ERROR_CHECK(settings_load());
    // Load effect parameters
    if (effects_load() != ESP_OK)
        ESP_ERROR_CHECK(effects_reset());

    // Initialize bus
    ESP_ERROR_CHECK(bus_init());

    // Try to start WIFI
    esp_err_t res = wifi_init();
    if (res != ESP_OK)
        ESP_LOGW(TAG, "Could not start WiFi: %d (%s)", res, esp_err_to_name(res));

    // Create main task
    if (xTaskCreate(main_loop, APP_NAME, MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL) != pdPASS)
    {
        ESP_LOGE(TAG, "Could not create main task");
        ESP_ERROR_CHECK(ESP_FAIL);
    }
}


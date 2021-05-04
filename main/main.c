#include "common.h"
#include "settings.h"
#include "wifi.h"
#include "bus.h"
#include "effect.h"
#include "surface.h"
#include "input.h"
#include "webserver.h"

static void main_loop(void *arg)
{
    event_t e;
    esp_err_t err;

    while (1)
    {
        if (bus_receive_event(&e, 1000) != ESP_OK)
            continue;

        switch (e.type)
        {
            case EVENT_BUTTON_CLICKED:
                surface_next_effect();
                break;

            case EVENT_BUTTON_LONG_PRESSED:
                if (surface_is_playing())
                    surface_stop();
                else
                    surface_play();
                break;

            case EVENT_NETWORK_UP:
                ESP_LOGI(TAG, "Network is up, starting HTTPD...");
                err = webserver_start();
                if (err != ESP_OK)
                    ESP_LOGW(TAG, "Error starting HTTPD: %d (%s)", err, esp_err_to_name(err));
                break;

            case EVENT_NETWORK_DOWN:
                ESP_LOGI(TAG, "Network is down, stopping HTTPD...");
                err = webserver_stop();
                if (err != ESP_OK)
                    ESP_LOGW(TAG, "Error stopping HTTPD: %d (%s)", err, esp_err_to_name(err));
                break;

            default:
                ESP_LOGI(TAG, "Event %d", e.type);
        }
    }
}

void app_main()
{
    ESP_LOGI(TAG, "Starting " APP_NAME);

    // Initialize NVS
    ESP_ERROR_CHECK(settings_init());
    // Load system settings from NVS
    ESP_ERROR_CHECK(sys_settings_load());
    // Load volatile settings
    ESP_ERROR_CHECK(vol_settings_load());
    // Load effect parameters
    ESP_ERROR_CHECK(effects_init());
    // Init surface
    ESP_ERROR_CHECK(surface_init());

    // Initialize bus
    ESP_ERROR_CHECK(bus_init());

    // Initialize input
    ESP_ERROR_CHECK(input_init());

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


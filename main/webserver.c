#include "webserver.h"

static httpd_handle_t server = NULL;

////////////////////////////////////////////////////////////////////////////////
/// Internal

////////////////////////////////////////////////////////////////////////////////
/// Handlers

////////////////////////////////////////////////////////////////////////////////
/// Init

static esp_err_t init()
{
    return ESP_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// Public

esp_err_t webserver_start()
{
    if (server)
    {
        ESP_LOGE(TAG, "Webserver already started");
        return ESP_ERR_INVALID_STATE;
    }

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    ESP_LOGI(TAG, "Starting webserver on port %d...", config.server_port);

    CHECK(httpd_start(&server, &config));
    CHECK(init());

    ESP_LOGI(TAG, "Webserver started");

    return ESP_OK;
}

esp_err_t webserver_stop()
{
    if (!server)
    {
        ESP_LOGE(TAG, "Webserver is not running");
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGI(TAG, "Stopping webserver...");

    CHECK(httpd_stop(server));

    ESP_LOGI(TAG, "Webserver stopped");

    return ESP_OK;
}

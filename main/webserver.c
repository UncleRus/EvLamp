#include "webserver.h"
#include <esp_http_server.h>
#include <cJSON.h>
#include "settings.h"

static httpd_handle_t server = NULL;

////////////////////////////////////////////////////////////////////////////////
/// Internal

////////////////////////////////////////////////////////////////////////////////
/// Handlers
static esp_err_t handle_status(httpd_req_t *req)
{
    cJSON *j_sys_settings = sys_settings_json();

    const char *resp = cJSON_Print(j_sys_settings);
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    free((void *)resp);

    cJSON_Delete(j_sys_settings);

    return ESP_OK;
}

static const httpd_uri_t route_status = {
    .uri = "/status",
    .method = HTTP_GET,
    .handler = handle_status
};

////////////////////////////////////////////////////////////////////////////////
/// Init

static esp_err_t init()
{
    // registering handlers
    httpd_register_uri_handler(server, &route_status);

    return ESP_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// Public

esp_err_t webserver_start()
{
    if (server)
    {
        ESP_LOGE(TAG, "HTTPD already started");
        return ESP_ERR_INVALID_STATE;
    }

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    CHECK(httpd_start(&server, &config));
    CHECK(init());

    ESP_LOGI(TAG, "HTTPD started on port %d", config.server_port);

    return ESP_OK;
}

esp_err_t webserver_stop()
{
    if (!server)
    {
        ESP_LOGE(TAG, "HTTPD is not running");
        return ESP_ERR_INVALID_STATE;
    }

    CHECK(httpd_stop(server));

    ESP_LOGI(TAG, "HTTPD stopped");

    return ESP_OK;
}

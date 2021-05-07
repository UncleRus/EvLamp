#include "webserver.h"
#include <esp_ota_ops.h>
#include <esp_http_server.h>
#include <cJSON.h>
#include <mustach-cjson.h>
#include "templates.h"
#include "api.h"

static httpd_handle_t server = NULL;

////////////////////////////////////////////////////////////////////////////////
/// Internal

//static int write_cb(void *ctx, const char *buffer, size_t size)
//{
//    return httpd_resp_send_chunk((httpd_req_t *)ctx, buffer, size);
//}
//
//static cJSON *app_info()
//{
//    const esp_app_desc_t *app_desc = esp_ota_get_app_description();
//
//    cJSON *res = cJSON_CreateObject();
//    cJSON_AddStringToObject(res, "app_name", app_desc->project_name);
//    cJSON_AddStringToObject(res, "app_version", app_desc->version);
//    cJSON_AddStringToObject(res, "build_date", app_desc->date);
//    cJSON_AddStringToObject(res, "idf_ver", app_desc->idf_ver);
//
//    return res;
//}
//
//static esp_err_t render_part(httpd_req_t *req, const char *tpl)
//{
//    cJSON *params = app_info();
//
//    CHECK(mustach_cJSON_write(tpl, 0, params, 0, write_cb, req));
//
//    cJSON_Delete(params);
//
//    return ESP_OK;
//}
//
//static esp_err_t render_template(httpd_req_t *req, const char *tpl, cJSON *params)
//{
//    CHECK(render_part(req, template_header));
//    CHECK(mustach_cJSON_write(tpl, 0, params, 0, write_cb, req));
//    CHECK(render_part(req, template_footer));
//    CHECK(httpd_resp_send_chunk(req, NULL, 0));
//    free(params);
//
//    return ESP_OK;
//}


////////////////////////////////////////////////////////////////////////////////
/// Init

static esp_err_t init()
{
    // registering handlers
    CHECK(api_init(server));

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
    //config.uri_match_fn = httpd_uri_match_wildcard;
    config.max_uri_handlers = 100;
    config.stack_size = HTTPD_STACK_SIZE;
    config.lru_purge_enable = true;

    CHECK(httpd_start(&server, &config));
    CHECK(init());

    ESP_LOGI(TAG, "HTTPD started on port %d, free mem: %d bytes", config.server_port, esp_get_free_heap_size());

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

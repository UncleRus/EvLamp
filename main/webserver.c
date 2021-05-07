#include "webserver.h"
#include <esp_http_server.h>
#include <cJSON.h>
//#include <mustach-cjson.h>
#include "api.h"
#include "embed.h"

static httpd_handle_t server = NULL;

////////////////////////////////////////////////////////////////////////////////
/// Internal

//static int write_cb(void *ctx, const char *buffer, size_t size)
//{
//    return httpd_resp_send_chunk((httpd_req_t *)ctx, buffer, size);
//}
//
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
/// Handlers

esp_err_t get_jquery(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/javascript");
    return httpd_resp_sendstr(req, embed_jquery_js);
}

static const httpd_uri_t route_get_jquery = {
    .uri = "/jquery.js",
    .method = HTTP_GET,
    .handler = get_jquery
};

esp_err_t get_styles(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/css");
    return httpd_resp_sendstr(req, embed_styles_css);
}

static const httpd_uri_t route_get_styles = {
    .uri = "/styles.css",
    .method = HTTP_GET,
    .handler = get_styles
};

////////////////////////////////////////////////////////////////////////////////
/// Init

static esp_err_t init()
{
    // registering handlers
    CHECK(api_init(server));

    CHECK(httpd_register_uri_handler(server, &route_get_jquery));
    CHECK(httpd_register_uri_handler(server, &route_get_styles));

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

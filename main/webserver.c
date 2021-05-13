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

#define DECLARE_EMBED_HANDLER(NAME, URI, CT) \
    esp_err_t get_##NAME(httpd_req_t *req) { \
        httpd_resp_set_type(req, CT); \
        return httpd_resp_sendstr(req, embed_##NAME); \
    } \
    static const httpd_uri_t route_get_##NAME = { .uri = (URI), .method = HTTP_GET, .handler = get_##NAME }

////////////////////////////////////////////////////////////////////////////////
/// Handlers

DECLARE_EMBED_HANDLER(jquery_js, "/jquery.js", "text/javascript");
DECLARE_EMBED_HANDLER(jquery_ui_js, "/jquery-ui.js", "text/javascript");
DECLARE_EMBED_HANDLER(jquery_ui_css, "/jquery-ui.css", "text/css");
DECLARE_EMBED_HANDLER(jquery_ui_structure_css, "/jquery-ui.structure.css", "text/css");
DECLARE_EMBED_HANDLER(jquery_ui_theme_css, "/jquery-ui.theme.css", "text/css");
DECLARE_EMBED_HANDLER(styles_css, "/styles.css", "text/css");

DECLARE_EMBED_HANDLER(index_html, "/index.html", "text/html");
static const httpd_uri_t route_get_root = { .uri = "/", .method = HTTP_GET, .handler = get_index_html };

////////////////////////////////////////////////////////////////////////////////
/// Init

static esp_err_t init()
{
    // API handlers
    CHECK(api_init(server));

    // Static file handlers
    CHECK(httpd_register_uri_handler(server, &route_get_jquery_js));
    CHECK(httpd_register_uri_handler(server, &route_get_jquery_ui_js));
    CHECK(httpd_register_uri_handler(server, &route_get_jquery_ui_css));
    CHECK(httpd_register_uri_handler(server, &route_get_jquery_ui_structure_css));
    CHECK(httpd_register_uri_handler(server, &route_get_jquery_ui_theme_css));
    CHECK(httpd_register_uri_handler(server, &route_get_styles_css));

    CHECK(httpd_register_uri_handler(server, &route_get_index_html));
    CHECK(httpd_register_uri_handler(server, &route_get_root));

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

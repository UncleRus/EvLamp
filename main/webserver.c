#include "webserver.h"
#include <esp_http_server.h>
#include "api.h"
#include "embed.h"

static httpd_handle_t server = NULL;

////////////////////////////////////////////////////////////////////////////////
/// Internal

#define DECLARE_EMBED_HANDLER(NAME, URI, CT) \
    esp_err_t get_##NAME(httpd_req_t *req) { \
        httpd_resp_set_type(req, CT); \
        return httpd_resp_sendstr(req, embed_##NAME); \
    } \
    static const httpd_uri_t route_get_##NAME = { .uri = (URI), .method = HTTP_GET, .handler = get_##NAME }

////////////////////////////////////////////////////////////////////////////////
/// Handlers

DECLARE_EMBED_HANDLER(jquery_js, "/jquery.js", "text/javascript");
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

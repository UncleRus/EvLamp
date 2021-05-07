#include "api.h"
#include "settings.h"
#include "effect.h"
#include "surface.h"

static esp_err_t respond_json(httpd_req_t *req, cJSON *resp)
{
    httpd_resp_set_type(req, "application/json");

    char *txt = cJSON_Print(resp);
    esp_err_t res = httpd_resp_sendstr(req, txt);
    free(txt);

    cJSON_Delete(resp);

    return res;
}

static esp_err_t respond_api(httpd_req_t *req, esp_err_t err)
{
    cJSON *resp = cJSON_CreateObject();
    cJSON_AddNumberToObject(resp, "result", err);
    cJSON_AddStringToObject(resp, "name", esp_err_to_name(err));

    return respond_json(req, resp);
}

static cJSON *effect_params(size_t effect)
{
    cJSON *params = cJSON_CreateArray();
    for (size_t c = 0; c < effects[effect].params_count; c++)
    {
        cJSON *p = cJSON_CreateObject();
        cJSON_AddItemToArray(params, p);
        cJSON_AddStringToObject(p, "name", effects[effect].params[c].name);
        cJSON_AddNumberToObject(p, "min", effects[effect].params[c].min);
        cJSON_AddNumberToObject(p, "max", effects[effect].params[c].max);
        cJSON_AddNumberToObject(p, "default", effects[effect].params[c].def);
        cJSON_AddNumberToObject(p, "value", effects[effect].params[c].value);
    }
    return params;
}
////////////////////////////////////////////////////////////////////////////////

static esp_err_t get_settings_reset(httpd_req_t *req)
{
    return respond_api(req, sys_settings_reset());
}

static const httpd_uri_t route_get_settings_reset = {
    .uri = "/api/settings/reset",
    .method = HTTP_GET,
    .handler = get_settings_reset
};

////////////////////////////////////////////////////////////////////////////////

static esp_err_t get_settings_wifi(httpd_req_t *req)
{
    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "mode", sys_settings.wifi.mode);

    cJSON *ip = cJSON_CreateObject();
    cJSON_AddItemToObject(res, "ip", ip);
    cJSON_AddBoolToObject(ip, "dhcp", sys_settings.wifi.ip.dhcp);
    cJSON_AddStringToObject(ip, "ip", sys_settings.wifi.ip.ip);
    cJSON_AddStringToObject(ip, "netmask", sys_settings.wifi.ip.netmask);
    cJSON_AddStringToObject(ip, "gateway", sys_settings.wifi.ip.gateway);
    cJSON_AddStringToObject(ip, "dns", sys_settings.wifi.ip.dns);

    cJSON *ap = cJSON_CreateObject();
    cJSON_AddItemToObject(res, "ap", ap);
    cJSON_AddStringToObject(ap, "ssid", (char *)sys_settings.wifi.ap.ssid);
    cJSON_AddNumberToObject(ap, "channel", sys_settings.wifi.ap.channel);
    cJSON_AddStringToObject(ap, "password", (char *)sys_settings.wifi.ap.password);

    cJSON *sta = cJSON_CreateObject();
    cJSON_AddItemToObject(res, "sta", sta);
    cJSON_AddStringToObject(sta, "ssid", (char *)sys_settings.wifi.sta.ssid);
    cJSON_AddStringToObject(sta, "password", (char *)sys_settings.wifi.sta.password);

    return respond_json(req, res);
}

static const httpd_uri_t route_get_settings_wifi = {
    .uri = "/api/settings/wifi",
    .method = HTTP_GET,
    .handler = get_settings_wifi
};

////////////////////////////////////////////////////////////////////////////////

static esp_err_t get_settings_leds(httpd_req_t *req)
{
    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "width", sys_settings.leds.width);
    cJSON_AddNumberToObject(res, "height", sys_settings.leds.height);
    cJSON_AddNumberToObject(res, "type", sys_settings.leds.type);
    cJSON_AddNumberToObject(res, "gpio", sys_settings.leds.gpio);
    cJSON_AddNumberToObject(res, "current_limit", sys_settings.leds.current_limit);

    return respond_json(req, res);
}

static const httpd_uri_t route_get_settings_leds = {
    .uri = "/api/settings/leds",
    .method = HTTP_GET,
    .handler = get_settings_leds
};

////////////////////////////////////////////////////////////////////////////////

static esp_err_t get_reboot(httpd_req_t *req)
{
    esp_restart();

    return ESP_OK; // dummy
}

static const httpd_uri_t route_get_reboot = {
    .uri = "/api/reboot",
    .method = HTTP_GET,
    .handler = get_reboot
};

////////////////////////////////////////////////////////////////////////////////

static esp_err_t get_effects(httpd_req_t *req)
{
    cJSON *res = cJSON_CreateArray();
    for (size_t i = 0; i < effects_count; i++)
    {
        cJSON *e = cJSON_CreateObject();
        cJSON_AddItemToArray(res, e);
        cJSON_AddStringToObject(e, "name", effects[i].name);
        cJSON_AddItemToObject(e, "params", effect_params(i));
    }

    return respond_json(req, res);
}

static const httpd_uri_t route_get_effects = {
    .uri = "/api/effects",
    .method = HTTP_GET,
    .handler = get_effects
};

////////////////////////////////////////////////////////////////////////////////

static esp_err_t get_effects_reset(httpd_req_t *req)
{
    return respond_api(req, effects_reset());
}

static const httpd_uri_t route_get_effects_reset = {
    .uri = "/api/effects/reset",
    .method = HTTP_GET,
    .handler = get_effects_reset
};

////////////////////////////////////////////////////////////////////////////////

static esp_err_t get_lamp_state(httpd_req_t *req)
{
    cJSON *res = cJSON_CreateObject();
    cJSON_AddBoolToObject(res, "on", surface_is_playing());
    cJSON_AddNumberToObject(res, "effect", vol_settings.effect);
    cJSON_AddNumberToObject(res, "brightness", vol_settings.brightness);
    cJSON_AddNumberToObject(res, "fps", vol_settings.fps);

    return respond_json(req, res);
}

static const httpd_uri_t route_get_lamp_state = {
    .uri = "/api/lamp/state",
    .method = HTTP_GET,
    .handler = get_lamp_state
};

////////////////////////////////////////////////////////////////////////////////

esp_err_t api_init(httpd_handle_t server)
{
    CHECK(httpd_register_uri_handler(server, &route_get_settings_reset));
    CHECK(httpd_register_uri_handler(server, &route_get_settings_wifi));
    CHECK(httpd_register_uri_handler(server, &route_get_settings_leds));
    CHECK(httpd_register_uri_handler(server, &route_get_reboot));
    CHECK(httpd_register_uri_handler(server, &route_get_effects));
    CHECK(httpd_register_uri_handler(server, &route_get_effects_reset));
    CHECK(httpd_register_uri_handler(server, &route_get_lamp_state));

    return ESP_OK;
}

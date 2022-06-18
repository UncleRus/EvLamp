#include "api.h"
#include <led_strip.h>
#include <lwip/ip_addr.h>
#include <esp_ota_ops.h>
#include <driver/gpio.h>
#include "settings.h"
#include "effect.h"
#include "surface.h"

static esp_err_t respond_json(httpd_req_t *req, cJSON *resp)
{
    httpd_resp_set_type(req, "application/json");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");

    char *txt = cJSON_Print(resp);
    esp_err_t res = httpd_resp_sendstr(req, txt);
    free(txt);

    cJSON_Delete(resp);

    return res;
}

static esp_err_t respond_api(httpd_req_t *req, esp_err_t err, const char *message)
{
    cJSON *resp = cJSON_CreateObject();
    cJSON_AddNumberToObject(resp, "result", err);
    cJSON_AddStringToObject(resp, "name", esp_err_to_name(err));
    cJSON_AddStringToObject(resp, "message", message ? message : "");

    return respond_json(req, resp);
}

static cJSON *effect_params_json(size_t effect)
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
        cJSON_AddNumberToObject(p, "type", effects[effect].params[c].type);
    }
    return params;
}

static esp_err_t parse_post_json(httpd_req_t *req, const char **msg, cJSON **json)
{
    *msg = NULL;
    *json = NULL;
    esp_err_t err = ESP_OK;

    char *buf = NULL;

    if (!req->content_len)
    {
        err = ESP_ERR_INVALID_ARG;
        *msg = "No POST data";
        goto exit;
    }

    if (req->content_len >= MAX_POST_SIZE)
    {
        err = ESP_ERR_NO_MEM;
        *msg = "POST data too big";
        goto exit;
    }

    buf = malloc(req->content_len + 1);
    if (!buf)
    {
        err = ESP_ERR_NO_MEM;
        *msg = "Out of memory";
        goto exit;
    }

    int res = httpd_req_recv(req, buf, req->content_len);
    if (res != req->content_len)
    {
        err = ESP_FAIL;
        *msg = res ? "Socket error" : "Connection closed by peer";
        goto exit;
    }
    buf[req->content_len] = 0;

    *json = cJSON_Parse(buf);
    if (!*json)
    {
        err = ESP_ERR_INVALID_ARG;
        *msg = "Invalid JSON";
    }

exit:
    if (buf)
        free(buf);
    return err;
}

////////////////////////////////////////////////////////////////////////////////

static esp_err_t get_info(httpd_req_t *req)
{
    const esp_app_desc_t *app_desc = esp_ota_get_app_description();

    cJSON *res = cJSON_CreateObject();
    cJSON_AddStringToObject(res, "app_name", app_desc->project_name);
    cJSON_AddStringToObject(res, "app_version", app_desc->version);
    cJSON_AddStringToObject(res, "build_date", app_desc->date);
    cJSON_AddStringToObject(res, "idf_ver", app_desc->idf_ver);

    return respond_json(req, res);
}

static const httpd_uri_t route_get_info = {
    .uri = "/api/info",
    .method = HTTP_GET,
    .handler = get_info
};

////////////////////////////////////////////////////////////////////////////////

static esp_err_t get_settings_reset(httpd_req_t *req)
{
    esp_err_t res = sys_settings_reset();
    if (res == ESP_OK)
        res = vol_settings_reset();
    return respond_api(req, res, res == ESP_OK ? "Reboot to apply" : NULL);
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

static esp_err_t post_settings_wifi(httpd_req_t *req)
{
    const char *msg = NULL;
    cJSON *json = NULL;

    esp_err_t err = parse_post_json(req, &msg, &json);
    if (err != ESP_OK)
        goto exit;

    cJSON *mode_item = cJSON_GetObjectItem(json, "mode");
    if (!cJSON_IsNumber(mode_item))
    {
        msg = "Item `mode` not found or invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }

    cJSON *ip = cJSON_GetObjectItem(json, "ip");
    if (!ip)
    {
        msg = "Object `ip` not found or invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *ip_dhcp_item = cJSON_GetObjectItem(ip, "dhcp");
    if (!cJSON_IsBool(ip_dhcp_item))
    {
        msg = "Object `ip.dhcp` not found or invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *ip_ip_item = cJSON_GetObjectItem(ip, "ip");
    if (!cJSON_IsString(ip_ip_item) || ipaddr_addr(cJSON_GetStringValue(ip_ip_item)) == IPADDR_NONE)
    {
        msg = "Item `ip.ip` not found, invalid or not an IP address";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *ip_netmask_item = cJSON_GetObjectItem(ip, "netmask");
    if (!cJSON_IsString(ip_netmask_item) || ipaddr_addr(cJSON_GetStringValue(ip_netmask_item)) == IPADDR_NONE)
    {
        msg = "Item `ip.netmask` not found, invalid or not an IP address";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *ip_gateway_item = cJSON_GetObjectItem(ip, "gateway");
    if (!cJSON_IsString(ip_gateway_item) || ipaddr_addr(cJSON_GetStringValue(ip_gateway_item)) == IPADDR_NONE)
    {
        msg = "Item `ip.gateway` not found, invalid or not an IP address";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *ip_dns_item = cJSON_GetObjectItem(ip, "dns");
    if (!cJSON_IsString(ip_dns_item) || ipaddr_addr(cJSON_GetStringValue(ip_dns_item)) == IPADDR_NONE)
    {
        msg = "Item `ip.dns` not found, invalid or not an IP address";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }

    cJSON *ap = cJSON_GetObjectItem(json, "ap");
    if (!ap)
    {
        msg = "Object `ap` not found or invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *ap_ssid_item = cJSON_GetObjectItem(ap, "ssid");
    if (!cJSON_IsString(ap_ssid_item))
    {
        msg = "Item `ap.ssid` not found or invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *ap_channel_item = cJSON_GetObjectItem(ap, "channel");
    if (!cJSON_IsNumber(ap_channel_item))
    {
        msg = "Item `ap.channel` not found or invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *ap_password_item = cJSON_GetObjectItem(ap, "password");
    if (!cJSON_IsString(ap_password_item))
    {
        msg = "Item `ap.password` not found or invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }

    cJSON *sta = cJSON_GetObjectItem(json, "sta");
    if (!sta)
    {
        msg = "Object `sta` not found or invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *sta_ssid_item = cJSON_GetObjectItem(sta, "ssid");
    if (!cJSON_IsString(sta_ssid_item))
    {
        msg = "Item `sta.ssid` not found or invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *sta_password_item = cJSON_GetObjectItem(sta, "password");
    if (!cJSON_IsString(sta_password_item))
    {
        msg = "Item `sta.password` not found or invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }

    uint8_t mode = (uint8_t)cJSON_GetNumberValue(mode_item);
    if (mode != WIFI_MODE_AP && mode != WIFI_MODE_STA)
    {
        msg = "Invalid mode value";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    const char *ap_ssid = cJSON_GetStringValue(ap_ssid_item);
    size_t len = strlen(ap_ssid);
    if (!len || len >= sizeof(sys_settings.wifi.ap.ssid))
    {
        msg = "Invalid ap.ssid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    uint8_t ap_channel = (uint8_t)cJSON_GetNumberValue(ap_channel_item);
    if (ap_channel > 32)
    {
        msg = "Invalid ap.channel";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    const char *ap_password = cJSON_GetStringValue(ap_password_item);
    len = strlen(ap_password);
    if (len >= sizeof(sys_settings.wifi.ap.password))
    {
        msg = "ap.password too long";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    const char *sta_ssid = cJSON_GetStringValue(sta_ssid_item);
    len = strlen(sta_ssid);
    if (!len || len >= sizeof(sys_settings.wifi.sta.ssid))
    {
        msg = "Invalid sta.ssid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    const char *sta_password = cJSON_GetStringValue(sta_password_item);
    len = strlen(sta_password);
    if (len >= sizeof(sys_settings.wifi.sta.password))
    {
        msg = "sta.password too long";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }

    sys_settings.wifi.mode = mode;
    sys_settings.wifi.ip.dhcp = cJSON_IsTrue(ip_dhcp_item);
    strncpy(sys_settings.wifi.ip.ip, cJSON_GetStringValue(ip_ip_item), sizeof(sys_settings.wifi.ip.ip) - 1);
    strncpy(sys_settings.wifi.ip.netmask, cJSON_GetStringValue(ip_netmask_item), sizeof(sys_settings.wifi.ip.netmask) - 1);
    strncpy(sys_settings.wifi.ip.gateway, cJSON_GetStringValue(ip_gateway_item), sizeof(sys_settings.wifi.ip.gateway) - 1);
    strncpy(sys_settings.wifi.ip.dns, cJSON_GetStringValue(ip_dns_item), sizeof(sys_settings.wifi.ip.dns) - 1);
    sys_settings.wifi.ap.channel = ap_channel;
    strncpy((char *)sys_settings.wifi.ap.ssid, ap_ssid, sizeof(sys_settings.wifi.ap.ssid) - 1);
    strncpy((char *)sys_settings.wifi.ap.password, ap_password, sizeof(sys_settings.wifi.ap.password) - 1);
    strncpy((char *)sys_settings.wifi.sta.ssid, sta_ssid, sizeof(sys_settings.wifi.sta.ssid) - 1);
    strncpy((char *)sys_settings.wifi.sta.password, sta_password, sizeof(sys_settings.wifi.sta.password) - 1);

    err = sys_settings_save();
    msg = err != ESP_OK
            ? "Error saving system settings"
            : "Settings saved, reboot to apply";

exit:
    if (json) cJSON_Delete(json);
    return respond_api(req, err, msg);
}

static const httpd_uri_t route_post_settings_wifi = {
    .uri = "/api/settings/wifi",
    .method = HTTP_POST,
    .handler = post_settings_wifi
};

////////////////////////////////////////////////////////////////////////////////

static esp_err_t get_settings_leds(httpd_req_t *req)
{
    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "block_width", sys_settings.leds.block_width);
    cJSON_AddNumberToObject(res, "block_height", sys_settings.leds.block_height);
    cJSON_AddNumberToObject(res, "h_blocks", sys_settings.leds.h_blocks);
    cJSON_AddNumberToObject(res, "v_blocks", sys_settings.leds.v_blocks);
    cJSON_AddNumberToObject(res, "type", sys_settings.leds.type);
    cJSON_AddNumberToObject(res, "rotation", sys_settings.leds.rotation);
    cJSON_AddNumberToObject(res, "current_limit", sys_settings.leds.current_limit);
    cJSON_AddItemToObject(res, "gpio", cJSON_CreateIntArray((const int *)sys_settings.leds.gpio, MAX_SURFACE_BLOCKS));

    return respond_json(req, res);
}

static const httpd_uri_t route_get_settings_leds = {
    .uri = "/api/settings/leds",
    .method = HTTP_GET,
    .handler = get_settings_leds
};

////////////////////////////////////////////////////////////////////////////////

static esp_err_t post_settings_leds(httpd_req_t *req)
{
    const char *msg = NULL;
    cJSON *json = NULL;

    int gpio_array[MAX_SURFACE_BLOCKS] = { 0 };

    esp_err_t err = parse_post_json(req, &msg, &json);
    if (err != ESP_OK)
        goto exit;

    cJSON *block_width_item = cJSON_GetObjectItem(json, "block_width");
    if (!cJSON_IsNumber(block_width_item))
    {
        msg = "Item `block_width` not found or invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *block_height_item = cJSON_GetObjectItem(json, "block_height");
    if (!cJSON_IsNumber(block_height_item))
    {
        msg = "Item `block_height` not found or invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *h_blocks_item = cJSON_GetObjectItem(json, "h_blocks");
    if (!cJSON_IsNumber(h_blocks_item))
    {
        msg = "Item `h_blocks` not found or invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *v_blocks_item = cJSON_GetObjectItem(json, "v_blocks");
    if (!cJSON_IsNumber(v_blocks_item))
    {
        msg = "Item `v_blocks` not found or invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *type_item = cJSON_GetObjectItem(json, "type");
    if (!cJSON_IsNumber(type_item))
    {
        msg = "Item `type` not found or invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *rotation_item = cJSON_GetObjectItem(json, "rotation");
    if (rotation_item && !cJSON_IsNumber(rotation_item))
    {
        msg = "Item `rotation` invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *limit_item = cJSON_GetObjectItem(json, "current_limit");
    if (!cJSON_IsNumber(limit_item))
    {
        msg = "Item `current_limit` not found or invalid";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    cJSON *gpio_item = cJSON_GetObjectItem(json, "gpio");


    size_t block_width = (size_t)cJSON_GetNumberValue(block_width_item);
    size_t block_height = (size_t)cJSON_GetNumberValue(block_height_item);
    if (block_width < MIN_BLOCK_SIZE || block_width > MAX_BLOCK_SIZE
        || block_height < MIN_BLOCK_SIZE || block_height > MAX_BLOCK_SIZE)
    {
        msg = "Invalid single block dimensions";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }
    if (block_height * block_width > MAX_BLOCK_LEDS)
    {
        msg = "Too much LEDs in single block";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }

    uint8_t h_blocks = (uint8_t)cJSON_GetNumberValue(h_blocks_item);
    uint8_t v_blocks = (uint8_t)cJSON_GetNumberValue(v_blocks_item);
    if (h_blocks < 1 || v_blocks < 1 || h_blocks * v_blocks > MAX_SURFACE_BLOCKS)
    {
        msg = "Invalid number of blocks";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }

    uint8_t type = (uint8_t)cJSON_GetNumberValue(type_item);
    if (type >= LED_STRIP_TYPE_MAX)
    {
        msg = "Invalid LED type";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }

    uint8_t rotation = 0;
    if (rotation_item)
    {
        rotation = (uint8_t)cJSON_GetNumberValue(rotation_item);
        if (rotation > SURFACE_ROTATION_270)
        {
            msg = "Invalid rotation value";
            err = ESP_ERR_INVALID_ARG;
            goto exit;
        }
    }

    size_t leds = block_width * block_height * h_blocks * v_blocks;
    uint32_t limit = (uint32_t)cJSON_GetNumberValue(limit_item);
    if (limit < leds * CONFIG_EL_SINGLE_LED_CURRENT_MA / 50)
    {
        msg = "Current limit too low";
        err = ESP_ERR_INVALID_ARG;
        goto exit;
    }

    int gpio_count = cJSON_GetArraySize(gpio_item);
    if (gpio_count)
    {
        if (gpio_count != h_blocks * v_blocks)
        {
            msg = "Invalid GPIO count";
            err = ESP_ERR_INVALID_ARG;
            goto exit;
        }

        for (int i = 0; i < gpio_count; i++)
        {
            int gpio_num = (int) cJSON_GetNumberValue(cJSON_GetArrayItem(gpio_item, i));
            if (gpio_num == CONFIG_EL_BUTTON_RESET_GPIO || gpio_num == CONFIG_EL_BUTTON_MAIN_GPIO
                || gpio_num == CONFIG_EL_BUTTON_UP_GPIO || gpio_num == CONFIG_EL_BUTTON_DOWN_GPIO)
            {
                msg = "GPIO occupied by button";
                err = ESP_ERR_INVALID_ARG;
                goto exit;
            }
            if (gpio_num < 0 || gpio_num >= GPIO_NUM_MAX)
            {
                msg = "Invalid GPIO number";
                err = ESP_ERR_INVALID_ARG;
                goto exit;
            }
            for (int b = 0; b < i; b++)
                if (gpio_array[b] == gpio_num)
                {
                    msg = "GPIO already occupied by another LED block";
                    err = ESP_ERR_INVALID_ARG;
                    goto exit;
                }
            gpio_array[i] = gpio_num;
        }
    }

    sys_settings.leds.block_width = block_width;
    sys_settings.leds.block_height = block_height;
    sys_settings.leds.h_blocks = h_blocks;
    sys_settings.leds.v_blocks = v_blocks;
    sys_settings.leds.type = type;
    if (rotation_item)
        sys_settings.leds.rotation = rotation;
    sys_settings.leds.current_limit = limit;
    for (int i = 0; i < gpio_count; i++)
        sys_settings.leds.gpio[i] = gpio_array[i];

    err = sys_settings_save();
    msg = err != ESP_OK
            ? "Error saving system settings"
            : "Settings saved, reboot to apply";

exit:
    if (json) cJSON_Delete(json);
    return respond_api(req, err, msg);
}

static const httpd_uri_t route_post_settings_leds = {
    .uri = "/api/settings/leds",
    .method = HTTP_POST,
    .handler = post_settings_leds
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
        cJSON_AddItemToObject(e, "params", effect_params_json(i));
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
    esp_err_t res = effects_reset();
    return respond_api(req, res, res == ESP_OK ?
            "Effect settings reset" : "Error resetting effects");
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

static esp_err_t post_lamp_state(httpd_req_t *req)
{
    const char *msg = NULL;
    cJSON *json = NULL;

    esp_err_t err = parse_post_json(req, &msg, &json);
    if (err != ESP_OK)
        goto exit;

    cJSON *on_item = cJSON_GetObjectItem(json, "on");
    if (on_item && !cJSON_IsBool(on_item))
    {
        err = ESP_ERR_INVALID_ARG;
        msg = "Invalid `on` item";
        goto exit;
    }
    cJSON *effect_item = cJSON_GetObjectItem(json, "effect");
    if (effect_item && !cJSON_IsNumber(effect_item))
    {
        err = ESP_ERR_INVALID_ARG;
        msg = "Invalid `effect` item";
        goto exit;
    }
    cJSON *brightness_item = cJSON_GetObjectItem(json, "brightness");
    if (brightness_item && !cJSON_IsNumber(brightness_item))
    {
        err = ESP_ERR_INVALID_ARG;
        msg = "Invalid `brightness` item";
        goto exit;
    }
    cJSON *fps_item = cJSON_GetObjectItem(json, "fps");
    if (fps_item && !cJSON_IsNumber(fps_item))
    {
        err = ESP_ERR_INVALID_ARG;
        msg = "Invalid `fps` item";
        goto exit;
    }

    if (effect_item)
    {
        err = surface_set_effect((size_t)cJSON_GetNumberValue(effect_item));
        if (err != ESP_OK)
        {
            msg = "Effect setting error";
            goto exit;
        }
    }
    if (on_item)
    {
        err = cJSON_IsTrue(on_item) ? surface_play() : surface_stop();
        if (err != ESP_OK)
        {
            msg = "On/off lamp error";
            goto exit;
        }
    }
    if (brightness_item)
    {
        err = surface_set_brightness((uint8_t)cJSON_GetNumberValue(brightness_item));
        if (err != ESP_OK)
        {
            msg = "Brightness setting error";
            goto exit;
        }
    }
    if (fps_item)
    {
        err = surface_set_fps((uint8_t)cJSON_GetNumberValue(fps_item));
        if (err != ESP_OK)
        {
            msg = "FPS setting error";
            goto exit;
        }
    }

exit:
    if (json) cJSON_Delete(json);
    return respond_api(req, err, msg);
}

static const httpd_uri_t route_post_lamp_state = {
    .uri = "/api/lamp/state",
    .method = HTTP_POST,
    .handler = post_lamp_state
};

////////////////////////////////////////////////////////////////////////////////

static esp_err_t get_lamp_effect(httpd_req_t *req)
{
    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "effect", vol_settings.effect);
    cJSON_AddItemToObject(res, "params", effect_params_json(vol_settings.effect));
    return respond_json(req, res);
}

static const httpd_uri_t route_get_lamp_effect = {
    .uri = "/api/lamp/effect",
    .method = HTTP_GET,
    .handler = get_lamp_effect
};

////////////////////////////////////////////////////////////////////////////////

static esp_err_t post_lamp_effect(httpd_req_t *req)
{
    const char *msg = NULL;
    cJSON *json = NULL;

    esp_err_t err = parse_post_json(req, &msg, &json);
    if (err != ESP_OK)
        goto exit;

    size_t effect = vol_settings.effect;

    cJSON *effect_item = cJSON_GetObjectItem(json, "effect");
    if (cJSON_IsNumber(effect_item))
    {
        effect = (size_t)cJSON_GetNumberValue(effect_item);
        if (effect >= effects_count)
        {
            err = ESP_ERR_INVALID_ARG;
            msg = "Invalid `effect` value";
            goto exit;
        }
    }

    cJSON *params = cJSON_GetObjectItem(json, "params");
    if (!cJSON_IsArray(params))
    {
        err = ESP_ERR_INVALID_ARG;
        msg = "Invalid JSON, `params` must be array";
        goto exit;
    }

    if (cJSON_GetArraySize(params) != effects[effect].params_count)
    {
        err = ESP_ERR_INVALID_ARG;
        msg = "Invalid JSON, array size is not equal to effect params count";
        goto exit;
    }

    for (size_t p = 0; p < effects[effect].params_count; p++)
    {
        cJSON *item = cJSON_GetArrayItem(params, p);
        if (!cJSON_IsNumber(item))
        {
            err = ESP_ERR_INVALID_ARG;
            msg = "Invalid JSON, parameter value must be numeric";
            goto exit;
        }
        err = effect_param_set(effect, p, (uint8_t)cJSON_GetNumberValue(item));
        if (err != ESP_OK)
        {
            msg = "Error set parameter";
            goto exit;
        }
    }

exit:
    if (json) cJSON_Delete(json);
    return respond_api(req, err, msg);
}

static const httpd_uri_t route_post_lamp_effect = {
    .uri = "/api/lamp/effect",
    .method = HTTP_POST,
    .handler = post_lamp_effect
};

////////////////////////////////////////////////////////////////////////////////

esp_err_t api_init(httpd_handle_t server)
{
    CHECK(httpd_register_uri_handler(server, &route_get_info));
    CHECK(httpd_register_uri_handler(server, &route_get_settings_reset));
    CHECK(httpd_register_uri_handler(server, &route_get_settings_wifi));
    CHECK(httpd_register_uri_handler(server, &route_post_settings_wifi));
    CHECK(httpd_register_uri_handler(server, &route_get_settings_leds));
    CHECK(httpd_register_uri_handler(server, &route_post_settings_leds));
    CHECK(httpd_register_uri_handler(server, &route_get_reboot));
    CHECK(httpd_register_uri_handler(server, &route_get_effects));
    CHECK(httpd_register_uri_handler(server, &route_get_effects_reset));
    CHECK(httpd_register_uri_handler(server, &route_get_lamp_state));
    CHECK(httpd_register_uri_handler(server, &route_post_lamp_state));
    CHECK(httpd_register_uri_handler(server, &route_get_lamp_effect));
    CHECK(httpd_register_uri_handler(server, &route_post_lamp_effect));

    return ESP_OK;
}

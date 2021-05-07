#ifndef MAIN_API_H_
#define MAIN_API_H_

#include "common.h"
#include <esp_http_server.h>

// API Handlers
//
// GET  /api/info                   -> Application name, version, build date etc
// GET  /api/settings/reset         -> reset wifi and led settings to defaults
// GET  /api/settings/wifi          -> return current wifi settings
// POST /api/settings/wifi          -> set wifi settings
// GET  /api/settings/leds          -> return led settings
// POST /api/settings/leds          -> set led settings
// GET  /api/reboot                 -> reboot lamp
// GET  /api/effects                -> get effects list
// GET  /api/effects/reset          -> reset effect settings to defaults
// GET  /api/lamp/state             -> get current lamp state, effect, brightness and FPS
// POST /api/lamp/state             -> set current lamp state, effect, brightness and FPS
// GET  /api/lamp/effect            -> get current effect settings
// POST /api/lamp/effect            -> set current effect settings

esp_err_t api_init(httpd_handle_t server);

#endif /* MAIN_API_H_ */

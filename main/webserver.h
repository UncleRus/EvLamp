#ifndef MAIN_WEBSERVER_H_
#define MAIN_WEBSERVER_H_

#include "common.h"
#include <esp_http_server.h>

esp_err_t webserver_start();
esp_err_t webserver_stop();

#endif /* MAIN_WEBSERVER_H_ */

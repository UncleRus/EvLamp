#ifndef MAIN_OTA_H_
#define MAIN_OTA_H_

#include "common.h"
#include <cJSON.h>

esp_err_t ota_get_available(cJSON **res);

esp_err_t ota_run(const char *url);

esp_err_t ota_rollback();

#endif /* MAIN_OTA_H_ */

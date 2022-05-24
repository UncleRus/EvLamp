#ifndef EVLAMP_COMMON_H_
#define EVLAMP_COMMON_H_

#include <stddef.h>
#include <stdlib.h>
#include <esp_log.h>
#include <esp_err.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/event_groups.h>

#include "config.h"

extern const char *TAG;

#define CHECK(x) \
    do { \
        esp_err_t __; \
        if ((__ = (x)) != ESP_OK) \
            return __; \
    } while (0)

#define CHECK_ARG(VAL) \
    do { \
        if (!(VAL)) \
            return ESP_ERR_INVALID_ARG; \
    } while (0)

#define CHECK_LOGE(x, msg, ...) \
    do { \
        esp_err_t __; \
        if ((__ = (x)) != ESP_OK) { \
            ESP_LOGE(TAG, msg, ## __VA_ARGS__); \
            return __; \
        } \
    } while (0)

#define MAX(a, b) \
   ({ __typeof__(a) _a = (a); \
      __typeof__(b) _b = (b); \
      _a > _b ? _a : _b; })

#define MIN(a, b) \
   ({ __typeof__(a) _a = (a); \
      __typeof__(b) _b = (b); \
      _a < _b ? _a : _b; })

#endif /* EVLAMP_COMMON_H_ */

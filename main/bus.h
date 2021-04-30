#ifndef MAIN_BUS_H_
#define MAIN_BUS_H_

#include "common.h"

typedef enum {
    EVENT_NETWORK_UP = 0,
    EVENT_NETWORK_DOWN,
    EVENT_BUTTON_PRESSED,
    EVENT_BUTTON_LONG_PRESSED,
    EVENT_BUTTON_RELEASED,
    EVENT_BUTTON_CLICKED,
    EVENT_TIMER
} event_type_t;

typedef struct
{
    event_type_t type;
    void *data;
    size_t size;
} event_t;

esp_err_t bus_init();

esp_err_t bus_send_event(event_type_t type, void *data, size_t size);

esp_err_t bus_receive_event(event_t *e, size_t timeout_ms);

#endif /* MAIN_BUS_H_ */

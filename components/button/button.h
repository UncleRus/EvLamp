#ifndef __COMPONENTS_BUTTON_H__
#define __COMPONENTS_BUTTON_H__

#include <stddef.h>
#include <driver/gpio.h>
#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct button_s button_t;

typedef enum {
    BUTTON_PRESSED = 0,
    BUTTON_RELEASED,
    BUTTON_CLICKED,
    BUTTON_PRESSED_LONG,
} button_state_t;

typedef void (*button_event_cb_t)(button_t *btn, button_state_t e);

struct button_s
{
    gpio_num_t gpio;
    bool internal_pull;
    uint8_t pressed_level;
    bool autorepeat;
    button_event_cb_t callback;
    struct {
        button_state_t state;
        uint32_t pressed_time;
        uint32_t repeating_time;
    } internal;
};

esp_err_t button_init(button_t *btn);

esp_err_t button_done(button_t *btn);

#ifdef __cplusplus
}
#endif

/**@}*/

#endif /* __COMPONENTS_BUTTON_H__ */

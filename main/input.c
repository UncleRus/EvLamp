#include "input.h"
#include <driver/gpio.h>
#include <esp_timer.h>
#include "bus.h"

#if CONFIG_EL_BUTTON_ENABLE

#define TIMER_INTERVAL_US (200 * 1000) // 200ms = 5Hz
#define DEAD_TIME_US (10 * 1000) // 10ms antijitter

typedef enum {
    BTN_RELEASED = 0,
    BTN_PRESSED,
    BTN_LONG_PRESSED,
    BTN_CLICKED,
} button_state_t;

static esp_timer_handle_t timer;
static button_state_t btn_state = BTN_RELEASED;
static uint64_t btn_pressed_time_us = 0;

static void check_button(void *arg)
{
    if (btn_state == BTN_PRESSED && btn_pressed_time_us < DEAD_TIME_US)
    {
        // Dead time
        btn_pressed_time_us += TIMER_INTERVAL_US;
        return;
    }

    // read button state
    if (gpio_get_level(CONFIG_EL_BUTTON_GPIO) == CONFIG_EL_BUTTON_LEVEL)
    {
        if (btn_state == BTN_RELEASED)
        {
            // first press
            btn_state = BTN_PRESSED;
            btn_pressed_time_us = 0;
            bus_send_event(EVENT_BUTTON_PRESSED, NULL, 0);
            return;
        }

        btn_pressed_time_us += TIMER_INTERVAL_US;

        if (btn_state == BTN_PRESSED && btn_pressed_time_us >= CONFIG_EL_BUTTON_LONG_PRESS_TIME * 1000)
        {
            // Long press
            btn_state = BTN_LONG_PRESSED;
            bus_send_event(EVENT_BUTTON_LONG_PRESSED, NULL, 0);
            return;
        }
    }
    else if (btn_state != BTN_RELEASED)
    {
        bool clicked = btn_state == BTN_PRESSED;
        // released
        btn_state = BTN_RELEASED;
        bus_send_event(EVENT_BUTTON_RELEASED, NULL, 0);
        if (clicked)
            bus_send_event(EVENT_BUTTON_CLICKED, NULL, 0);
    }
}

static const esp_timer_create_args_t timer_args = {
    .name = "button",
    .arg = NULL,
    .callback = check_button,
    .dispatch_method = ESP_TIMER_TASK
};

#endif

esp_err_t input_init()
{
#if CONFIG_EL_BUTTON_ENABLE
    CHECK(gpio_set_direction(CONFIG_EL_BUTTON_GPIO, GPIO_MODE_INPUT));
#if CONFIG_EL_BUTTON_PULLUPDOWN
#if CONFIG_EL_BUTTON_LEVEL == 0
    CHECK(gpio_set_pull_mode(CONFIG_EL_BUTTON_GPIO, GPIO_PULLUP_ONLY));
#else
    CHECK(gpio_set_pull_mode(CONFIG_EL_BUTTON_GPIO, GPIO_PULLDOWN_ONLY));
#endif /* CONFIG_EL_BUTTON_LEVEL */
#endif /* CONFIG_EL_BUTTON_PULLUPDOWN */
    CHECK(esp_timer_create(&timer_args, &timer));
    CHECK(esp_timer_start_periodic(timer, TIMER_INTERVAL_US));
#endif
    return ESP_OK;
}

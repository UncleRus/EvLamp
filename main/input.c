#include "input.h"
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include "bus.h"

#if CONFIG_EL_BUTTON_ENABLE

#define TIMER_INTERVAL_MS 100 // 100ms = 10z
#define DEAD_TIME_MS 10 // 10ms antijitter

typedef enum {
    BTN_RELEASED = 0,
    BTN_PRESSED,
    BTN_LONG_PRESSED,
    BTN_CLICKED,
} button_state_t;

static button_state_t btn_state = BTN_RELEASED;
static uint64_t btn_pressed_time_ms = 0;

static void input_task(void *arg)
{
    ESP_LOGI(TAG, "Button init, GPIO %d", CONFIG_EL_BUTTON_GPIO);

    gpio_set_direction(CONFIG_EL_BUTTON_GPIO, GPIO_MODE_INPUT);
#if CONFIG_EL_BUTTON_PULLUPDOWN
#if CONFIG_EL_BUTTON_LEVEL == 0
    gpio_set_pull_mode(CONFIG_EL_BUTTON_GPIO, GPIO_PULLUP_ONLY);
#else
    gpio_set_pull_mode(CONFIG_EL_BUTTON_GPIO, GPIO_PULLDOWN_ONLY);
#endif /* CONFIG_EL_BUTTON_LEVEL */
#endif /* CONFIG_EL_BUTTON_PULLUPDOWN */

    while (1)
    {
        if (btn_state == BTN_PRESSED && btn_pressed_time_ms < DEAD_TIME_MS)
        {
            // Dead time
            btn_pressed_time_ms += DEAD_TIME_MS;
            goto end;
        }

        // read button state
        if (gpio_get_level(CONFIG_EL_BUTTON_GPIO) == CONFIG_EL_BUTTON_LEVEL)
        {
            if (btn_state == BTN_RELEASED)
            {
                // first press
                btn_state = BTN_PRESSED;
                btn_pressed_time_ms = 0;
                bus_send_event(EVENT_BUTTON_PRESSED, NULL, 0);
                goto end;
            }

            btn_pressed_time_ms += TIMER_INTERVAL_MS;

            if (btn_state == BTN_PRESSED && btn_pressed_time_ms >= CONFIG_EL_BUTTON_LONG_PRESS_TIME)
            {
                // Long press
                btn_state = BTN_LONG_PRESSED;
                bus_send_event(EVENT_BUTTON_LONG_PRESSED, NULL, 0);
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

end:
        vTaskDelay(pdMS_TO_TICKS(TIMER_INTERVAL_MS));
    }
}

#endif

esp_err_t input_init()
{
#if CONFIG_EL_BUTTON_ENABLE
    if (xTaskCreate(input_task, "input", INPUT_TASK_STACK_SIZE, NULL, INPUT_TASK_PRIORITY, NULL) != pdPASS)
    {
        ESP_LOGE(TAG, "Could not create input task");
        return ESP_FAIL;
    }
#endif
    return ESP_OK;
}

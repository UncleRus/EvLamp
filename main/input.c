#include "input.h"
#include <button.h>
#include "bus.h"

static const event_type_t event_types[] = {
    [BUTTON_PRESSED]      = EVENT_BUTTON_PRESSED,
    [BUTTON_RELEASED]     = EVENT_BUTTON_RELEASED,
    [BUTTON_CLICKED]      = EVENT_BUTTON_CLICKED,
    [BUTTON_PRESSED_LONG] = EVENT_BUTTON_PRESSED_LONG,
};

static button_t buttons[INPUT_BTN_MAX] = { 0 };

void callback(button_t *btn, button_state_t state)
{
    size_t button_id = INPUT_BTN_MAIN;
    for (size_t i = 0; i < INPUT_BTN_MAX; i++)
        if (&buttons[i] == btn)
        {
            button_id = i;
            break;
        }

    bus_send_event(event_types[state], &button_id, sizeof(button_id));
}

esp_err_t input_init()
{
    buttons[INPUT_BTN_MAIN].gpio = CONFIG_EL_BUTTON_MAIN_GPIO;
    buttons[INPUT_BTN_MAIN].internal_pull = CONFIG_EL_BUTTON_PULLUPDOWN;
    buttons[INPUT_BTN_MAIN].pressed_level = CONFIG_EL_BUTTON_LEVEL;
    buttons[INPUT_BTN_MAIN].autorepeat = false;
    buttons[INPUT_BTN_MAIN].callback = callback;
    CHECK_LOGE(button_init(&buttons[INPUT_BTN_MAIN]),
            "Failed init 'Main' button");

    buttons[INPUT_BTN_RESET].gpio = CONFIG_EL_BUTTON_RESET_GPIO;
    buttons[INPUT_BTN_RESET].internal_pull = CONFIG_EL_BUTTON_PULLUPDOWN;
    buttons[INPUT_BTN_RESET].pressed_level = CONFIG_EL_BUTTON_RESET_LEVEL;
    buttons[INPUT_BTN_RESET].autorepeat = false;
    buttons[INPUT_BTN_RESET].callback = callback;
    CHECK_LOGE(button_init(&buttons[INPUT_BTN_RESET]),
            "Failed init 'Reset' button");

    buttons[INPUT_BTN_UP].gpio = CONFIG_EL_BUTTON_UP_GPIO;
    buttons[INPUT_BTN_UP].internal_pull = CONFIG_EL_BUTTON_PULLUPDOWN;
    buttons[INPUT_BTN_UP].pressed_level = CONFIG_EL_BUTTON_LEVEL;
    buttons[INPUT_BTN_UP].autorepeat = true;
    buttons[INPUT_BTN_UP].callback = callback;
    CHECK_LOGE(button_init(&buttons[INPUT_BTN_UP]),
            "Failed init 'Up' button");

    buttons[INPUT_BTN_DOWN].gpio = CONFIG_EL_BUTTON_DOWN_GPIO;
    buttons[INPUT_BTN_DOWN].internal_pull = CONFIG_EL_BUTTON_PULLUPDOWN;
    buttons[INPUT_BTN_DOWN].pressed_level = CONFIG_EL_BUTTON_LEVEL;
    buttons[INPUT_BTN_DOWN].autorepeat = true;
    buttons[INPUT_BTN_DOWN].callback = callback;
    CHECK_LOGE(button_init(&buttons[INPUT_BTN_DOWN]),
            "Failed init 'Down' button");

    return ESP_OK;
}

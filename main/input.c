#include "input.h"
#include <button.h>
#include "bus.h"

static const event_type_t event_types[] = {
    [BUTTON_PRESSED]      = EVENT_BUTTON_PRESSED,
    [BUTTON_RELEASED]     = EVENT_BUTTON_RELEASED,
    [BUTTON_CLICKED]      = EVENT_BUTTON_CLICKED,
    [BUTTON_PRESSED_LONG] = EVENT_BUTTON_PRESSED_LONG,
};

static button_handle_t buttons[INPUT_BTN_MAX] = { 0 };

static void callback(const button_event_t *event, void *ctx)
{
    bus_send_event(event_types[event->type], &ctx, sizeof(ctx));
}

static esp_err_t create_button(size_t id, gpio_num_t gpio, uint8_t pressed_level,
        bool pullup, bool autorepeat)
{
    button_config_t config = BUTTON_DEFAULT_CONFIG();

    config.gpio = gpio;
    config.pressed_level = pressed_level;
    config.enable_internal_pull = pullup;
    config.autorepeat = autorepeat;
    config.callback = callback;
    config.callback_ctx = (void *)id;

    return button_create(&config, &buttons[id]);
}

esp_err_t input_init()
{
    CHECK_LOGE(create_button(INPUT_BTN_MAIN, CONFIG_EL_BUTTON_MAIN_GPIO,
            CONFIG_EL_BUTTON_LEVEL, CONFIG_EL_BUTTON_PULLUPDOWN, false),
            "Failed init 'Main' button");

    CHECK_LOGE(create_button(INPUT_BTN_RESET, CONFIG_EL_BUTTON_RESET_GPIO,
            CONFIG_EL_BUTTON_RESET_LEVEL, CONFIG_EL_BUTTON_PULLUPDOWN, false),
            "Failed init 'Reset' button");

    CHECK_LOGE(create_button(INPUT_BTN_UP, CONFIG_EL_BUTTON_UP_GPIO,
            CONFIG_EL_BUTTON_LEVEL, CONFIG_EL_BUTTON_PULLUPDOWN, true),
            "Failed init 'Up' button");

    CHECK_LOGE(create_button(INPUT_BTN_DOWN, CONFIG_EL_BUTTON_DOWN_GPIO,
            CONFIG_EL_BUTTON_LEVEL, CONFIG_EL_BUTTON_PULLUPDOWN, true),
            "Failed init 'Down' button");

    return ESP_OK;
}

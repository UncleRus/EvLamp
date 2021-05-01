#include "surface.h"
#include <framebuffer.h>
#include <led_strip.h>
#include "settings.h"
#include "effect.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/task.h>
#include <soc/soc.h>

static led_strip_t strip;
static framebuffer_t fb;
static TaskHandle_t task;
static uint8_t global_brightness;
static bool playing = false;

// frame renderer, framebuffer -> LED strip
static esp_err_t render_frame()
{
    while (led_strip_busy(&strip)) {}

    for (size_t y = 0; y < fb.height; y++)
        for (size_t x = 0; x < fb.width; x++)
        {
            // calculate strip index of pixel
            size_t strip_idx = y * fb.width + (y % 2 ? fb.width - x - 1 : x);
            // find pixel offset in state frame buffer
            rgb_t color = fb.data[FB_OFFSET(&fb, x, y)];
            if (vol_settings.brightness != 255)
                color = rgb_scale_video(color, vol_settings.brightness);
            CHECK(led_strip_set_pixel(&strip, strip_idx, rgb_scale_video(color, global_brightness)));
        }

    // flush strip buffer
    return led_strip_flush(&strip);
}

void surface_task(void *arg)
{
    vTaskSuspend(NULL);
    while (1)
    {
        TickType_t last_wake_time = xTaskGetTickCount();
        if (effects[vol_settings.effect].run(&fb) != ESP_OK)
            ESP_LOGW(TAG, "Frame draw error");
        if (render_frame() != ESP_OK)
            ESP_LOGW(TAG, "Frame dropped");
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(1000 / vol_settings.fps));
    }
}

esp_err_t surface_init()
{
    led_strip_install();
    // LED strip
    memset(&strip, 0, sizeof(led_strip_t));

    strip.length = sys_settings.leds.width * sys_settings.leds.height;
    strip.gpio = sys_settings.leds.gpio;
    strip.type = sys_settings.leds.type;
    CHECK(led_strip_init(&strip));

    // TODO : calculate single LED current based on its type or just move it to config
    global_brightness = ((float)sys_settings.leds.current_limit / strip.length) / (SINGLE_LED_CURRENT_MA / 256);

    ESP_LOGI(TAG, "Hardware config: w: %d, h: %d, GPIO = %d, type = %d",
            sys_settings.leds.width, sys_settings.leds.height, strip.gpio, strip.type);

    // Framebuffer
    memset(&fb, 0, sizeof(framebuffer_t));
    CHECK(fb_init(&fb, sys_settings.leds.width, sys_settings.leds.height, render_frame));

    if (xTaskCreatePinnedToCore(surface_task, "render", 8192, NULL, 5, &task, APP_CPU_NUM) != pdPASS)
    {
        ESP_LOGE(TAG, "Could not create surface task");
        return ESP_FAIL;
    }

    // Run
    return surface_play();
}

esp_err_t surface_prepare_effect(size_t effect)
{
    if (effects[effect].prepare)
        return effects[effect].prepare(&fb);

    return ESP_OK;
}

esp_err_t surface_set_effect(size_t num)
{
    CHECK_ARG(num < effects_count);

    surface_stop();

    // prepare
    if (effects[num].prepare)
        CHECK(effects[num].prepare(&fb));

    vol_settings.effect = num;
    if (vol_settings_save() != ESP_OK)
        ESP_LOGW(TAG, "Could not save volatile settings");

    ESP_LOGI(TAG, "Switching to effect '%s' (%d)", effects[num].name, num);

    playing = true;
    vTaskResume(task);

    return ESP_OK;
}

esp_err_t surface_next_effect()
{
    if (vol_settings.effect == effects_count - 1)
        return surface_set_effect(0);
    else
        return surface_set_effect(vol_settings.effect + 1);
}

esp_err_t surface_set_brightness(uint8_t val)
{
    CHECK_ARG(val > 0);

    vol_settings.brightness = val;
    if (vol_settings_save() != ESP_OK)
        ESP_LOGW(TAG, "Could not save volatile settings");

    return ESP_OK;
}

esp_err_t surface_set_fps(uint8_t val)
{
    CHECK_ARG(val > 0 && val <= FPS_MAX);

    vol_settings.fps = val;
    return surface_set_effect(vol_settings.effect);
}

esp_err_t surface_play()
{
    if (playing) return ESP_OK;

    ESP_LOGI(TAG, "Starting animation");

    return surface_set_effect(vol_settings.effect);
}

esp_err_t surface_stop()
{
    if (!playing) return ESP_OK;

    ESP_LOGI(TAG, "Stopping animation");

    vTaskSuspend(task);
    playing = false;
    fb_clear(&fb);
    render_frame();

    return ESP_OK;
}

bool surface_is_playing()
{
    return playing;
}

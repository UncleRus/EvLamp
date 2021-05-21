#include "surface.h"
#include <framebuffer.h>
#include <led_strip.h>
#include <soc/soc.h>
#include "settings.h"
#include "effect.h"

#define FRAME_TICKS (pdMS_TO_TICKS(1000 / vol_settings.fps))
#define BIT_PLAYING (1)

static led_strip_t strip;
static framebuffer_t framebuffer;
static EventGroupHandle_t state;

// frame renderer, framebuffer -> LED strip
static esp_err_t render_frame(framebuffer_t *fb, void *arg)
{
    for (size_t y = 0; y < fb->height; y++)
        for (size_t x = 0; x < fb->width; x++)
        {
            size_t strip_idx = y * fb->width + (y % 2 ? fb->width - x - 1 : x);
            rgb_t color = rgb_scale_video(fb->data[FB_OFFSET(fb, x, y)], vol_settings.brightness);
            CHECK(led_strip_set_pixel(&strip, strip_idx, color));
        }

    return led_strip_flush(&strip);
}

static void surface_task(void *arg)
{
    led_strip_install();
    memset(&strip, 0, sizeof(led_strip_t));

    strip.length = sys_settings.leds.width * sys_settings.leds.height;
    strip.gpio = CONFIG_EL_MATRIX_GPIO;
    strip.type = sys_settings.leds.type;
    strip.channel = RMT_CHANNEL;
    // TODO : calculate single LED current based on its type or just move it to config
    strip.brightness = ((float)sys_settings.leds.current_limit / strip.length) / (SINGLE_LED_CURRENT_MA / 256.0f);

    ESP_ERROR_CHECK(led_strip_init(&strip));

    ESP_LOGI(TAG, "LED strip config: %dx%d, GPIO = %d, type = %d",
            sys_settings.leds.width, sys_settings.leds.height, strip.gpio, strip.type);

    // Framebuffer
    memset(&framebuffer, 0, sizeof(framebuffer_t));
    ESP_ERROR_CHECK(fb_init(&framebuffer, sys_settings.leds.width, sys_settings.leds.height, render_frame));

    ESP_LOGI(TAG, "Surface initialized");

    while (1)
    {
        // wait for playing bit, not clear
        xEventGroupWaitBits(state, BIT_PLAYING, pdFALSE, pdTRUE, portMAX_DELAY);

        TickType_t last_wake_time = xTaskGetTickCount();
        if (effects[vol_settings.effect].run(&framebuffer) != ESP_OK)
            ESP_LOGW(TAG, "Frame draw error");
        if (fb_render(&framebuffer, NULL) != ESP_OK)
            ESP_LOGW(TAG, "Frame dropped");
        vTaskDelayUntil(&last_wake_time, FRAME_TICKS);
    }
}

////////////////////////////////////////////////////////////////////////////////

esp_err_t surface_init()
{
    state = xEventGroupCreate();
    if (!state)
    {
        ESP_LOGE(TAG, "Could not create surface event group");
        return ESP_ERR_NO_MEM;
    }

    if (xTaskCreatePinnedToCore(surface_task, "surface", SURFACE_TASK_STACK_SIZE,
            NULL, uxTaskPriorityGet(NULL) + 1, NULL, APP_CPU_NUM) != pdPASS)
    {
        ESP_LOGE(TAG, "Could not create surface task");
        return ESP_FAIL;
    }

    vTaskDelay(pdMS_TO_TICKS(10));

    if (vol_settings.effect >= effects_count)
        vol_settings.effect = CONFIG_EL_EFFECT_DEFAULT;

    CHECK(surface_set_effect(vol_settings.effect));

    return ESP_OK;
}

esp_err_t surface_prepare_effect(size_t effect)
{
    if (!effects[effect].prepare)
        return ESP_OK;

    CHECK(surface_pause());
    CHECK(effects[effect].prepare(&framebuffer));
    CHECK(surface_play());

    return ESP_OK;
}

bool surface_is_playing()
{
    return xEventGroupGetBits(state) & BIT_PLAYING;
}

esp_err_t surface_pause()
{
    if (!surface_is_playing())
        return ESP_OK;

    xEventGroupClearBits(state, BIT_PLAYING);
    vTaskDelay(FRAME_TICKS + 1);

    return ESP_OK;
}

esp_err_t surface_stop()
{
    CHECK(surface_pause());

    CHECK(fb_begin(&framebuffer));
    fb_clear(&framebuffer);
    CHECK(fb_end(&framebuffer));

    CHECK(fb_render(&framebuffer, NULL));

    ESP_LOGI(TAG, "Animation stopped");

    return ESP_OK;
}

esp_err_t surface_play()
{
    if (surface_is_playing())
        return ESP_OK;

    xEventGroupSetBits(state, BIT_PLAYING);
    ESP_LOGI(TAG, "Animation started");

    return ESP_OK;
}

esp_err_t surface_set_effect(size_t num)
{
    CHECK_ARG(num < effects_count);

    CHECK(surface_stop());

    vol_settings.effect = num;
    if (vol_settings_save() != ESP_OK)
        ESP_LOGW(TAG, "Could not save volatile settings");

    CHECK(surface_prepare_effect(num));
    CHECK(surface_play());

    ESP_LOGI(TAG, "Switched to effect '%s' (%d)", effects[num].name, num);

    return ESP_OK;
}

esp_err_t surface_next_effect()
{
    return surface_set_effect(vol_settings.effect < effects_count - 1
            ? vol_settings.effect + 1
            : 0);
}

esp_err_t surface_set_brightness(uint8_t val)
{
    CHECK(surface_pause());

    vol_settings.brightness = val;
    if (vol_settings_save() != ESP_OK)
        ESP_LOGW(TAG, "Could not save volatile settings");

    CHECK(surface_play());

    return ESP_OK;
}

esp_err_t surface_increment_brightness(int8_t val)
{
    int16_t b = vol_settings.brightness + val;
    if (b < 0) b = 0;
    else if (b > 255) b = 255;
    return surface_set_brightness(b);
}

esp_err_t surface_set_fps(uint8_t val)
{
    CHECK_ARG(val > 0 && val <= FPS_MAX);

    CHECK(surface_pause());

    vol_settings.fps = val;
    if (vol_settings_save() != ESP_OK)
        ESP_LOGW(TAG, "Could not save volatile settings");

    CHECK(surface_play());

    return ESP_OK;
}

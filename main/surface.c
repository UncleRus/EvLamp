#include "surface.h"
#include <framebuffer.h>
#include <fbanimation.h>
#include <led_strip.h>
#include <soc/soc.h>
#include "settings.h"
#include "effect.h"

static led_strip_t strip;
static framebuffer_t framebuffer;
static fb_animation_t animation;
static bool playing = false;

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

static void init_task(void *arg)
{
    led_strip_install();
    // LED strip
    memset(&strip, 0, sizeof(led_strip_t));

    strip.length = sys_settings.leds.width * sys_settings.leds.height;
    strip.gpio = CONFIG_EL_MATRIX_GPIO;
    strip.type = sys_settings.leds.type;
    strip.channel = RMT_CHANNEL;
    // TODO : calculate single LED current based on its type or just move it to config
    strip.brightness = ((float)sys_settings.leds.current_limit / strip.length) / (SINGLE_LED_CURRENT_MA / 256);

    ESP_ERROR_CHECK(led_strip_init(&strip));

    ESP_LOGI(TAG, "LED strip config: %dx%d, GPIO = %d, type = %d",
            sys_settings.leds.width, sys_settings.leds.height, strip.gpio, strip.type);

    // Framebuffer
    memset(&framebuffer, 0, sizeof(framebuffer_t));
    ESP_ERROR_CHECK(fb_init(&framebuffer, sys_settings.leds.width, sys_settings.leds.height, render_frame));

    // Animation
    ESP_ERROR_CHECK(fb_animation_init(&animation, &framebuffer));

    ESP_LOGI(TAG, "Surface initialized");

    ESP_ERROR_CHECK(surface_play());

    vTaskDelete(NULL);
}

esp_err_t surface_init()
{
    if (xTaskCreatePinnedToCore(init_task, "surface_init", 2048, NULL, uxTaskPriorityGet(NULL) + 1, NULL, APP_CPU_NUM) != pdPASS)
    {
        ESP_LOGE(TAG, "Could not create surface init task");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t surface_prepare_effect(size_t effect)
{
    if (effects[effect].prepare)
        return effects[effect].prepare(&framebuffer);

    return ESP_OK;
}

esp_err_t surface_set_effect(size_t num)
{
    CHECK_ARG(num < effects_count);

    vol_settings.effect = num;
    if (vol_settings_save() != ESP_OK)
        ESP_LOGW(TAG, "Could not save volatile settings");

    ESP_LOGI(TAG, "Switching to effect '%s' (%d)", effects[num].name, num);

    // stop previous
    fb_animation_stop(&animation);
    // clear frambuffer
    fb_clear(&framebuffer);
    // prepare
    if (effects[num].prepare)
        CHECK(effects[num].prepare(&framebuffer));
    playing = true;
    // play new
    return fb_animation_play(&animation, vol_settings.fps, effects[num].run, NULL);
}

esp_err_t surface_next_effect()
{
    return surface_set_effect(vol_settings.effect < effects_count - 1
            ? vol_settings.effect + 1
            : 0);
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

    CHECK(surface_set_effect(vol_settings.effect));

    ESP_LOGI(TAG, "Animation started");

    return ESP_OK;
}

esp_err_t surface_stop()
{
    if (!playing) return ESP_OK;

    fb_animation_stop(&animation);

    fb_clear(&framebuffer);
    render_frame(&framebuffer, NULL);

    playing = false;

    ESP_LOGI(TAG, "Animation stopped");

    return ESP_OK;
}

bool surface_is_playing()
{
    return playing;
}

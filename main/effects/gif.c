#include "effects/gif.h"
#include <nsgif.h>

// TODO upload custom gif files in web browser, store in NVS

#define EMBED_GIF(name) \
    extern const uint8_t embed_##name##_start[] asm("_binary_"#name"_gif_start"); \
    extern const uint8_t embed_##name##_end[] asm("_binary_"#name"_gif_end")

#define USE_EMBED_GIF(name) { .start = embed_##name##_start, .end = embed_##name##_end }

EMBED_GIF(heart);
EMBED_GIF(heart2);
EMBED_GIF(boldman);
EMBED_GIF(coin);
EMBED_GIF(disk);
EMBED_GIF(girl);
EMBED_GIF(hairyman);
EMBED_GIF(hero);
EMBED_GIF(torch);
EMBED_GIF(zombie);
EMBED_GIF(cutebear);

typedef struct
{
    const uint8_t *start;
    const uint8_t *end;
} embed_gif_t;

static embed_gif_t gifs[] = {
    USE_EMBED_GIF(heart),
    USE_EMBED_GIF(heart2),
    USE_EMBED_GIF(boldman),
    USE_EMBED_GIF(coin),
    USE_EMBED_GIF(disk),
    USE_EMBED_GIF(girl),
    USE_EMBED_GIF(hairyman),
    USE_EMBED_GIF(hero),
    USE_EMBED_GIF(torch),
    USE_EMBED_GIF(zombie),
    USE_EMBED_GIF(cutebear),
};

#define P_FILE 0

EFFECT_PARAMS(gif, 1) = {
    DECL_PARAM_RANGE(P_FILE, "File", 0, sizeof(gifs) / sizeof(embed_gif_t) - 1, 0),
};

nsgif_t *gif = NULL;
const nsgif_info_t *gif_info;
uint32_t next_frame_time = 0;
size_t pos_x = 0, pos_y = 0;

static nsgif_bitmap_t *bitmap_create(int width, int height)
{
    return calloc(width * height, 4);
}

static void bitmap_destroy(nsgif_bitmap_t *bitmap)
{
    free(bitmap);
}

static uint8_t *bitmap_get_buffer(nsgif_bitmap_t *bitmap)
{
    return (uint8_t *)bitmap;
}

static void bitmap_set_opaque(void *bitmap, bool opaque) {}

static bool bitmap_test_opaque(void *bitmap)
{
    return false;
}

static void bitmap_mark_modified(void *bitmap) {}

static nsgif_bitmap_cb_vt bitmap_callbacks = {
    .create = bitmap_create,
    .destroy = bitmap_destroy,
    .get_buffer = bitmap_get_buffer,
    .set_opaque = bitmap_set_opaque,
    .test_opaque = bitmap_test_opaque,
    .modified = bitmap_mark_modified,
    .get_rowspan = NULL
};

esp_err_t effect_gif_prepare(framebuffer_t *fb)
{
    if (gif)
        nsgif_destroy(gif);

    nsgif_error res = nsgif_create(&bitmap_callbacks, NSGIF_BITMAP_FMT_ABGR8888, &gif);
    if (res != NSGIF_OK)
    {
        ESP_LOGE(TAG, "Error creating GIF handler: %d (%s)", res, nsgif_strerror(res));
        return ESP_FAIL;
    }

    size_t size = gifs[PARAM_VAL(gif, P_FILE)].end - gifs[PARAM_VAL(gif, P_FILE)].start;
    res = nsgif_data_scan(gif, size, (uint8_t *)gifs[PARAM_VAL(gif, P_FILE)].start);

    if (res != NSGIF_OK)
    {
        nsgif_destroy(gif);
        gif = NULL;
        ESP_LOGE(TAG, "Error loading GIF: %d (%s)", res, nsgif_strerror(res));
        return ESP_FAIL;
    }

    gif_info = nsgif_get_info(gif);
    ESP_LOGI(TAG, "GIF frame_count=%d, width=%d, height=%d", gif_info->frame_count, gif_info->width, gif_info->height);

    pos_x = fb->width / 2 - gif_info->width / 2;
    pos_y = fb->height / 2 - gif_info->height / 2;

    next_frame_time = 0;

    return ESP_OK;
}

esp_err_t effect_gif_run(framebuffer_t *fb)
{
    if (!gif)
        return ESP_OK;

    uint32_t time_cs = esp_timer_get_time() / 10000;
    if (time_cs < next_frame_time)
        return ESP_OK;
    
    nsgif_rect_t frame_rect;
    uint32_t delay_cs;
    uint32_t frame;
    nsgif_error res = nsgif_frame_prepare(gif, &frame_rect, &delay_cs, &frame);
    if (res == NSGIF_ERR_ANIMATION_END)
        return ESP_OK;
    if (res != NSGIF_OK)
    {
        ESP_LOGW(TAG, "Error preparing GIF frame: %d (%s)", res, nsgif_strerror(res));
        return ESP_FAIL;
    }
    next_frame_time = delay_cs == NSGIF_INFINITE ? UINT32_MAX : time_cs + delay_cs - 1;

    CHECK(fb_begin(fb));

    nsgif_bitmap_t *bitmap;
    res = nsgif_frame_decode(gif, frame, &bitmap);
    if (res != NSGIF_OK)
    {
        ESP_LOGW(TAG, "Error decoding GIF frame: %d (%s)", res, nsgif_strerror(res));
        return ESP_FAIL;
    }

    uint32_t *frame_image = (uint32_t *)bitmap;

    for (size_t x = frame_rect.x0; x < MIN(fb->width, frame_rect.x1); x++)
        for (size_t y = frame_rect.y0; y < MIN(fb->height, frame_rect.y1); y++)
        {
            size_t offs = y * gif_info->width + x;
            rgb_t c = {
                .r = frame_image[offs],
                .g = frame_image[offs] >> 8,
                .b = frame_image[offs] >> 16,
            };
            fb_set_pixel_rgb(fb, pos_x + x, pos_y + fb->height - y - 1, c);
        }

    return fb_end(fb);
}

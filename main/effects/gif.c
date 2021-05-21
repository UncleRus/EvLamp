#include "effects/gif.h"
#include <libnsgif.h>

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
    DECL_PARAM(P_FILE, "File", 0, sizeof(gifs) / sizeof(embed_gif_t) - 1, 0),
};

static int frame = -1;
gif_animation *gif = NULL;

static void* bitmap_create(int width, int height)
{
    return calloc(width * height, 4);
}

static void bitmap_destroy(void *bitmap)
{
    free(bitmap);
}

static unsigned char* bitmap_get_buffer(void *bitmap)
{
    return bitmap;
}

static void bitmap_set_opaque(void *bitmap, bool opaque) {}

static bool bitmap_test_opaque(void *bitmap)
{
    return false;
}

static void bitmap_mark_modified(void *bitmap) {}

static gif_bitmap_callback_vt bitmap_callbacks = {
    bitmap_create,
    bitmap_destroy,
    bitmap_get_buffer,
    bitmap_set_opaque,
    bitmap_test_opaque,
    bitmap_mark_modified
};

esp_err_t effect_gif_prepare(framebuffer_t *fb)
{
    if (gif)
    {
        gif_finalise(gif);
        free(gif);
    }
    gif = calloc(1, sizeof(gif_animation));
    if (!gif)
        return ESP_ERR_NO_MEM;
    gif_create(gif, &bitmap_callbacks);

    gif_result res;
    do
    {
        size_t size = gifs[EPARAM(gif, P_FILE)].end - gifs[EPARAM(gif, P_FILE)].start;
        res = gif_initialise(gif, size, (uint8_t *)gifs[EPARAM(gif, P_FILE)].start);
    } while (res == GIF_WORKING);

    if (res != GIF_OK)
    {
        gif_finalise(gif);
        free(gif);
        gif = NULL;
        ESP_LOGE(TAG, "Unsupported GIF");
    }

    frame = -1;
    ESP_LOGI(TAG, "GIF frame_count=%d, width=%d, height=%d", gif->frame_count, gif->width, gif->height);

    return ESP_OK;
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

esp_err_t effect_gif_run(framebuffer_t *fb)
{
    if (frame >= 0)
    {
        uint32_t t = (esp_timer_get_time() - fb->last_frame_us) / 10000; // cs
        if (t < gif->frames[frame].frame_delay)
            return ESP_OK;
    }
    CHECK(fb_begin(fb));

    frame++;
    frame %= gif->frame_count;

    gif_result res = gif_decode_frame(gif, frame);
    if (res == GIF_OK)
    {
        size_t width = MIN(fb->width, gif->width);
        size_t height = MIN(fb->height, gif->height);
        uint32_t *frame_image = (uint32_t *)gif->frame_image;
        for (size_t x = 0; x < width; x++)
            for (size_t y = 0; y < height; y++)
            {
                size_t offs = y * gif->width + x;
                rgb_t c = {
                    .r = frame_image[offs] & 0xff,
                    .g = (frame_image[offs] >> 8) & 0xff,
                    .b = (frame_image[offs] >> 16) & 0xff,
                };
                fb_set_pixel_rgb(fb, x, height - y - 1, c);
            }
    }
    else ESP_LOGW(TAG, "gif: failed to decode frame %d", frame);

    return fb_end(fb);
}

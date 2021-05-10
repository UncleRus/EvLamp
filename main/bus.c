#include "bus.h"
#include <string.h>

static QueueHandle_t bus;

esp_err_t bus_init()
{
    bus = xQueueCreate(BUS_QUEUE_LEN, sizeof(event_t));
    if (!bus)
    {
        ESP_LOGE(TAG, "Cannot create bus queue");
        return ESP_ERR_NO_MEM;
    }

    return ESP_OK;
}

esp_err_t bus_send_event(event_type_t type, void *data, size_t size)
{
    if (size > BUS_EVENT_DATA_SIZE)
    {
        ESP_LOGE(TAG, "Event data size too big: %d", size);
        return ESP_ERR_NO_MEM;
    }

    event_t e;
    e.type = type;
    if (data && size)
        memcpy(e.data, data, size);

    if (xQueueSend(bus, &e, pdMS_TO_TICKS(BUS_TIMEOUT_MS)) != pdPASS)
    {
        ESP_LOGE(TAG, "Timeout while sending event with type %d", type);
        return ESP_ERR_TIMEOUT;
    }

    return ESP_OK;
}

esp_err_t bus_receive_event(event_t *e, size_t timeout_ms)
{
    CHECK_ARG(e);

    if (xQueueReceive(bus, e, pdMS_TO_TICKS(timeout_ms)) != pdPASS)
        return ESP_ERR_TIMEOUT;

    return ESP_OK;
}

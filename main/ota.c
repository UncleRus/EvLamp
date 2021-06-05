#include "ota.h"
#include <esp_http_client.h>
#include "embed.h"

static char *http_buf = NULL;
static size_t http_buf_size = 0;
static size_t http_buf_pos = 0;
static int http_status = 0;

static const char *github_url = OTA_REPO_URL;

esp_err_t _http_event_handle(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
        case HTTP_EVENT_ERROR:
            ESP_LOGW(TAG, "Got error while HTTP request");
            return ESP_FAIL;
        case HTTP_EVENT_ON_DATA:
            if (!http_status)
            {
                http_status = esp_http_client_get_status_code(evt->client);
                if (http_status != 200)
                {
                    ESP_LOGE(TAG, "Error, got HTTP status = %d", http_status);
                    return ESP_FAIL;
                }
            }
            if (!http_buf)
            {
                http_buf_size = esp_http_client_get_content_length(evt->client);
                http_buf_pos = 0;
                http_buf = malloc(http_buf_size);
                if (!http_buf)
                {
                    ESP_LOGE(TAG, "Error allocating %d bytes of memory", http_buf_size);
                    return ESP_ERR_NO_MEM;
                }
            }
            if (http_buf_pos + evt->data_len > http_buf_size)
            {
                ESP_LOGE(TAG, "HTTP client buffer overflow");
                return ESP_FAIL;
            }
            memcpy(http_buf + http_buf_pos, evt->data, evt->data_len);
            http_buf_pos += evt->data_len;
            break;
        default:
            break;
    }
    return ESP_OK;
}

static esp_err_t request_github_api(const char *endpoint, cJSON **result)
{
    size_t github_url_len = strlen(github_url);
    size_t endpoint_len = strlen(endpoint);
    char *url = malloc(github_url_len + endpoint_len + 1);
    if (!url)
    {
        ESP_LOGE(TAG, "URL is too long");
        return ESP_ERR_NO_MEM;
    }
    memcpy(url, github_url, github_url_len);
    memcpy(url + github_url_len, endpoint, endpoint_len);
    url[github_url_len + endpoint_len] = 0;

    ESP_LOGI(TAG, "Requesting GitHub API %s", url);

    esp_http_client_config_t config = {
        .url = url,
        .cert_pem = embed_ota_server_cert_pem,
        .event_handler = _http_event_handle,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Accept", "application/json, application/vnd.github.v3+json");
    esp_err_t err = esp_http_client_perform(client);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error requesting GitHub API: %d (%s)", err, esp_err_to_name(err));
        goto cleanup;
    }

    *result = cJSON_Parse(http_buf);
    if (!*result)
    {
        err = ESP_ERR_INVALID_RESPONSE;
        ESP_LOGE(TAG, "Invalid JSON response from GitHub API");
    }

cleanup:
    esp_http_client_cleanup(client);
    if (http_buf)
        free(http_buf);
    http_status = 0;
    free(url);

    return err;
}

//////////////////////////////////////////////////////////////////////////////////////

esp_err_t ota_get_available(cJSON **res)
{
    CHECK_ARG(res);

    cJSON *api_resp;
    CHECK(request_github_api("releases?page=1&per_page=3", &api_resp));

    ESP_LOGI(TAG, "%s", cJSON_Print(api_resp));

    *res = cJSON_CreateArray();
    for (int i = 0; i < cJSON_GetArraySize(api_resp); i++)
    {
        cJSON *release = cJSON_GetArrayItem(api_resp, i);

        cJSON *item = cJSON_CreateObject();
        cJSON_AddItemToArray(*res, item);

        cJSON_AddStringToObject(item, "version", cJSON_GetStringValue(cJSON_GetObjectItem(release, "tag_name")));
        cJSON_AddStringToObject(item, "description", cJSON_GetStringValue(cJSON_GetObjectItem(release, "body")));
        //cJSON_AddStringToObject(item, "firmware",
    }

    cJSON_Delete(api_resp);

    return ESP_OK;
}

esp_err_t ota_run(const char *url)
{
    return ESP_OK;
}

esp_err_t ota_rollback()
{
    return ESP_OK;
}

#include "ota.h"
#include <esp_http_client.h>
#include <esp_ota_ops.h>
#include "embed.h"
#include "surface.h"

typedef esp_err_t (*http_data_cb_t)(void *ctx, esp_http_client_handle_t client, size_t len);

typedef struct {
    const esp_partition_t *partition;
    size_t len;
} ota_info_t;

static const char *github_url = "https://api.github.com/repos/" CONFIG_OTA_GITHUB_REPO "/";

static int version_cmp(const char *a, const char *b)
{
    int result = 0;
    char *pc1 = (char *)a;
    char *pc2 = (char *)b;
    while (result == 0)
    {
        char *tail1;
        char *tail2;
        unsigned long ver1 = strtoul(pc1, &tail1, 10);
        unsigned long ver2 = strtoul(pc2, &tail2, 10);
        if (ver1 < ver2)
            result = -1;
        else if (ver1 > ver2)
            result = +1;
        else
        {
            pc1 = tail1;
            pc2 = tail2;
            if (*pc1 == '\0' && *pc2 == '\0')
                break;
            else if (*pc1 == '\0')
                result = -1;
            else if (*pc2 == '\0')
                result = +1;
            else
            {
                pc1++;
                pc2++;
            }
        }
    }
    return result;
}

static esp_err_t http_request(const char *url, const char *accept, void *ctx, http_data_cb_t http_data_cb)
{
    CHECK_ARG(url && http_data_cb);

    ESP_LOGI(TAG, "Requesting %s...", url);

    esp_err_t err;

    // Init client
    esp_http_client_config_t config = {
        .url = url,
        .cert_pem = embed_ota_server_cert_pem,
        .keep_alive_enable = true,
        .user_data = ctx,
        .max_redirection_count = 3,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client)
    {
        err = ESP_FAIL;
        ESP_LOGE(TAG, "Failed to initialise HTTP client");
        goto cleanup;
    }

    // Set request headers
    if (accept)
        esp_http_client_set_header(client, "Accept", accept);

    // Open connection
    err = esp_http_client_open(client, 0);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %d (%s)", err, esp_err_to_name(err));
        goto cleanup;
    }

    ESP_LOGI(TAG, "Fetching headers...");
    // Fetch headers
    int len = esp_http_client_fetch_headers(client);
    if (len < 0)
    {
        err = ESP_FAIL;
        ESP_LOGE(TAG, "Error fetching HTTP headers: %d", len);
        goto cleanup;
    }

    ESP_LOGI(TAG, "Analyzing...");
    if (esp_http_client_get_status_code(client) == 302)
    {
        esp_http_client_set_redirection(client);
        char location[512] = { 0 };
        esp_http_client_get_url(client, location, sizeof(location) - 1);
        ESP_LOGI(TAG, "Redirected to: %s", location);

        err = http_request(location, accept, ctx, http_data_cb);
        goto cleanup;
    }

    ESP_LOGI(TAG, "Processing...");
    // Read and process response body
    err = http_data_cb(ctx, client, len);
    if (err != ESP_OK)
        ESP_LOGE(TAG, "Error receiving HTTP response body");

cleanup:
    if (client)
    {
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
    }

    return err;
}

static esp_err_t github_api_http_cb(void *ctx, esp_http_client_handle_t client, size_t len)
{
    if (!len)
    {
        ESP_LOGE(TAG, "Empty HTTP response");
        return ESP_ERR_INVALID_RESPONSE;
    }

    char *buffer = malloc(len);
    if (!buffer)
    {
        ESP_LOGE(TAG, "HTTP response body is too big (%d)", len);
        return ESP_ERR_NO_MEM;
    }

    esp_err_t err = ESP_OK;

    int data_read = esp_http_client_read(client, buffer, len);
    if (data_read < 0)
    {
        err = ESP_FAIL;
        ESP_LOGE(TAG, "Error receiving HTTP response body");
        goto exit;
    }

    cJSON **json_ctx = (cJSON **)ctx;
    *json_ctx = cJSON_Parse(buffer);
    if (!*json_ctx)
    {
        err = ESP_ERR_INVALID_RESPONSE;
        ESP_LOGE(TAG, "Invalid JSON response from GitHub API");
    }

exit:
    free(buffer);
    return err;
}

static esp_err_t request_github_api(const char *endpoint, cJSON **result)
{
    esp_err_t err;

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

    err = http_request(url, "application/vnd.github+json", result, github_api_http_cb);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error requesting GitHub: %d (%s)", err, esp_err_to_name(err));
        goto exit;
    }

exit:
    free(url);
    return err;
}

static esp_err_t github_ota_http_cb(void *ctx, esp_http_client_handle_t client, size_t len)
{
    ota_info_t *info = (ota_info_t *)ctx;
    if (!len)
        len = info->len;

    esp_ota_handle_t ota;
    char *buffer = NULL;

    ESP_LOGI(TAG, "Before ota_begin..., len = %d", len);
    esp_err_t res = esp_ota_begin(info->partition, len, &ota);
    if (res != ESP_OK)
    {
        ESP_LOGE(TAG, "Cannot start update: %d (%s)", res, esp_err_to_name(res));
        goto exit;
    }

    ESP_LOGI(TAG, "Before malloc");
    buffer = malloc(OTA_CHUNK_SIZE);
    if (!buffer)
    {
        res = ESP_ERR_NO_MEM;
        ESP_LOGE(TAG, "Cannot allocate chunk buffer");
        goto exit;
    }

    size_t bytes = 0;
    ESP_LOGI(TAG, "Writing OTA to partition '%s': %d of %d (%d%%)...",
            info->partition->label, bytes, len, bytes / len * 100);
    while (bytes < len)
    {
        int data_read = esp_http_client_read(client, buffer, OTA_CHUNK_SIZE);
        if (data_read < 0)
        {
            res = ESP_FAIL;
            ESP_LOGE(TAG, "Error reading HTTP response");
            goto exit;
        }
        if (data_read == 0)
        {
            res = ESP_FAIL;
            ESP_LOGE(TAG, "Error reading HTTP response");
        }
        bytes += data_read;

        ESP_LOGI(TAG, "Writing OTA to partition '%s': %d of %d (%d%%)...",
                info->partition->label, bytes, len, bytes / len * 100);

        //ESP_LOG_BUFFER_HEXDUMP(TAG, buffer, data_read, ESP_LOG_INFO);
        vTaskDelay(1);

        res = esp_ota_write(ota, buffer, data_read);
        if (res != ESP_OK)
        {
            ESP_LOGE(TAG, "Error writing OTA data: %d (%s)", res, esp_err_to_name(res));
            goto exit;
        }
    }

    res = esp_ota_end(ota);
    if (res != ESP_OK)
        ESP_LOGE(TAG, "Error finishing OTA: %d (%s)", res, esp_err_to_name(res));

    ESP_LOGI(TAG, "OTA image has been written");

exit:
    if (res != ESP_OK)
        esp_ota_abort(ota);
    if (buffer)
        free(buffer);
    return res;
}

static esp_err_t ota_update(const char *version, cJSON *available, const esp_partition_t *partition)
{
    CHECK_ARG(version && available && partition);
    CHECK(surface_pause());

    esp_err_t res = ESP_OK;
    //char *current = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(available, "current"), "version"));

    esp_log_level_set("*", ESP_LOG_DEBUG);

    cJSON *updates = cJSON_GetObjectItem(available, "updates");
    for (int i = 0; i < cJSON_GetArraySize(updates); i++)
    {
        cJSON *update = cJSON_GetArrayItem(updates, i);
        if (!version_cmp(cJSON_GetStringValue(cJSON_GetObjectItem(update, "version")), version))
        {
            uint32_t size = cJSON_GetNumberValue(cJSON_GetObjectItem(update, "size"));
            ESP_LOGI(TAG, "Downloading version  %s (%d bytes) to partition '%s' at 0x%08x...",
                    version, size, partition->label, partition->address);
            if (partition->size < size)
            {
                ESP_LOGE(TAG, "Image too big");
                res = ESP_FAIL;
                goto exit;
            }
            // Download
            cJSON *jurl = cJSON_GetObjectItem(update, "url");
            ESP_LOGI(TAG, "Downloading image from %s", cJSON_GetStringValue(jurl));

            ota_info_t info = {
                .partition = partition,
                .len = size,
            };
            esp_err_t err = http_request(cJSON_GetStringValue(jurl), "application/octet-stream", (void *)&info, github_ota_http_cb);
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Error requesting image: %d (%s)", err, esp_err_to_name(err));
                return err;
            }

            goto exit;
        }
    }

    res = ESP_FAIL;
    ESP_LOGE(TAG, "Unknown version %s", version);

exit:
    esp_log_level_set("*", ESP_LOG_INFO);
    CHECK(surface_play());
    return res;
}

//////////////////////////////////////////////////////////////////////////////////////

esp_err_t ota_get_available(cJSON **res)
{
    CHECK_ARG(res);
    CHECK(surface_pause());

    cJSON *api_resp;
    CHECK(request_github_api("releases?page=1&per_page=3", &api_resp));

    *res = cJSON_CreateObject();

    // current app
    const esp_app_desc_t *app = esp_ota_get_app_description();
    cJSON *current = cJSON_AddObjectToObject(*res, "current");
    cJSON_AddStringToObject(current, "version", app->version);
    cJSON_AddStringToObject(current, "date", app->date);

    // available updates
    cJSON *updates = cJSON_AddArrayToObject(*res, "updates");
    for (int i = 0; i < cJSON_GetArraySize(api_resp); i++)
    {
        cJSON *release = cJSON_GetArrayItem(api_resp, i);

        char *version = cJSON_GetStringValue(cJSON_GetObjectItem(release, "tag_name"));

        // find asset
        cJSON *assets = cJSON_GetObjectItem(release, "assets");
        for (int c = 0; c < cJSON_GetArraySize(assets); c++)
        {
            cJSON *asset = cJSON_GetArrayItem(assets, c);
            char *name = cJSON_GetStringValue(cJSON_GetObjectItem(asset, "name"));
            if (strcmp(name, OTA_BINARY_NAME))
                continue;

            cJSON *update = cJSON_CreateObject();
            cJSON_AddStringToObject(update, "version", version);
            cJSON_AddStringToObject(update, "description", cJSON_GetStringValue(cJSON_GetObjectItem(release, "body")));
            cJSON_AddStringToObject(update, "date", cJSON_GetStringValue(cJSON_GetObjectItem(release, "created_at")));
            cJSON_AddStringToObject(update, "url", cJSON_GetStringValue(cJSON_GetObjectItem(asset, "browser_download_url")));
            cJSON_AddNumberToObject(update, "size", cJSON_GetNumberValue(cJSON_GetObjectItem(asset, "size")));
            cJSON_AddItemToArray(updates, update);
        }
    }

    cJSON_Delete(api_resp);

    CHECK(surface_play());

    return ESP_OK;
}

esp_err_t ota_run(const char *version)
{
    CHECK_ARG(version);

    const esp_partition_t *part = esp_ota_get_next_update_partition(NULL);
    if (!part)
    {
        ESP_LOGE(TAG, "No partition for update");
        return ESP_FAIL;
    }

    cJSON *avail = NULL;
    esp_err_t res = ota_get_available(&avail);
    if (res != ESP_OK)
    {
        ESP_LOGE(TAG, "Cannot find available updates");
        goto exit;
    }

    char *cur_version = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(avail, "current"), "version"));
    int cmp = version_cmp(cur_version, version);
//    if (!cmp)
//    {
//        ESP_LOGW(TAG, "Cannot update to same version");
//        res = ESP_FAIL;
//        goto exit;
//    }
#ifndef CONFIG_OTA_DOWNGRADE
    if (cmp > 0)
    {
        ESP_LOGE(TAG, "Cannot downgrade");
        res = ESP_FAIL;
        goto exit;
    }
#endif
    res = ota_update(version, avail, part);
    if (res != ESP_OK)
    {
        ESP_LOGE(TAG, "Update failed: %d (%s)", res, esp_err_to_name(res));
        res = ESP_FAIL;
        goto exit;
    }
    res = esp_ota_set_boot_partition(part);
    if (res != ESP_OK)
    {
        ESP_LOGE(TAG, "Cannot set boot partition: %d (%s)", res, esp_err_to_name(res));
        res = ESP_FAIL;
        goto exit;
    }

exit:
    if (avail)
        cJSON_Delete(avail);
    return res;
}

esp_err_t ota_rollback()
{
    return ESP_OK;
}

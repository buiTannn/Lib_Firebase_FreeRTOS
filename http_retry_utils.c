#include "http_retry_utils.h"
#include "freertos/task.h"

#define TAG "HTTP_RETRY"

static bool should_retry_status(int status_code) {
    return status_code == 408 || status_code == 429 ||
           status_code == 500 || status_code == 502 ||
           status_code == 503 || status_code == 504;
}

esp_err_t http_request_with_retry(esp_http_client_handle_t client, int max_retries, int base_delay_ms) {
    int retry_count = 0;

    while (retry_count <= max_retries) {
        esp_err_t err = esp_http_client_perform(client);

        if (err == ESP_OK) {
            int status = esp_http_client_get_status_code(client);

            if (should_retry_status(status)) {
                ESP_LOGW(TAG, "Status %d, retrying... (%d/%d)", status, retry_count + 1, max_retries);
            } else if (status >= 400) {
                ESP_LOGE(TAG, "Client error %d, won't retry", status);
                return ESP_FAIL;
            } else {
                ESP_LOGI(TAG, "Request succeeded with status %d", status);
                return ESP_OK;
            }
        } else {
            ESP_LOGE(TAG, "HTTP error: %s, retrying... (%d/%d)", esp_err_to_name(err), retry_count + 1, max_retries);
        }

        retry_count++;
        if (retry_count > max_retries) break;

        int delay_ms = base_delay_ms * (1 << retry_count); // Exponential backoff
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }

    ESP_LOGE(TAG, "Exceeded max retries");
    return ESP_FAIL;
}

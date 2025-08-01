#pragma once

#include "esp_http_client.h"
#include "esp_log.h"
#include <stdbool.h>

esp_err_t http_request_with_retry(esp_http_client_handle_t client, int max_retries, int base_delay_ms);

#include "firebase.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include <string.h>
#include "cJSON.h"

#define TAG "FIREBASE"
static char firebase_url[256];
static char firebase_token[128];

extern const char firebase_root_cert_pem_start[] asm("_binary_firebase_root_cert_pem_start");
extern const char firebase_root_cert_pem_end[] asm("_binary_firebase_root_cert_pem_end");

void firebase_init(const char* url, const char* token) {
    strncpy(firebase_url, url, sizeof(firebase_url));
    strncpy(firebase_token, token, sizeof(firebase_token));
}
//PUT (done)
void firebase_put(const char* path, const char* data) {
    char full_url[512];
    snprintf(full_url, sizeof(full_url), "%s/%s.json?auth=%s", firebase_url, path, firebase_token);

    esp_http_client_config_t config = {
        .url = full_url,
        .cert_pem = firebase_root_cert_pem_start,  
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_method(client, HTTP_METHOD_PUT);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, data, strlen(data));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Firebase PUT success: %s", full_url);
    } else {
        ESP_LOGE(TAG, "Firebase PUT failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}
//GET (done)
void firebase_get(const char* path, char* response_buf, int buf_size) {
    char full_url[512];
    snprintf(full_url, sizeof(full_url), "%s/%s.json?auth=%s", firebase_url, path, firebase_token);

    esp_http_client_config_t config = {
        .url = full_url,
        .cert_pem = firebase_root_cert_pem_start,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Accept", "application/json");
    esp_http_client_set_method(client, HTTP_METHOD_GET);

    // Mở connection
    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return;
    }

    // Gửi request và nhận header
    int content_length = esp_http_client_fetch_headers(client);
    int status_code = esp_http_client_get_status_code(client);

    if (status_code == 200) {
        int total_read = 0;
        int read_len = 0;
        
        // Đọc response
        while (total_read < buf_size - 1) {
            read_len = esp_http_client_read(client, response_buf + total_read, buf_size - total_read - 1);
            if (read_len <= 0) {
                break;
            }
            total_read += read_len;
        }

        if (total_read > 0) {
            response_buf[total_read] = '\0';  // null-terminate
        } else {
            ESP_LOGE(TAG, "No data received");
            response_buf[0] = '\0';
        }
    } else {
        ESP_LOGE(TAG, "HTTP Error: %d", status_code);
        response_buf[0] = '\0';
    }

    esp_http_client_close(client);
    esp_http_client_cleanup(client);
}
//PATCH (updating )
void firebase_patch(const char* path, const char* data) {
    char full_url[512];
    snprintf(full_url, sizeof(full_url), "%s/%s.json?auth=%s", firebase_url, path, firebase_token);

    esp_http_client_config_t config = {
        .url = full_url,
        .cert_pem = firebase_root_cert_pem_start,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_method(client, HTTP_METHOD_PATCH);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, data, strlen(data));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Firebase PATCH success: %s", full_url);
    } else {
        ESP_LOGE(TAG, "Firebase PATCH failed: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
}
//updating
void firebase_delete(const char* path) {
    char full_url[512];
    snprintf(full_url, sizeof(full_url), "%s/%s.json?auth=%s", firebase_url, path, firebase_token);

    esp_http_client_config_t config = {
        .url = full_url,
        .cert_pem = firebase_root_cert_pem_start,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_method(client, HTTP_METHOD_DELETE);

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Firebase DELETE success: %s", full_url);
    } else {
        ESP_LOGE(TAG, "Firebase DELETE failed: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
}

bool firebase_test_connection() {
    char full_url[512];
    snprintf(full_url, sizeof(full_url), "%s/.json?auth=%s&print=pretty", firebase_url, firebase_token);

    esp_http_client_config_t config = {
        .url = full_url,
        .cert_pem = firebase_root_cert_pem_start,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
        .timeout_ms = 5000, 
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_method(client, HTTP_METHOD_GET);

    esp_err_t err = esp_http_client_perform(client);
    esp_http_client_cleanup(client);
    
    return (err == ESP_OK);
}
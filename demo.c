
//add demo.c to src , change name to main.c and build 

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "firebase.h"
#include "rtdb.h"
#include <stdint.h>
#include <stdio.h>
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"

#define WIFI_SSID "Redmi 13" 
#define WIFI_PASS "99999999"

#define BASE_URL "https://test-8ccfa-default-rtdb.firebaseio.com" //your base URL
#define TOKEN    "AdzSbSABNPcZRpR8uOqqsT8TtMPAMX1nwHaoqbNH" //Your secret Token 


static EventGroupHandle_t wifi_event_group;
int receivedata = 0 ;
#define WIFI_CONNECTED_BIT BIT0

// check events wifi
static void handler(void* arg, esp_event_base_t base, int32_t id, void* data) {
    if (base == WIFI_EVENT && id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    }

    if (base == WIFI_EVENT && id == WIFI_EVENT_STA_DISCONNECTED) {
        printf("WiFi disconnected. Reconnecting...\n");
        esp_wifi_connect(); 
    }

    if (base == IP_EVENT && id == IP_EVENT_STA_GOT_IP) {
        printf("IP: " IPSTR "\n", IP2STR(&((ip_event_got_ip_t*)data)->ip_info.ip));
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

//connect to WiFi
void wifi_connect() {
    wifi_event_group = xEventGroupCreate();
    esp_netif_init(); esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    esp_wifi_init(&(wifi_init_config_t)WIFI_INIT_CONFIG_DEFAULT());

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &handler, NULL);

    wifi_config_t cfg = { .sta = { .ssid = WIFI_SSID, .password = WIFI_PASS } };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &cfg);
    esp_wifi_start();

    printf("Connecting...\n");
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);
    printf("Connected!\n");
}

//get data from sensor/temperature
void app_main(void) {
    nvs_flash_init();
    wifi_connect();

    firebase_init(BASE_URL, TOKEN);
    while(1){
        vTaskDelay(pdMS_TO_TICKS(1000)); 
        printf("Receiving temperature data...\n");
        firebase_get_int("sensor/temperature", &receivedata);
        printf("Temperature data received: %d\n", receivedata);
    }
}

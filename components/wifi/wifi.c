/*
 * wifi_manager.c
 *
 *  Created on: Jul 3, 2026
 *      Author: Hariharan
 */

#include "wifi.h"

#include "esp_err.h"
#include "esp_wifi_default.h"
#include "esp_wifi_types_generic.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "nvs.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include <stdint.h>
#include <display.h>
#include <string.h>
#include "event_manager.h"
#include "wifi_info.h"

#define TAG "WI-FI"

// WiFi Credentials
#define WIFI_SSID      "HARIHARAN" 
#define WIFI_PASS      "thanya02"

static wifi_info_t wifi_info;

// WiFi Event Handler
static void wifi_event_handler(
    void *arg,
    esp_event_base_t event_base,
    int32_t event_id,
    void *event_data)
{
    // WiFi Started
    if(event_base == WIFI_EVENT &&
       event_id == WIFI_EVENT_STA_START)
    {
        ESP_LOGI(TAG, "WiFi Started");

        esp_wifi_connect();
    }

    // WiFi Connected
    else if(event_base == WIFI_EVENT &&
            event_id == WIFI_EVENT_STA_CONNECTED)
    {
        ESP_LOGI(TAG, "Connected to Router");

        wifi_info.state = WIFI_STATE_CONNECTED;

        app_event_msg_t event;

        sprintf(event.data.display.lines[0], "%s", "wifi");
        sprintf(event.data.display.lines[1], "%s", "connected");
        event.data.display.line_count = 2;

        event.event = APP_EVENT_WIFI_CONNECTED;
        event_manager_post(&event);
    }

    // IP Address Received
    else if(event_base == IP_EVENT &&
            event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event =
            (ip_event_got_ip_t *) event_data;

        ESP_LOGI(TAG, "Got IP Address");

        ESP_LOGI(TAG, "IP: " IPSTR,
                 IP2STR(&event->ip_info.ip));

        strcpy(wifi_info.ip, &event->ip_info.ip) ;       

    }

    // WiFi Disconnected
    else if(event_base == WIFI_EVENT &&
            event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
		
		wifi_event_sta_disconnected_t *event = (wifi_event_sta_disconnected_t *) event_data;
		
        ESP_LOGW(TAG, "Disconnect reason: %d",
         event->reason);

        // Retry Connection
        ESP_LOGI(TAG, "Retrying Connection...");

        esp_wifi_connect();
    }
}


app_status_t wifi_scan()
{

    uint16_t ap_count = 0;

    wifi_ap_record_t ap_info[20];

    memset(ap_info, 0, sizeof(ap_info));

    esp_wifi_scan_start(NULL, true); // wait until scan complete

    esp_wifi_scan_get_ap_num(&ap_count);

    printf("Found %u access points\n\n", ap_count);

    esp_wifi_scan_get_ap_records(&ap_count,
                                 ap_info);

    for (int i = 0; i < ap_count; i++)
    {
        printf("ssid %s | irrd %d \n", ap_info[i].ssid, ap_info[i].rssi);
    }

    return APP_OK;
}

app_status_t wifi_init()
{

    ESP_LOGI(TAG, "ESP 32 WI-FI init");

    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        nvs_flash_erase();
        nvs_flash_init();
    };

    // Initialize TCP/IP Stack
    ESP_ERROR_CHECK(esp_netif_init());

    // Create Default Event Loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Create Default Wi-Fi Station
    esp_netif_create_default_wifi_sta();

    // Wi-Fi driver Configuration
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_wifi_init(&config));

    // Register WiFi Events
    esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &wifi_event_handler,
        NULL,
        NULL);

    // Register IP Events
    esp_event_handler_instance_register(
        IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &wifi_event_handler,
        NULL,
        NULL);
    // wifi_scan();

    return APP_OK;
}

app_status_t wifi_start()
{
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // WiFi Configuration
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            
            .pmf_cfg = {
            	.capable = true,
            	.required = true
        	},
        },
    };

    // Set WiFi Configuration
    ESP_ERROR_CHECK(esp_wifi_set_config(
        WIFI_IF_STA,
        &wifi_config
    ));

    ESP_ERROR_CHECK(esp_wifi_start());

    printf("Esp32 wi-fi started\n");

    return APP_OK;
}

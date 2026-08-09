#include "application.h"

#include "event_manager.h"
#include "display.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "logger.h"
#include "menu.h"
#include "wifi.h"
#include "wifi_info.h"
#include "mqtt_manager.h"
#include "nvs_manager.h"
#include "nvs.h"
#include "string.h"
#include "sensor_manager.h"

#define TAG "APP"

static bool I_WIWFI_CONNECETD = false;

static TaskHandle_t s_app_task_handle = NULL;

app_status_t application_display_handle(const display_frame_t *display_data)
{

    log_write(LOG_INFO, "APP", "DISPLAY HANDLE");

    ESP_LOGI("display data ", "LINE COUNT %d", display_data->line_count);

    display_show_text(display_data);

    return APP_OK;
}

app_status_t application_init()
{

    return APP_OK;
}

app_status_t application_start(void)
{
    if (s_app_task_handle != NULL)
    {
        return APP_ALREADY_INITIALIZED;
    }

    BaseType_t ret;

    ret = xTaskCreate(
        application_task,
        "APP_TASK",
        4096,
        NULL,
        5,
        &s_app_task_handle);

    if (ret != pdPASS)
    {
        return APP_NO_MEMORY;
    }

    return APP_OK;
}

app_status_t application_stop(void)
{
    if (s_app_task_handle == NULL)
    {
        return APP_NOT_INITIALIZED;
    }

    vTaskDelete(s_app_task_handle);

    s_app_task_handle = NULL;

    return APP_OK;
}

app_status_t application_task(void *arg)
{
    app_event_msg_t event;
    esp_err_t ret;
    wifi_config_t set_wifi_config;
    get_wifi_config_t get_wifi_config = {0};

    while (1)
    {
        if (event_manager_get(&event) != APP_OK)
        {
            continue;
        }

        switch (event.event)
        {
        case APP_EVENT_BOOT:
        {
            log_write(LOG_INFO, "APP", "BOOT...");
            break;
        }

        case APP_EVENT_MENU_LOAD:
        {
            log_write(LOG_INFO, "APP", "MENU LOAD");
            application_display_handle(&event.data.display);

            break;
        }

        case APP_EVENT_BUTTON_DOWN:
        {
            log_write(LOG_INFO, "APP", "BUTTON DOWN PRESSED");
            menu_next();
            application_display_handle(&event.data.display);
            break;
        }

        case APP_EVENT_BUTTON_UP:
        {
            log_write(LOG_INFO, "APP", "BUTTON UP PRESSED");
            menu_previous();
            application_display_handle(&event.data.display);
            break;
        }

        case APP_EVENT_BUTTON_OK:
        {
            log_write(LOG_INFO, "APP", "BUTTON OK PRESSED");
            menu_ok();
            // vTaskDelay(pdMS_TO_TICKS(100));

            wifi_start();
            break;
        }

        case APP_EVENT_WIFI_CONNECTED:
        {
            application_display_handle(&event.data.display);
            // I_WIWFI_CONNECETD = true;

            app_status_t ret = mqtt_init();

            if (ret != APP_OK)
            {
                ESP_LOGI(TAG, "MQTT INITIALIZATION FAILED");
                break;
            }

            mqtt_start();

            break;
        }

        case APP_EVENT_WIFI_CONNECTING:
        {
            application_display_handle(&event.data.display);
            break;
        }

        case APP_EVENT_WIFI_DISCONNECTED:
        {
            application_display_handle(&event.data.display);

            vTaskDelay(pdMS_TO_TICKS(3000));

            wifi_retry();
            break;
        }

        case APP_EVENT_UART_COMMAND:
        {

            if (strncmp(event.data.uart_command, "GET", 3) == 0 ||
                strncmp(event.data.uart_command, "SET", 3) == 0)
            {
                ESP_LOGI("uart command", "GET command");
                char *command;
                char *ssid;
                char *password;

                command = strtok(event.data.uart_command, "|");
                ssid = strtok(NULL, "|");
                password = strtok(NULL, "|");

                if (strcmp(command, "GET_WIFI_CONFIG") == 0)
                {

                    ret = nvs_manager_read_string("wifi", "ssid", get_wifi_config.ssid, sizeof(get_wifi_config.ssid));
                    ESP_LOGI("UART NVS SSID :", "%s", get_wifi_config.ssid);

                    if (ret != ESP_OK)
                    {
                        if (ret == ESP_ERR_NVS_NOT_FOUND)
                        {
                            ESP_LOGE("wifi ssid", "null");
                        }
                    }

                    ESP_LOGI("WIFI_GET_CONFIG SSID", "%s", get_wifi_config.ssid);

                    ret = nvs_manager_read_string("wifi", "password", get_wifi_config.password, sizeof(get_wifi_config.password));
                    ESP_LOGI("UART NVS PASSWORD :", "%s", get_wifi_config.password);

                    if (ret != ESP_OK)
                    {
                        if (ret == ESP_ERR_NVS_NOT_FOUND)
                        {
                            ESP_LOGE("wifi ssid", "null");
                        }
                    }

                    ESP_LOGI("WIFI_GET_CONFIG PASSWORD", "%s", get_wifi_config.password);
                }
                else if (strcmp(command, "SET_WIFI_CONFIG") == 0)
                {
                    ESP_LOGI("UART SET CMD", "TEST");

                    if (command == NULL ||
                        ssid == NULL ||
                        password == NULL)
                    {
                        ESP_LOGW("APP", "Invalid SET_WIFI frame");
                        break;
                    }

                    if (strlen(ssid) >= sizeof(set_wifi_config.sta.ssid))
                    {
                        ESP_LOGW("APP", "SSID too long");
                        break;
                    }

                    if (strlen(password) >= sizeof(set_wifi_config.sta.password))
                    {
                        ESP_LOGW("APP", "Password too long");
                        break;
                    }

                    // strcpy((wifi_config.sta.ssid), ssid);
                    // strcpy(wifi_config.sta.password, password);
                    ESP_LOGI("WIFI_GET_CONFIG SSID", "%s", ssid);

                    ret = nvs_manager_write_string("wifi", "ssid", ssid);

                    if (ret != ESP_OK)
                    {
                        ESP_LOGI("UART SET SSID :", "EROOR");
                        break;
                    }

                    ret = nvs_manager_write_string("wifi", "password", password);

                    if (ret != ESP_OK)
                    {
                        ESP_LOGI("UART SET PASSWORD :", "EROOR");
                        break;
                    }

                    ESP_LOGI("UART SET CONFIG", "succesfully saved");
                }
            }
            else if (strcmp(event.data.uart_command, "START_SENSOR") == 0)
            {
                sensor_manager_start();
            }
            else if (strcmp(event.data.uart_command,"STOP_SENSOR") == 0)
            {
                sensor_manager_stop();
            }
            else if (strcmp(event.data.uart_command, "NVS_FLASH") == 0)
            {
                app_event_msg_t event_fact;

                event_fact.event = APP_EVENT_FACTORY_RESET_CONFIRM;

                event_manager_post(&event_fact);
            }

            break;
        }

        case APP_EVENT_MQTT_CONNECTED:
        {
            mqtt_publish("factory/temp", "10");
            break;
        }

        case APP_EVENT_FACTORY_RESET_CONFIRM:
        {
            ret = nvs_manager_erase_namespace("wifi");

            if (ret != APP_OK)
            {
                ESP_LOGI("APP", "NVS EARESE IS FAILED");
            }

            ESP_LOGI("APP", "NVS EARESE IS SUCCESFULLY");
            memset(&get_wifi_config, 0, sizeof(get_wifi_config));
            break;
        }

        default:
        {
            break;
        }
        }
    }
}
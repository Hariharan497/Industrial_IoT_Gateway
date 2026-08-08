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
            if (strcmp(event.data.uart_command, "GET_WIFI_CONFIG") == 0)
            {
                ESP_LOGI("UART_CMD","TEST");
                char message[32];
                esp_err_t ret = nvs_manager_read_string("wifi", "ssid", message, sizeof(message));

                if (ret != ESP_OK)
                {
                    if (ret == ESP_ERR_NVS_NOT_FOUND)
                    {
                        ESP_LOGI(
                            "UART COMMAND", "WIFI NOT FOUNDED");
                    }
                }
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
            break;
        }

        default:
        {
            break;
        }
        }
    }
}
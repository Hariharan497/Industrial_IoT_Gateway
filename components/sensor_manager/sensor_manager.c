#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "sensor_manager.h"
#include "response_manager.h"
#include "app_type.h"

#define TAG "SENSOR_MANGER"

TaskHandle_t sensor_task_handle = NULL;

static void sensor_read(void)
{
    float temperature = 28.4f;
    float humidity = 62.1f;

    ESP_LOGI(
        TAG,
        "Temperature = %.2f C",
        temperature
    );

    ESP_LOGI(
        TAG,
        "Humidity = %.2f %%",
        humidity
    );

    char response[64];

    snprintf(
        response,
        sizeof(response),
        "TEMP=%.2f,HUM=%.2f",
        temperature,
        humidity
    );

    response_manager_send(
        RESPONSE_SENSOR_DATA,
        response
    );
}

static void sensor_task(void *arg)
{
    ESP_LOGI(
        TAG,
        "Sensor task started"
    );

    while (1)
    {
        sensor_read();

        vTaskDelay(
            pdMS_TO_TICKS(1000)
        );
    }
}

app_state_t sensor_manager_init()
{
    if (sensor_task_handle != NULL)
    {
        return APP_ALREADY_INITIALIZED;
    }

    BaseType_t ret = xTaskCreate(
        sensor_task,
        "sensore_task",
        4096,
        NULL,
        5,
        &sensor_task_handle);

    if(ret != pdPASS)
    {
        sensor_task_handle = NULL;

        ESP_LOGE(
            TAG,
            "Sensor task creation failed"
        );

        return APP_ERROR;
    } 
    
    vTaskSuspend(sensor_task_handle);

    return APP_OK;
}

app_state_t sensor_manager_start()
{
     if (sensor_task_handle == NULL)
    {
        return APP_NOT_INITIALIZED;
    }

    vTaskResume(sensor_task_handle);

    ESP_LOGI(
        TAG,
        "Sensor monitoring started"
    );

    return APP_OK;
}

app_state_t sensor_manager_stop()
{
     if (sensor_task_handle == NULL)
    {
        return APP_NOT_INITIALIZED;
    }

    vTaskSuspend(sensor_task_handle);

    ESP_LOGI(
        TAG,
        "Sensor monitoring stop"
    );

    return APP_OK;
}
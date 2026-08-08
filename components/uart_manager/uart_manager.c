#include <stdio.h>
#include <string.h>

#include "driver/uart.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "uart_manager.h"
#include "event_manager.h"

#define TAG "UART_MANAGER"

#define UART_PORT UART_NUM_0
#define UART_BAUD_RATE 115200

#define UART_RX_PIN 3
#define UART_TX_PIN 1

#define UART_BUF_SIZE 256

static void uart_receive_task(void *arg)
{
    uint8_t data[UART_BUF_SIZE];

    ESP_LOGI(TAG, "UART RECEIVE TASK STARTED");

    while (1)
    {
        int len = uart_read_bytes(
            UART_PORT,
            data,
            sizeof(data),
            pdMS_TO_TICKS(1000));

        if (len > 0)
        {
            app_event_msg_t event;
            ESP_LOGI(TAG, "RX BYTES = %d", len);

            // data[len] = '\0';
            data[strcspn((char *)data, "\r\n")] = '\0';

            printf("Received: %d\n", strlen((char *)data));

            printf("\n");

            event.event = APP_EVENT_UART_COMMAND;

            memcpy(event.data.uart_command, data, strlen((char *)data) + 1);

            // event.data.uart_command[len] = '\0';

            if (strcmp((char *)data, "GET_WIFI_CONFIG") == 0)
            {
                ESP_LOGI(TAG, "UART strcmp MATCH");
            }
            else
            {
                ESP_LOGI(TAG, "UART strcmp NO MATCH");
            }

            event_manager_post(&event);
        }
    }
}

app_status_t uart_manager_init(void)
{
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    /* Configure UART parameters */
    ESP_ERROR_CHECK(
        uart_param_config(
            UART_PORT,
            &uart_config));

    /*
     * UART0 uses the ESP32 USB-UART connection.
     *
     * GPIO1 -> TX
     * GPIO3 -> RX
     */
    // ESP_ERROR_CHECK(
    //     uart_set_pin(
    //         UART_PORT,
    //         UART_TX_PIN,
    //         UART_RX_PIN,
    //         UART_PIN_NO_CHANGE,
    //         UART_PIN_NO_CHANGE
    //     )
    // );

    /* Install UART driver */
    ESP_ERROR_CHECK(
        uart_driver_install(
            UART_PORT,
            UART_BUF_SIZE,
            UART_BUF_SIZE,
            0,
            NULL,
            0));

    /* Create UART receive task */
    BaseType_t result = xTaskCreate(
        uart_receive_task,
        "uart_receive_task",
        4096,
        NULL,
        5,
        NULL);

    if (result != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create UART task");
        return APP_ERROR;
    }

    ESP_LOGI(TAG, "UART manager initialized");

    // const char *msg = "Hello UART\r\n";

    // // Send data
    //     uart_write_bytes(UART_PORT,
    //                      msg,
    //                      strlen(msg));

    return APP_OK;
}
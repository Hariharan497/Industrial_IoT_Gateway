/*
 * menu_create.c
 *
 *  Created on: Jul 3, 2026
 *      Author: Hariharan
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/_intsup.h>
#include "driver/gpio.h"
#include "driver/gpio_etm.h"
#include "esp_attr.h"
#include "esp_err.h"
#include "esp_intr_alloc.h"
#include "esp_private/esp_gpio_reserve.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "logger.h"
#include "app_type.h"
#include "app_event.h"
#include "event_manager.h"

#include "hal/gpio_types.h"
// #include "display.h"

#define Btn_Down 25
#define Btn_Up 26
#define Btn_ok 27

volatile int menu_event = 0;

TaskHandle_t button_handle_t;

static void IRAM_ATTR button_isr_handle(void *arg)
{

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(button_handle_t, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static app_status_t btn_press_task(void *pv)
{

    while (1)
    {

        ulTaskNotifyTake(
            pdTRUE,
            portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(50));

        if (gpio_get_level(Btn_Down) == 0)
        {

            log_write(LOG_INFO, "BUTTON", "BUTTON DOWN PRESSED");

            app_event_msg_t event;
            event.event = APP_EVENT_BUTTON_DOWN;
            event_manager_post(&event);
        }

        if (gpio_get_level(Btn_Up) == 0)
        {

            log_write(LOG_INFO, "BUTTON", "BUTTON UP PRESSED");

            app_event_msg_t event;
            event.event = APP_EVENT_BUTTON_UP;
            event_manager_post(&event);
        }

        if (gpio_get_level(Btn_ok) == 0)
        {
            log_write(LOG_INFO, "BUTTON", "BUTTON OK PRESSED");

            app_event_msg_t event;
            event.event = APP_EVENT_BUTTON_OK;
            event_manager_post(&event);
        }
    }
    return APP_OK;
}

app_status_t button_init()
{
    gpio_config_t btn_config =
        {
            .pin_bit_mask = (1ULL << Btn_Down) | (1ULL << Btn_Up) | (1ULL << Btn_ok),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_NEGEDGE};

    ESP_ERROR_CHECK(gpio_config(&btn_config));

    xTaskCreate(
        btn_press_task,
        "btn_press_task",
        4096,
        NULL,
        10,
        &button_handle_t);

    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_IRAM));

    ESP_ERROR_CHECK(gpio_isr_handler_add(Btn_Down, &button_isr_handle, NULL));
    ESP_ERROR_CHECK(gpio_isr_handler_add(Btn_Up, &button_isr_handle, NULL));
    ESP_ERROR_CHECK(gpio_isr_handler_add(Btn_ok, &button_isr_handle, NULL));

    return APP_OK;
}

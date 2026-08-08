#include "event_manager.h"
#include  "app_type.h"

#include "freertos/FreeRTOS.H"
#include "freertos/queue.h"
#include "esp_log.h"

#define EVENT_QUEUE_LENGTH (10U)

#define EVENT_QUEUE_ITEM_SIZE (sizeof(app_event_msg_t))

static QueueHandle_t s_event_queue = NULL;

static app_status_t event_manager_is_init(void)
{
    if(s_event_queue == NULL){
        return APP_NOT_INITIALIZED;
    }

    return APP_OK;
}

/*----------------------------------------------------------
 * Initialize Event Manager
 *---------------------------------------------------------*/

app_status_t event_manager_init(void)
{

    if(s_event_queue != NULL){
        return APP_ALREADY_INITIALIZED;
    }

    s_event_queue = xQueueCreate(EVENT_QUEUE_LENGTH, EVENT_QUEUE_ITEM_SIZE);

    if (s_event_queue == NULL ){

        return APP_QUEUE_CREATE_FAIL;
    }

    ESP_LOGI("EVENT MANAGER", "EVENT INIT");
    
    return APP_OK;
    
}

/*----------------------------------------------------------
 * Initialize Event Manager
 *---------------------------------------------------------*/

 app_status_t event_manager_deinit(void)
 {
    if(event_manager_is_init() != APP_OK)
    {
        return APP_NOT_INITIALIZED;
    } 

    vQueueDelete(s_event_queue);

    s_event_queue = NULL;

    return APP_OK;
}

/*----------------------------------------------------------
 * Post Event
 *---------------------------------------------------------*/

 app_status_t event_manager_post(const app_event_msg_t *event)
 {
     if(event_manager_is_init() != APP_OK)
    {
        return APP_NOT_INITIALIZED;
    } 

    if(event == NULL ){
        return APP_INVALID_PARAMETER;
    }

    if(xQueueSend(s_event_queue, event, pdMS_TO_TICKS(100) ) != pdPASS){
        return APP_QUEUE_FULL;
    }

    ESP_LOGI("EVENT", "EVENT POST");

    return APP_OK;

}

/*----------------------------------------------------------
 * Get Event
 *---------------------------------------------------------*/

app_status_t event_manager_get(app_event_msg_t *event)
{
    if(event_manager_is_init() != APP_OK)
    {
        return APP_NOT_INITIALIZED;
    } 

    if(event == NULL ){
        return APP_INVALID_PARAMETER;
    }

    if(xQueueReceive(s_event_queue, event, portMAX_DELAY ) != pdPASS){
        return APP_QUEUE_EMPTY;
    }

    ESP_LOGI("EVENT MANGER", "EVENT GET");
    return APP_OK;
}



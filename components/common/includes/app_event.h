#ifndef APP_EVENT_H
#define APP_EVENT_H

#include "display_type.h"
#include "menu_type.h"

typedef enum 
{

    APP_EVENT_NONE = 0,

    // BOOT EVENT
    APP_EVENT_BOOT ,
    //

    // WIFI EVENT
    APP_EVENT_WIFI_CONNECTED,
    APP_EVENT_WIFI_CONNECTING,
    APP_EVENT_WIFI_DISCONNECTED,
    APP_EVENT_WIFI_GET_CONFIG,
    APP_EVENT_WIFI_SET_CONFIG,
    //

    // SENSOR EVENT
    APP_EVENT_SENSOR_UPDATE,
    //

    // FACTORY RESET EVENTS
    APP_EVENT_FACTORY_RESET_REQUEST,
    APP_EVENT_FACTORY_RESET_CONFIRM,
    //

    // CONFIGURATION EVENT
    APP_EVENT_CONFIG_CHANGE,
    //

    // MQTT EVENT
    APP_EVENT_MQTT_CONNECTED,
    APP_EVENT_MQTT_CONNECTING,
    APP_EVENT_MQTT_DISCONNECTED,
    APP_EVENT_MQTT_START,
    APP_EVENT_MQTT_STOP,
    APP_EVENT_MQTT_ERROR,
    APP_EVENT_MQTT_GET_CONFIG,
    APP_EVENT_MQTT_SET_CONFIG,
    //

    // DISPLAY EVENT
    APP_EVENT_DISPLAY_ERROR,
    APP_EVENT_DIPLAY_NOT_INITALIZATION,
    APP_EVENT_DISPLAY_UPDATE,
    //

    // LOG EVENT
    APP_EVENT_LOG_ADD,
    //

    //BUTTON 
    APP_EVENT_BUTTON_UP,
    APP_EVENT_BUTTON_DOWN,
    APP_EVENT_BUTTON_OK,
    //

    //MENU
    APP_EVENT_MENU_LOAD,
    //

    APP_EVENT_UART_COMMAND

}app_event_t;

typedef union 
{
    display_frame_t display;

    menu_frame_t menu;

    char uart_command[20];
    
}app_event_data_t;


typedef struct 
{
    app_event_t event;

    app_event_data_t data;

}app_event_msg_t;


#endif
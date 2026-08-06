#ifndef APP_ERROR_H
#define APP_ERROR_H

typedef enum 
{
    APP_ERROR_NONE = 0,

    APP_ERROR_WIFI,

    APP_ERROR_MQTT,
    
    APP_ERROR_SENSOR,
    
    APP_ERROR_NVS,

    APP_ERROR_DISPLAY

}app_error_t;

#endif
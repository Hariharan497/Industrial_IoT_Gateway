#ifndef WIFI_INFO_H
#define WIFI_INFO_H

#include <stdint.h>

typedef enum
{
    WIFI_STATE_NOT_INITIALIZED,
    WIFI_STATE_INITIALIZED,
    WIFI_STATE_STARTED,
    WIFI_STATE_SCANNING,
    WIFI_STATE_CONNECTED,
    WIFI_STATE_DISCONNECTED,
    WIFI_STATE_ERROR

} wifi_state_t;

typedef struct 
{
    //char ssid[33];

    char ip[16];

    //uint8_t mac[6];

    wifi_state_t state;
}wifi_info_t;


#endif
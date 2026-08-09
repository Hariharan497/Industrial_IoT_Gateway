#ifndef RESPONSE_MANAGER_H
#define RESPONSE_MANAGER_H

#include "app_type.h"

typedef enum
{
    RESPONSE_OK,
    RESPONSE_ERROR,
    RESPONSE_WIFI_CONFIG,
    RESPONSE_SENSOR_DATA

} response_type_t;


app_status_t response_manager_init(void);

app_status_t response_manager_send(
    response_type_t type,
    const char *data
);

#endif
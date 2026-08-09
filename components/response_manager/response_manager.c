#include <stdio.h>
#include <string.h>

#include "response_manager.h"
#include "uart_manager.h"

app_status_t response_manager_send(
    response_type_t type,
    const char *data)
{
    if (data == NULL)
    {
        return APP_INVALID_PARAMETER;
    }

    char response[128];

    switch (type)
    {
        case RESPONSE_SENSOR_DATA:

            snprintf(
                response,
                sizeof(response),
                "SENSOR|%s\r\n",
                data
            );

            break;


        case RESPONSE_OK:

            snprintf(
                response,
                sizeof(response),
                "OK|%s\r\n",
                data
            );

            break;


        case RESPONSE_ERROR:

            snprintf(
                response,
                sizeof(response),
                "ERROR|%s\r\n",
                data
            );

            break;


        default:

            return APP_ERROR;
    }

    return uart_manager_send(response);
}
#ifndef APP_TYPE_H
#define APP_TYPE_H

typedef enum 
{
    APP_OK = 0,

    APP_ERROR,

    APP_TIMEOUT,

    APP_INVALID_PARAMETER,

    APP_NOT_INITIALIZED,

    APP_NO_MEMORY,

    APP_ALREADY_INITIALIZED,

    APP_QUEUE_CREATE_FAIL,

    APP_QUEUE_EMPTY,

    APP_QUEUE_FULL
    
}app_status_t ;

#endif
#ifndef APP_STATE_H
#define APP_STATE_H

typedef enum 
{
    APP_STATE_BOOT = 0,

    APP_STATE_INITALIZATION ,

    APP_STATE_READY ,

    APP_STATE_ERROR ,

    APP_STATE_FACTORY_RESET,

    APP_STATE_FACTORY_RESET_PENDING
    
} app_state_t;

#endif
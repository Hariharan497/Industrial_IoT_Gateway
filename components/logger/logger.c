#include "esp_log.h"
#include "logger.h"

app_status_t log_init(){
    return APP_OK;
}

app_status_t log_write(log_level_t level, const char *tag, const char *message){

    switch(level)
    {
        case LOG_INFO:
            ESP_LOGI(tag, "%s" ,message);
            break;
        case LOG_WARNING:
            ESP_LOGW(tag, "%s", message);
            break;
        case LOG_ERROR:
            ESP_LOGE(tag, "%s", message);
            break;
        default:
            return APP_INVALID_PARAMETER;
             
    }
    return APP_OK;
}


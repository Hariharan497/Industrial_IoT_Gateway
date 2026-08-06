
#include "application.h"
#include "display.h"
#include "event_manager.h"
#include "esp_log.h"
#include "i2c.h"
#include "logger.h"
#include "button.h"
#include "menu.h"
//#include "driver/i2c_master.h"
#include "wifi.h"
#include "mqtt_manager.h"


void app_main(void){

    i2c_master_bus_handle_t bus_handle;

    app_status_t status;

    status = i2c_init(&bus_handle);

    if(status != APP_OK){
        log_write(LOG_INFO, "MAIN", "I2C INILIALIZED FAILED");
    }

    button_init();
    
    display_init(bus_handle);

    event_manager_init();

    application_init();

    application_start();

    app_event_msg_t event ;
    event.event = APP_EVENT_BOOT;
    
    event_manager_post(&event);

    menu_init();

    wifi_init();

    mqtt_init();   

}
idf_component_register(SRCS "main.c"
                    REQUIRES 
                    display 
                    event_manager 
                    application 
                    i2c 
                    logger 
                    button 
                    menu 
                    nvs_manager
                    wifi
                    mqtt_manager
                    uart_manager
                    sensor_manager
                    response_manager
                    )

idf_component_register(SRCS "main.c"
                    REQUIRES 
                    display 
                    event_manager 
                    application 
                    i2c 
                    logger 
                    button 
                    menu 
                    wifi
                    mqtt_manager
                    )

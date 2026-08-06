idf_component_register(SRCS "display.c" 
                    INCLUDE_DIRS "include" 
                    REQUIRES
				        driver 
				        esp_type_utils 
				        k0i05__esp_ssd1306
                        common
                        i2c
                        logger
                    )


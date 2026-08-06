idf_component_register(SRCS "button.c" 
                    INCLUDE_DIRS "include" 
                    REQUIRES
				        driver 
                        common
                        logger
                        event_manager
                    )


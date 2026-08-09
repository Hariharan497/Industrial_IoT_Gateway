idf_component_register(SRCS "response_manager.c"
                    INCLUDE_DIRS "include"
                    REQUIRES
				       event_manager
                       common
                       driver
                       uart_manager
                       
                    )
idf_component_register(SRCS "sensor_manager.c"
                    INCLUDE_DIRS "include"
                    REQUIRES
				       event_manager
                       common
                       driver
                       response_manager
                    )
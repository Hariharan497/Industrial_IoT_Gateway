idf_component_register(SRCS "application.c"
                    INCLUDE_DIRS "include"
                    REQUIRES display event_manager common logger menu wifi mqtt_manager
                    )

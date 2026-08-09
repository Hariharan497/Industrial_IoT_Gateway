#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "app_state.h"

app_state_t sensor_manager_init(void);
app_state_t sensor_manager_start(void);
app_state_t sensor_manager_stop(void);

#endif
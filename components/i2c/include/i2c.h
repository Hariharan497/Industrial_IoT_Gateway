#ifndef I2C_H
#define I2C_H

#include "app_event.h"
#include "app_type.h"
#include "driver/i2c_master.h"

app_status_t i2c_init();

app_status_t add_device(i2c_master_bus_handle_t bus_handle, uint8_t DEV_ADDR, uint32_t DEV_CLK_SPD );


#endif
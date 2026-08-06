#include "driver/i2c_master.h"
#include "i2c.h"
#include <stdint.h>
#include "esp_log.h"

#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_SDA_IO 21

#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000

// i2c initializtion   TODO: move to i2c model 
app_status_t i2c_init( i2c_master_bus_handle_t *bus_handle)
{

	i2c_master_bus_config_t i2c_mst_config = {
		.clk_source = I2C_CLK_SRC_DEFAULT,
		.i2c_port = I2C_MASTER_NUM,
		.scl_io_num = I2C_MASTER_SCL_IO,
		.sda_io_num = I2C_MASTER_SDA_IO,
		.glitch_ignore_cnt = 7,
		.flags.enable_internal_pullup = true,
	};

	ESP_LOGI("I2C", "I2C INIT FUNCTION");
	ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, bus_handle));
	ESP_LOGI("I2C", "I2C AFTER MASTER BUS");
	return APP_OK;
}

app_status_t add_device(i2c_master_bus_handle_t bus_handle, uint8_t DEV_ADDR, uint32_t DEV_CLK_SPD ){

	ESP_LOGI("I2C", "I2C ADD DEVICE FUNC");
	i2c_device_config_t dev_cfg = {
		.dev_addr_length = I2C_ADDR_BIT_LEN_7,
		.device_address = DEV_ADDR,
		.scl_speed_hz = DEV_CLK_SPD,
	};

	ESP_LOGI("I2C", "ADD DEV CONFIG");
	i2c_master_dev_handle_t dev_handle;
	ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));
	ESP_LOGI("I2C", "I2C ADD DEVICE ");
	return APP_OK;

}
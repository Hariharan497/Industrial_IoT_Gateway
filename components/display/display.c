#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
// #include "driver/i2c_master.h"
#include "font_latin_8x8.h"
#include "bitmap_icon.h"

#include "portmacro.h"
#include "ssd1306.h"
#include <string.h>

#include "display.h"
#include "i2c.h"
#include "logger.h"

#define TAG "SSD"

#define DISPLAY_MAX_PAGE 8
#define DISPLAY_MAX_CONTRAST 0xFF
#define DISPLAY_MIN_CONTRAST 0x00

static ssd1306_handle_t s_display_handle = NULL;

static ssd1306_config_t dev_cfg = I2C_SSD1306_128x64_CONFIG_DEFAULT;

// display initialized validation
static app_status_t display_check_initialized(void)
{
	if (s_display_handle == NULL)
	{
		ESP_LOGE(TAG, "ssd1306 handle init failed");

		return APP_ERROR;
	}

	return APP_OK;
}

// validate page
static app_status_t display_check_page(uint8_t page)
{
	if (page >= DISPLAY_MAX_PAGE)
	{
		return APP_INVALID_PARAMETER;
	}

	return APP_OK;
}

// validate text or, bitmap
static app_status_t display_check_null_pointer(const void *pointer)
{

	if (pointer == NULL)
	{
		return APP_INVALID_PARAMETER;
	}
	return APP_OK;
}

// display inialization
app_status_t display_init(i2c_master_bus_handle_t master_bus_handle)
{

	if (s_display_handle != NULL)
	{
		return APP_ALREADY_INITIALIZED;
	}

	// add device

	add_device(master_bus_handle, I2C_SSD1306_DEV_ADDR, I2C_SSD1306_DEV_CLK_SPD);

	// init device
	esp_err_t ret = ssd1306_init(master_bus_handle, &dev_cfg, &s_display_handle);
	if (ret != ESP_OK)
	{
		return APP_ERROR;
	}

	ssd1306_set_contrast(s_display_handle, DISPLAY_MAX_CONTRAST);

	ESP_LOGI(TAG, "ssd1306 handle successfully ");


	return APP_OK;
}

// clear the display
app_status_t display_clear()
{

	app_status_t status = display_check_initialized();
	if (status != APP_OK)
	{
		return status;
	}

	ssd1306_clear_display(s_display_handle, false);
	return APP_OK;
}

// display the text
app_status_t display_show_text(const display_frame_t *data)
{
	app_status_t status;

	status = display_check_initialized();
	if (status != APP_OK)
	{
		return status;
	}

	if(data == NULL){
		log_write(LOG_INFO, "DISPLAY", "INVALID PARAMETER");
		return APP_INVALID_PARAMETER;
	}

	// status = display_check_page(page);
	// if (status != APP_OK)
	// {
	// 	return status;
	// }

	status = display_check_null_pointer(data->lines[0]);
	if (status != APP_OK)
	{
		return status;
	}

	display_clear();
	
	for (uint8_t i = 0; i < data->line_count; i++)

	{
		ssd1306_set_contrast(s_display_handle, 0xff);
		esp_err_t ret = ssd1306_display_text(s_display_handle,i,data->lines[i] , false);
		if (ret != ESP_OK)
		{
			return APP_INVALID_PARAMETER;
		}
	}

	return APP_OK;
}

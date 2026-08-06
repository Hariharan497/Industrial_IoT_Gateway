/*
 * display.h
 *
 *  Created on: Jul 3, 2026
 *      Author: Hariharan
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#include "app_type.h"
#include "app_event.h"

app_status_t display_init();

app_status_t display_deinit();

app_status_t display_clear();

app_status_t display_show_text(const display_frame_t *data);

app_status_t display_draw_bitmap(uint8_t  page, const uint8_t *bitmap);

app_status_t display_set_contrast(uint8_t contrast);

#endif /* COMPONENTS_DISPLAY_INCLUDE_DISPLAY_H_ */

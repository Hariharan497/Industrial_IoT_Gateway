#ifndef LOGGER_H
#define LOGGER_H

#include "app_event.h"
#include "app_type.h"
#include "logger_type.h"

app_status_t log_init();

app_status_t log_write(log_level_t level, const char *tag, const char *message);

app_status_t log_deinit();

#endif
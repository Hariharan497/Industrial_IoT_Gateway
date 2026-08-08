#ifndef NVS_MAHAGER_H
#define NVS_MANAGER_H

#include "app_event.h"
#include "app_type.h"

#include <stddef.h>

app_status_t nvs_manager_init(void);

app_status_t nvs_manager_write_string(
    const char *namespace_name,
    const char *key,
    const char *value
);

app_status_t nvs_manager_read_string(
    const char *namespace_name,
    const char *key,
    char *value,
    size_t value_size
);


#endif
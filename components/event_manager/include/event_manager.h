#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "app_type.h"
#include "app_event.h"

app_status_t event_manager_init(void);

app_status_t event_manager_post(const app_event_msg_t *event);

app_status_t event_manager_get(app_event_msg_t *event);

app_status_t event_manager_deinit(void);

#endif
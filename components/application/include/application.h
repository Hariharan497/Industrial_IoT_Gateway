#ifndef APPLICAION_H
#define APPLICATION_H

#include "app_event.h"
#include "app_type.h"

app_status_t application_init(void);

app_status_t application_start(void);

app_status_t application_stop(void);

app_status_t application_task(void *args);

#endif 
#ifndef WIFI_H_
#define WIFI_H_

#include "app_event.h"
#include "app_type.h"

app_status_t wifi_init(void);
app_status_t wifi_scan(void);
app_status_t wifi_start(void);
app_status_t wifi_retry();

#endif
#ifndef MENU_H
#define MENU_H

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#include "app_event.h"
#include "app_type.h"

app_event_t menu_init();

app_status_t menu_next();

app_status_t menu_previous();

app_status_t menu_ok();

#endif
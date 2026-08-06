#include "menu.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "menu_type.h"
#include "event_manager.h"
#include "esp_log.h"

#include <string.h>

const char *menu_items[] = {
    "WI-FI",
    "BLUETOOTH",
    "SETTINGS",
    "ABOUT",
    "CONFIG",
};

static int selected = 0;
static int total_items = 5;
static uint8_t old_selected = 0;

TaskHandle_t menu_task_handle;

static app_status_t menu_load()
{
    static display_frame_t frame;

    app_event_msg_t event;

    for (int i = 0; i < total_items; i++)
    {

        char line[20];
        // Add arrow to selected item
        // first_change = selected;
        if (i == selected)
        {
            sprintf(frame.lines[i], "> %s", menu_items[i]);
        }
        else
        {
            sprintf(frame.lines[i], "  %s", menu_items[i]);
        }
    }

    // ESP_LOGI("MENU", "MIN %d", MIN(old_selected, selected));
    // ESP_LOGI("MENU", "MAX %d", MAX(old_selected, selected));

    frame.line_count = 5;

    event.event = APP_EVENT_MENU_LOAD;
    event.data.display = frame;

    event_manager_post(&event);

    old_selected = selected;
    return APP_OK;
}

app_event_t menu_init()
{

    // xTaskCreate(menu_create, "menu_create",4096, NULL, 5, &menu_task_handle);

    // frame.first_changed_line = MIN(old_selected, total_items);
    // frame.last_changed_line = MAX(old_selected, total_items);

    menu_load();

    return APP_OK;
}

app_status_t menu_next()
{

    selected++;

    if (selected >= total_items)
    {
        selected = total_items - 1;
    }

    // frame.first_changed_line = MIN(old_selected, selected);
    // frame.last_changed_line = MAX(old_selected, selected + 1);
    menu_load();
    return APP_OK;
}

app_status_t menu_previous()
{
    selected--;

    if (selected < 0)
    {
        selected = 0;
    }

    // frame.first_changed_line = MIN(old_selected, selected);
    // frame.last_changed_line = MAX(old_selected + 1, selected);
    menu_load();
    
    return APP_OK;
}

app_status_t menu_ok()
{
    if(selected == 0){

        display_frame_t display_frame ;
        sprintf(display_frame.lines[0], "%s", "wifi");
        sprintf(display_frame.lines[1], "%s", "connecting...");
        //display_frame.lines[0] = ;
        display_frame.line_count = 2;

        app_event_msg_t event;
        event.event = APP_EVENT_WIFI_CONNECTING;
        event.data.display = display_frame;
        event_manager_post(&event);
    }

    return APP_OK;
}

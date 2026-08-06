#ifndef MENU_TYPE_H
#define MENU_TYPE_H

#include <stdint.h>
#define DISPLAY_MAX_LINES    5

typedef struct
{
    const char *lines[DISPLAY_MAX_LINES];
    uint8_t line_count;

}menu_frame_t;


#endif
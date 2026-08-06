#ifndef DISPLAY_TYPE_H
#define DISPLAY_TYPE_H

#include <stdint.h>

typedef struct 
{
    uint8_t line_count;
    //char text[32];
    const char *text[5];

}display_event_data_t;

#define DISPLAY_MAX_LINES    5

typedef struct
{
    
    char lines[DISPLAY_MAX_LINES][20];
    uint8_t line_count;
    uint8_t first_changed_line;
    uint8_t last_changed_line;
    
} display_frame_t;


#endif
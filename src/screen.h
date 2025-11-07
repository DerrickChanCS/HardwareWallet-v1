#pragma once
#include <bsp.h>
#include "oled.h"

#define MENU_BTN_BACK 16 //button 3
#define MENU_BTN_CONFIRM 17 //button 4

typedef struct screen_t {
    void (*on_load)(void *data);
    void (*on_unload)(void);
    bool (*on_evt)(bsp_event_t event);
    struct screen_t *prev_screen;
    void *data;
} screen_t;

void screen_load(screen_t *screen, void *data);
void screen_set_prev(screen_t *screen);
void screen_prev();
void screen_evt(bsp_event_t evt);

extern screen_t pair_screen;
extern screen_t connected_screen;
extern screen_t unlock_screen;
extern screen_t send_screen;

extern screen_t main_menu_screen;

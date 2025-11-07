#pragma once
#include "screen.h"

typedef struct menu_t {
    int index;
    int item_count;
    const char **items;
    void (*on_select)(const char *item);
} menu_t;

void menu_init(menu_t *menu);
bool menu_evt(menu_t *menu, bsp_event_t event);

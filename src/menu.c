#include "menu.h"
#define MENU_BTN_UP 15
#define MENU_BTN_DOWN 17
#define MENU_BTN_OK 16

#define ITEMS_PER_SCREEN 4

void menu_draw(menu_t *menu) {
    oled_clear();
    int page = menu->index / ITEMS_PER_SCREEN;
    int index = menu->index % ITEMS_PER_SCREEN;
    int start = page*ITEMS_PER_SCREEN;
    for (int i=0; i<ITEMS_PER_SCREEN && start+i<menu->item_count; i++) {
        if (i == index) oled_write(0, i, ">");
        oled_write(8, i, menu->items[start+i]);
    }
    oled_redraw();
}

void menu_init(menu_t *menu) {
    menu->index = 0;
    menu_draw(menu);
}

bool menu_evt(menu_t *menu, bsp_event_t event) {
    switch (event) {
        case MENU_BTN_UP:
            if (menu->index > 0) {
                menu->index--;
                menu_draw(menu);
            }
            return true;
        case MENU_BTN_DOWN:
            if (menu->index < menu->item_count-1) {
                menu->index++;
                menu_draw(menu);
            }
            return true;
        case MENU_BTN_OK:
            menu->on_select(menu->items[menu->index]);
            return true;
        default:
            return false;
    }
}

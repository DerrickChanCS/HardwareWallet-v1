#include "oled.h"
#include "screen.h"

static screen_t *current_screen = NULL;

static void _screen_load(screen_t *screen) {
    if (current_screen && current_screen->on_unload) {
        current_screen->on_unload();
    }

    current_screen = screen;
    if (current_screen) {
        current_screen->on_load(screen->data);
    }
}

void screen_load(screen_t *screen, void *data) {
    if (screen) {
        screen->prev_screen = current_screen;
        screen->data = data;
    }
    _screen_load(screen);
}

void screen_set_prev(screen_t *screen) {
    if (current_screen) {
        current_screen->prev_screen = screen;
    }
}

void screen_prev() {
    if (current_screen->prev_screen) {
        _screen_load(current_screen->prev_screen);
    }
}

void screen_evt(bsp_event_t event) {
    bool caught = false;
    if (current_screen->on_evt) {
        caught = current_screen->on_evt(event);
    }
    if (!caught) {
        switch (event) {
            case MENU_BTN_BACK:
                screen_prev();
                break;
            default:
                break;
        }
    }
}

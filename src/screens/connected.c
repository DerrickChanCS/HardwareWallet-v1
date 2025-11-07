#include "screen.h"

void connected_screen_load(void *data) {
    oled_clear();
    oled_write(0, 0, "Connected.");
    oled_write(0, 2, "Send a transaction from your phone");
    oled_redraw();
}

screen_t connected_screen = {
    .on_load = connected_screen_load,
    .on_unload = NULL,
    .on_evt = NULL
};



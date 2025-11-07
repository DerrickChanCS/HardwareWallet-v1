#include "screen.h"

void pair_screen_load(void *data) {
    oled_clear();
    oled_write(0, 0, "Ready to pair...");
    oled_write(0, 2, "Tap your phone to the device");
    oled_redraw();
}

screen_t pair_screen = {
    .on_load = pair_screen_load,
    .on_unload = NULL,
    .on_evt = NULL
};


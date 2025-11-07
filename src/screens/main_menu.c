#include <nrf_log.h>
#include "app.h"
#include "menu.h"

static bool screen_inverted = false;

static const char *main_menu_items[] = {
    "Invert",
    "Disconnect",
    "Clear bonds",
    "Advertising start",
    "Advertising stop",
    "Reset",
    "Power off"
};

static void main_menu_select(const char *item) {
    NRF_LOG_RAW_INFO("Selected: ");
    NRF_LOG_RAW_INFO(item);
    NRF_LOG_RAW_INFO("\n");

    if (strcmp(item, "Invert") == 0) {
        screen_inverted = !screen_inverted;
        oled_invert(screen_inverted);
    }
    else if (strcmp(item, "Disconnect") == 0) {
        app_bsp_event(BSP_EVENT_DISCONNECT);
    }
    else if (strcmp(item, "Clear bonds") == 0) {
        app_bsp_event(BSP_EVENT_CLEAR_BONDING_DATA);
    }
    else if (strcmp(item, "Advertising start") == 0) {
        app_bsp_event(BSP_EVENT_ADVERTISING_START);
    }
    else if (strcmp(item, "Advertising stop") == 0) {
        app_bsp_event(BSP_EVENT_ADVERTISING_STOP);
    }
    else if (strcmp(item, "Reset") == 0) {
        app_bsp_event(BSP_EVENT_RESET);
    }
    else if (strcmp(item, "Power off") == 0) {
        app_bsp_event(BSP_EVENT_SYSOFF);
    } else {
        NRF_LOG_WARNING("Unhandled item: %s", item);
    }
}

menu_t main_menu = {
    .items = main_menu_items,
    .item_count = ARRAY_SIZE(main_menu_items),
    .on_select = main_menu_select
};



static void main_menu_init() {
    menu_init(&main_menu);
}
static bool main_menu_evt(bsp_event_t event) {
    return menu_evt(&main_menu, event);
}

screen_t main_menu_screen = {
    .on_load = main_menu_init,
    .on_unload = NULL,
    .on_evt = main_menu_evt
};

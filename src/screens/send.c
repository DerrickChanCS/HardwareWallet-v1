#include <nrf_log.h>
#include <ble_gatts.h>
#include "app_ble.h"
#include "eth.h"
#include "screen.h"

int page;
eth_transaction_t *transaction;

void draw_page() {
    oled_clear();
    char line[43];

    switch (page) {
        case 0:
            line[0] = '0';
            line[1] = 'x';
            a2hexstr(&line[2], transaction->from, 20);
            oled_write(0, 0, "From:");
            oled_write(0, 1, line);
            break;

        case 1:
            line[0] = '0';
            line[1] = 'x';
            a2hexstr(&line[2], transaction->to, 20);
            oled_write(0, 0, "To:");
            oled_write(0, 1, line);
            break;

        case 2:
            itoa(transaction->value, line, 10);
            strcat(line, " ETH");
            oled_write(0,  0, "Value:");
            oled_write(48, 0, line);

            itoa(transaction->gasPrice, line, 10);
            strcat(line, " Gwei");
            oled_write(0,  1, "Gas Price: ");
            oled_write(72, 1, line);

            itoa(transaction->gas, line, 10);
            oled_write(0,  2, "Gas:");
            oled_write(32, 2, line);
            break;

        case 3:
            oled_write(0,   0, "Confirm?");
            oled_write(0,   3, "Yes");
            oled_write(100, 3, "No");
            break;

        case 4: {
            NRF_LOG_INFO("Case 4");
            eth_hash_t *hash = eth_signTransaction(*transaction);
            eth_sendTransaction_char_notification(&eth_service, hash);
            oled_write(0, 1, "Sent!");
        } break;
        default:
            screen_load(&connected_screen, NULL);
            return;
    }
    oled_redraw();
}

void send_screen_load(void *data) {
    transaction = data;
    page = 0;
    draw_page();
}

bool send_screen_evt(bsp_event_t event) {
    switch (event) {
        case MENU_BTN_CONFIRM:
            page++;
            draw_page();
            return true;
        default:
            return false;
    }
}

screen_t send_screen = {
    .on_load = send_screen_load,
    .on_unload = NULL,
    .on_evt = send_screen_evt
};

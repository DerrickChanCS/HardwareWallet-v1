#pragma once
#include <ble.h>
#include "ble_config.h"
#include "eth.h"

#define BLE_UUID_DEVICE_SERVICE 0x1000
#define BLE_UUID_DEVICE_CHAR_UNLOCK_METHODS 0x1001
#define BLE_UUID_DEVICE_CHAR_UNLOCK 0x1002

typedef struct
{
    uint16_t                 service_handle;     /**< Handle of our Service (as provided by the BLE stack). */
    uint16_t                 conn_handle;        /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).*/
    ble_gatts_char_handles_t unlockMethods_char_handles;
    ble_gatts_char_handles_t unlock_char_handles;
} device_service_t;

extern uint8_t is_unlocked;

void device_service_on_ble_evt(device_service_t *service, const ble_evt_t *p_ble_evt);
void device_service_init(device_service_t *service);
void device_unlock_char_notification(device_service_t *service, uint8_t unlocked);


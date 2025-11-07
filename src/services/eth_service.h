#pragma once
#include <ble.h>
#include "ble_config.h"
#include "eth.h"

#define BLE_UUID_ETH_SERVICE 0x2000
#define BLE_UUID_ETH_CHAR_LIST_ACCOUNTS 0x2001
#define BLE_UUID_ETH_CHAR_NEW_ACCOUNT 0x2002
#define BLE_UUID_ETH_CHAR_SEND_TRANSACTION 0x2003

typedef struct
{
    uint16_t                 service_handle;     /**< Handle of our Service (as provided by the BLE stack). */
    uint16_t                 conn_handle;        /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).*/
    ble_gatts_char_handles_t listAccounts_char_handles;
    ble_gatts_char_handles_t newAccount_char_handles;
    ble_gatts_char_handles_t sendTransaction_char_handles;
} eth_service_t;

void eth_service_on_ble_evt(eth_service_t *service, const ble_evt_t *p_ble_evt);
void eth_service_init(eth_service_t *service);
void eth_listAccounts_char_notification(eth_service_t *service, int32_t *value);
void eth_sendTransaction_char_notification(eth_service_t *service, eth_hash_t *value);

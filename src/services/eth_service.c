#include <ble_srv_common.h>
#include <app_error.h>
#include <mpack.h>
#include "util.h"
#include "screen.h"
#include "eth.h"
#include "eth_service.h"

#define NRF_LOG_MODULE_NAME transaction
#include <nrf_log.h>
NRF_LOG_MODULE_REGISTER();

size_t pack_accounts(char *dest, size_t size, eth_address_t *addresses, size_t len) {
    mpack_writer_t writer;
    mpack_writer_init(&writer, dest, size);
    mpack_start_array(&writer, len);
    for (int i=0; i<len; i++) {
        mpack_write_bin(&writer, (char*)addresses[i], ETH_ADDRESS_SIZE);
    }
    mpack_finish_array(&writer);
    size_t bytes = mpack_writer_buffer_used(&writer);
    mpack_writer_destroy(&writer);
    return bytes;
}

void unpack_transaction(eth_transaction_t *dest, const char *src, size_t size) {
    mpack_node_data_t pool[256];
    mpack_tree_t tree;
    mpack_tree_init_pool(&tree, src, size, pool, ARRAY_SIZE(pool));
    mpack_node_t root = mpack_tree_root(&tree);

    char from[64];
    mpack_node_copy_cstr(mpack_node_map_cstr(root, "from"), from, 64);

    char to[64];
    mpack_node_copy_cstr(mpack_node_map_cstr(root, "to"), to, 64);

    //dest->nonce = mpack_node_int(mpack_node_map_cstr(root, "nonce"));

    dest->gas = mpack_node_int(mpack_node_map_cstr(root, "gas"));

    dest->gasPrice = mpack_node_int(mpack_node_map_cstr(root, "gasPrice"));

    dest->value = mpack_node_int(mpack_node_map_cstr(root, "value"));

    /*mpack_node_t n = mpack_node_map_cstr_optional(root, "data");
    if (n.data->type != mpack_type_nil) {
        mpack_node_copy_cstr(n, (char*)dest->data, 64);
    }*/

    mpack_tree_destroy(&tree);

    hexstr2a(dest->from, from, strlen(from));
    hexstr2a(dest->to, to, strlen(to));
}

void eth_service_on_ble_evt(eth_service_t *service, const ble_evt_t *p_ble_evt) {
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            service->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            service->conn_handle = BLE_CONN_HANDLE_INVALID;
            break;
        case BLE_GATTS_EVT_WRITE: {
            NRF_LOG_INFO("Wrote to handle 0x%04x", (int)p_ble_evt->evt.gatts_evt.params.write.handle);
            if (p_ble_evt->evt.gatts_evt.params.write.handle == service->sendTransaction_char_handles.value_handle) {
                static eth_transaction_t transaction;
                unpack_transaction(&transaction, (char*)p_ble_evt->evt.gatts_evt.params.write.data, p_ble_evt->evt.gatts_evt.params.write.len);
                screen_load(&send_screen, (void*)&transaction);
            }
        } break;
        default:
            // No implementation needed.
            break;
    }
}

void eth_listAccounts_char_init(eth_service_t *service) {
    uint32_t err_code;

    static uint8_t data[64];
    NRF_LOG_INFO("Loading accounts...");
    eth_address_t addresses[10];
    int count = eth_listAddresses(addresses);
    size_t len = pack_accounts((char*)data, sizeof(data), addresses, count*ETH_ADDRESS_SIZE);
    static char str[128];
    a2hexstr(str, (uint8_t*)data, len);

    ble_uuid_t char_uuid;
    char_uuid.uuid = BLE_UUID_ETH_CHAR_LIST_ACCOUNTS;

    ble_uuid128_t base_uuid = {BLE_UUID_VENDOR_BASE};
    err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(err_code);

    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read  = 1;
    char_md.char_props.write = 0;

    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc              = BLE_GATTS_VLOC_STACK;
    char_md.p_cccd_md         = &cccd_md;
    char_md.char_props.notify = 1;

    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    //BLE_GAP_CONN_SEC_MODE_SET_ENC_WITH_MITM(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.vlen = 1;

    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid    = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.max_len     = 200;
    attr_char_value.init_len    = len;
    attr_char_value.p_value     = data;

    err_code = sd_ble_gatts_characteristic_add(service->service_handle,
                                   &char_md,
                                   &attr_char_value,
                                   &service->listAccounts_char_handles);
    NRF_LOG_INFO("Characteristic handle: 0x%04x", service->listAccounts_char_handles.value_handle);
    APP_ERROR_CHECK(err_code);
}

void eth_sendTransaction_char_init(eth_service_t *service) {
    uint32_t err_code;
    uint8_t value[128] = {0xDE,0xAD,0xBE,0xEF};

    ble_uuid_t char_uuid;
    char_uuid.uuid = BLE_UUID_ETH_CHAR_SEND_TRANSACTION;

    ble_uuid128_t base_uuid = {BLE_UUID_VENDOR_BASE};
    err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(err_code);

    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read  = 0;
    char_md.char_props.write = 1;

    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc              = BLE_GATTS_VLOC_STACK;
    char_md.p_cccd_md         = &cccd_md;
    char_md.char_props.notify = 1;

    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    //BLE_GAP_CONN_SEC_MODE_SET_ENC_WITH_MITM(&attr_md.write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.vlen = 1;

    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid    = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.max_len     = 128;
    attr_char_value.init_len    = 4;
    attr_char_value.p_value     = value;

    err_code = sd_ble_gatts_characteristic_add(service->service_handle,
                                   &char_md,
                                   &attr_char_value,
                                   &service->sendTransaction_char_handles);
    NRF_LOG_INFO("Characteristic handle: 0x%04x", service->sendTransaction_char_handles.value_handle);
    APP_ERROR_CHECK(err_code);
}

void eth_service_init(eth_service_t *service) {
    uint32_t err_code;
    ble_uuid_t service_uuid;
    service_uuid.uuid = BLE_UUID_ETH_SERVICE;

    ble_uuid128_t base_uuid = {BLE_UUID_VENDOR_BASE};
    err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    APP_ERROR_CHECK(err_code);

    service->conn_handle = BLE_CONN_HANDLE_INVALID;

    eth_init();
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                    &service_uuid,
                                    &service->service_handle);
    APP_ERROR_CHECK(err_code);

    eth_listAccounts_char_init(service);
    eth_sendTransaction_char_init(service);
}

void eth_listAccounts_char_notification(eth_service_t *service, int32_t *value) {
    if (service->conn_handle != BLE_CONN_HANDLE_INVALID) {
        uint16_t               len = 32;
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = service->listAccounts_char_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len  = &len;
        hvx_params.p_data = (uint8_t*)value;

        sd_ble_gatts_hvx(service->conn_handle, &hvx_params);
    }
}

void eth_sendTransaction_char_notification(eth_service_t *service, eth_hash_t *value) {
    if (service->conn_handle != BLE_CONN_HANDLE_INVALID) {
        uint16_t               len = 32;
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = service->sendTransaction_char_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len  = &len;
        hvx_params.p_data = (uint8_t*)value;

		NRF_LOG_INFO("Send transaction notification");
        sd_ble_gatts_hvx(service->conn_handle, &hvx_params);
    }
}

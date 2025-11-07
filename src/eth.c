#include <app_error.h>
#include <nrf_soc.h>
#include "util.h"
#include "flash.h"
#include "rand.h"
#include "eth.h"

#define NRF_LOG_MODULE_NAME eth
#include <nrf_log.h>
NRF_LOG_MODULE_REGISTER();

//Must be multiple of write size (4)
#define FLASH_ETHADDR_COUNT (FLASH_ETH_START+0)
#define FLASH_ETHADDR_LIST  (FLASH_ETH_START+4)

static void fstorage_evt_handler(nrf_fstorage_evt_t * p_evt);

NRF_FSTORAGE_DEF(nrf_fstorage_t fstorage) =
{
    /* Set a handler for fstorage events. */
    .evt_handler = fstorage_evt_handler,

    /* These below are the boundaries of the flash space assigned to this instance of fstorage.
     * You must set these manually, even at runtime, before nrf_fstorage_init() is called. */
    .start_addr = FLASH_ETH_START,
    .end_addr   = FLASH_ETH_END,
};


static void fstorage_evt_handler(nrf_fstorage_evt_t * p_evt)
{
    if (p_evt->result != NRF_SUCCESS)
    {
        NRF_LOG_INFO("--> Event received: ERROR '%d' while executing an fstorage operation", p_evt->result);
        NRF_LOG_INFO("--> %s", nrf_strerror_get(p_evt->result));
        return;
    }

    switch (p_evt->id)
    {
        case NRF_FSTORAGE_EVT_READ_RESULT:
        {
            NRF_LOG_INFO("--> Event received: read %d bytes at address 0x%x.",
                         p_evt->len, p_evt->addr);
        } break;

        case NRF_FSTORAGE_EVT_WRITE_RESULT:
        {
            NRF_LOG_INFO("--> Event received: wrote %d bytes at address 0x%x.",
                         p_evt->len, p_evt->addr);
        } break;

        case NRF_FSTORAGE_EVT_ERASE_RESULT:
        {
            NRF_LOG_INFO("--> Event received: erased %d page from address 0x%x.",
                         p_evt->len, p_evt->addr);
        } break;

        default:
            break;
    }
}


static void eth_addresses_count(uint32_t *dest) {
    flash_wait_ready(&fstorage);
    ret_code_t rc = nrf_fstorage_read(&fstorage, FLASH_ETHADDR_COUNT, dest, 4);
    APP_ERROR_CHECK(rc);
}

static void eth_addresses_write(eth_address_t *addresses, uint32_t count) {
    eth_clearAddresses();

    uint8_t buffer[1024];
    memset(buffer, 0xFF, 1024);
    for (int i=0; i<count; i++) {
        /*NRF_LOG_INFO("addr %d: ");
        NRF_LOG_HEXDUMP_INFO(addresses[i], ETH_ADDRESS_SIZE);*/
        memcpy(&buffer[i*ETH_ADDRESS_SIZE], addresses[i], ETH_ADDRESS_SIZE);
    }

    ret_code_t rc;
    flash_wait_ready(&fstorage);
    rc = nrf_fstorage_write(&fstorage, FLASH_ETHADDR_COUNT, &count, 4, NULL);
    APP_ERROR_CHECK(rc);

    flash_wait_ready(&fstorage);
    rc = nrf_fstorage_write(&fstorage, FLASH_ETHADDR_LIST, buffer, count*ETH_ADDRESS_SIZE, NULL);
    APP_ERROR_CHECK(rc);
    NRF_LOG_INFO("%d Accounts written", count);
}

void eth_init() {
    flash_init(&fstorage);
    flash_print_info(&fstorage);

    /*eth_address_t addresses[] = {
        {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA},
        {0xC0, 0xDE, 0xCA, 0xFE, 0xC0, 0xDE, 0xCA, 0xFE, 0xC0, 0xDE, 0xCA, 0xFE, 0xC0, 0xDE, 0xCA, 0xFE, 0xC0, 0xDE, 0xCA, 0xFE}
    };
    eth_addresses_write(addresses, 2);*/

    //eth_newAddress();

    /*eth_address_t addresses[10];
    eth_listAddresses(addresses);*/
}

void eth_clearAddresses() {
    flash_wait_ready(&fstorage);
    ret_code_t rc = nrf_fstorage_erase(&fstorage, FLASH_ETH_START, FLASH_ETH_PAGES, NULL);
    APP_ERROR_CHECK(rc);
    NRF_LOG_INFO("Accounts erased");
}

uint32_t eth_listAddresses(eth_address_t *addresses) {
    uint32_t count;
    eth_addresses_count(&count);
    NRF_LOG_INFO("num accounts: %d", count);

    if (count>0) {
        static uint8_t data[512];
        flash_wait_ready(&fstorage);
        ret_code_t rc = nrf_fstorage_read(&fstorage, FLASH_ETHADDR_LIST, data, count*ETH_ADDRESS_SIZE);
        APP_ERROR_CHECK(rc);

        /*NRF_LOG_INFO("data: ");
        NRF_LOG_HEXDUMP_INFO(data, count*ETH_ADDRESS_SIZE);*/

        for (int i=0; i<count; i++) {
            memcpy(&addresses[i], &data[i*ETH_ADDRESS_SIZE], ETH_ADDRESS_SIZE);
            /*NRF_LOG_INFO("addr %d: ");
            NRF_LOG_HEXDUMP_INFO(addresses[i], ETH_ADDRESS_SIZE);*/
        }
    }

    return count;
}

void eth_newAddress() {
    eth_address_t addresses[10];
    uint32_t count = eth_listAddresses(addresses);
    NRF_LOG_INFO("num accounts: %d", count);
    if (count >= UINT32_MAX) {
        NRF_LOG_ERROR("Account overflow");
        return;
    }

    //TODO Yuya: Generate real key value pairs
    static eth_address_t new_address = {0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF};
    memcpy(&addresses[count], new_address, ETH_ADDRESS_SIZE);

    count++;
    eth_addresses_write(addresses, count);
}

eth_hash_t* eth_signTransaction(eth_transaction_t transaction) {
    //TODO Yuya: Add real computation
    /*static eth_hash_t value = {
        0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
        0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F
    };*/
	static eth_hash_t hash;
	rand_generate(hash, ETH_HASH_SIZE);
    return &hash;
}

void eth_dumpTransaction(eth_transaction_t *transaction) {
    NRF_LOG_INFO("From: %s", (uint32_t)transaction->from);
    NRF_LOG_INFO("To: %s", (uint32_t)transaction->to);
    NRF_LOG_INFO("Nonce: %d", transaction->nonce);
    NRF_LOG_INFO("Gas: %d", transaction->gas);
    NRF_LOG_INFO("Gas Price: %d", transaction->gasPrice);
    NRF_LOG_INFO("Value: %d", transaction->value);
    NRF_LOG_INFO("Data: %s", (uint32_t)transaction->data);
}

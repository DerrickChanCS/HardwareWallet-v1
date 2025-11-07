#pragma once
#include <stdint.h>
#define ETH_ADDRESS_SIZE 20
#define ETH_HASH_SIZE 32

typedef uint8_t eth_address_t[ETH_ADDRESS_SIZE];
typedef uint8_t eth_hash_t[ETH_HASH_SIZE];

typedef struct eth_transaction_t {
    eth_address_t from;
    eth_address_t to;
    uint64_t nonce;
    int gas;
    int gasPrice;
    int value;
    uint8_t *data; //for contracts
} eth_transaction_t;


void eth_init();

void eth_clearAddresses();
uint32_t eth_listAddresses(eth_address_t *addresses);
void eth_newAddress();

eth_hash_t* eth_signTransaction(eth_transaction_t transaction);
void eth_dumpTransaction(eth_transaction_t *transaction);

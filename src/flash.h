#pragma once
#include <nrf_fstorage.h>

//Addresses must be a multiple of the page size (4K)
#define FLASH_ETH_START 0xF0000
#define FLASH_ETH_END   0xF1000
#define FLASH_ETH_PAGES 1

void flash_init(nrf_fstorage_t *p_fstorage);
void flash_print_info(nrf_fstorage_t *p_fstorage);
void flash_wait_ready(nrf_fstorage_t const *p_fstorage);
uint32_t flash_end_addr();

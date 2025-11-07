#include <app_error.h>
#include <nrf_fstorage.h>
#include <nrf_fstorage_sd.h>
#include <nrf_soc.h>
#include <nrf_log.h>
#include "flash.h"

void flash_init(nrf_fstorage_t *p_fstorage)
{
    ret_code_t rc;
    rc = nrf_fstorage_init(p_fstorage, &nrf_fstorage_sd, NULL);
    APP_ERROR_CHECK(rc);
}

void flash_wait_ready(nrf_fstorage_t const *p_fstorage)
{
    /* While fstorage is busy, sleep and wait for an event. */
    while (nrf_fstorage_is_busy(p_fstorage))
    {
        (void) sd_app_evt_wait();
    }
}

uint32_t flash_end_addr()
{
    uint32_t const bootloader_addr = NRF_UICR->NRFFW[0];
    uint32_t const page_sz         = NRF_FICR->CODEPAGESIZE;
    uint32_t const code_sz         = NRF_FICR->CODESIZE;

    return (bootloader_addr != 0xFFFFFFFF ?
            bootloader_addr : (code_sz * page_sz));
}

void flash_print_info(nrf_fstorage_t *p_fstorage)
{
    NRF_LOG_INFO("========| flash info |========");
    //NRF_LOG_INFO("p_flash_info=%p", p_fstorage->p_flash_info);
    NRF_LOG_INFO("end addr: 0x%x", flash_end_addr());
    NRF_LOG_INFO("erase unit:   %d bytes", p_fstorage->p_flash_info->erase_unit);
    NRF_LOG_INFO("program unit: %d bytes", p_fstorage->p_flash_info->program_unit);
    NRF_LOG_INFO("==============================");
}

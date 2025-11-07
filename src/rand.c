#include <stdint.h>
#include <nrf_delay.h>
#include <app_error.h>
#include <nrf_drv_rng.h>
#include <nrf_assert.h>


#include <nrf_log.h>
#include <nrf_log_ctrl.h>
#include <nrf_log_default_backends.h>


uint8_t rand_generate(uint8_t * p_buff, uint8_t size)
{
    uint8_t  available;
    nrf_drv_rng_bytes_available(&available);
    uint8_t length = MIN(size, available);

    uint32_t err_code = nrf_drv_rng_rand(p_buff, length);
    APP_ERROR_CHECK(err_code);

    return length;
}

//In place Fisher-Yates shuffle
void shuffle(uint8_t *buff, uint8_t *rand, uint8_t n)
{
	for (uint8_t i=n-1; i>0; i--) {
		uint8_t j = rand[i]%i;
		uint8_t tmp = buff[i];
		buff[i] = buff[j];
		buff[j] = tmp;
	}
}

void rand_init(void)
{
    uint32_t err_code = nrf_drv_rng_init(NULL);
    APP_ERROR_CHECK(err_code);
}

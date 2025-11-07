#include "app_timer.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "app_timer.h"

#include "app.h"
#include "app_ble.h"
#include "oled.h"
#include "cli.h"
#include "screen.h"

/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    //NRF_LOG_DEFAULT_BACKENDS_INIT();
}


/**@brief Function for the Timer initialization.
 */
static void timers_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for starting timers.
 */
static void application_timers_start(void)
{
    /* YOUR_JOB: Start your timers. below is an example of how to start a timer.
       ret_code_t err_code;
       err_code = app_timer_start(m_app_timer_id, TIMER_INTERVAL, NULL);
       APP_ERROR_CHECK(err_code); */

}

// Go to system-off mode (this function will not return)
void power_off() {
    ret_code_t err_code;
    err_code = sd_power_system_off();
    #ifdef DEBUG_NRF
        // For reasons unknown to man, we don't actually power off in debug mode.
        // Therefore we have to stop code execution and prevent a return ourselves,
        // otherwise "bad things happen" (tm).
        __BKPT(0);
    #endif
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for putting the chip into sleep mode.
 *
 * @note This function will not return.
 */
void sleep_mode_enter(void)
{
    ret_code_t err_code;

    err_code = bsp_indication_set(BSP_INDICATE_IDLE);
    APP_ERROR_CHECK(err_code);

    app_ble_sleep();
    power_off();
}


/**@brief Function for handling events from the BSP module.
 *
 * @param[in]   event   Event generated when button is pressed.
 */
static void bsp_event_handler(bsp_event_t event)
{
    NRF_LOG_INFO("bsp_evt: %i", event);
    screen_evt(event);
    switch (event)
    {
        case BSP_EVENT_SLEEP:
            sleep_mode_enter();
            break;

        case BSP_EVENT_DISCONNECT:
            app_disconnect();
            break;

        case BSP_EVENT_CLEAR_BONDING_DATA:
            app_delete_bonds();
            break;

        case BSP_EVENT_ADVERTISING_START:
            app_advertising_start(false);
            break;

        case BSP_EVENT_ADVERTISING_STOP:
            app_advertising_stop();
            break;

        case BSP_EVENT_RESET:
            NRF_LOG_INFO("Resetting...");
            NRF_LOG_FLUSH();
            sd_nvic_SystemReset();
            break;

        case BSP_EVENT_SYSOFF:
            power_off();
            break;

        default:
            break;
    }
}


/**@brief Function for initializing the bsp
 *
 * @param[out] p_erase_bonds  Will be true if the clear bonding button was pressed to wake the application up.
 */
static void app_bsp_init()
{
    ret_code_t err_code = bsp_init(BSP_INIT_LED | BSP_INIT_BUTTONS, bsp_event_handler);
    APP_ERROR_CHECK(err_code);
}


void app_bsp_event(bsp_event_t event) {
    bsp_event_handler(event);
}

/**@brief Function for the Power manager.
 */
/*static void power_manage(void)
{
    ret_code_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}*/


/**@brief Function for application main entry.
 */
int main(void)
{
    // Initialize.
    log_init();
    timers_init();
    cli_init();
    app_bsp_init();

    // Start execution.
    NRF_LOG_INFO("app started!");
    application_timers_start();
    oled_init();

    // Start bluetooth
    app_ble_init();

    // Enter main loop.
    for (;;)
    {
        NRF_LOG_PROCESS();
        cli_process();
    }
}

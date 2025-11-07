#include <nrf_cli.h>
#include <nrf_cli_rtt.h>
#include <nrf_cli_types.h>
#include "app.h"

static void cmd_reset(nrf_cli_t const * p_cli, size_t argc, char **argv) {
    app_bsp_event(BSP_EVENT_RESET);
}

static void cmd_sleep(nrf_cli_t const * p_cli, size_t argc, char **argv) {
    app_bsp_event(BSP_EVENT_SLEEP);
}

static void cmd_sysoff(nrf_cli_t const * p_cli, size_t argc, char **argv) {
    app_bsp_event(BSP_EVENT_SYSOFF);
}

static void cmd_disconnect(nrf_cli_t const * p_cli, size_t argc, char **argv) {
    app_bsp_event(BSP_EVENT_DISCONNECT);
}

static void cmd_clear_bonding(nrf_cli_t const * p_cli, size_t argc, char **argv) {
    app_bsp_event(BSP_EVENT_CLEAR_BONDING_DATA);
}

static void cmd_adv_start(nrf_cli_t const * p_cli, size_t argc, char **argv) {
    app_bsp_event(BSP_EVENT_ADVERTISING_START);
}

static void cmd_adv_stop(nrf_cli_t const * p_cli, size_t argc, char **argv) {
    app_bsp_event(BSP_EVENT_ADVERTISING_STOP);
}

NRF_CLI_RTT_DEF(m_cli_rtt_transport);
NRF_CLI_DEF(m_cli_rtt, "$ ", &m_cli_rtt_transport.transport, '\n', 4);
NRF_CLI_CMD_REGISTER(reset, NULL, "restarts the board", cmd_reset);
NRF_CLI_CMD_REGISTER(sleep, NULL, "press any key to wake", cmd_sleep);
NRF_CLI_CMD_REGISTER(sysoff, NULL, "power off", cmd_sysoff);
NRF_CLI_CMD_REGISTER(disconnect, NULL, "disconnects from the host", cmd_disconnect);
NRF_CLI_CMD_REGISTER(bond_clear, NULL, "forgets previously bonded devices", cmd_clear_bonding);
NRF_CLI_CMD_REGISTER(adv_start, NULL, "start advertising", cmd_adv_start);
NRF_CLI_CMD_REGISTER(adv_stop, NULL, "stop advertising", cmd_adv_stop);

void cli_init() {
    ret_code_t err_code = nrf_cli_init(&m_cli_rtt, NULL, true, true, NRF_LOG_SEVERITY_INFO);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_cli_start(&m_cli_rtt);
    APP_ERROR_CHECK(err_code);
}

void cli_process() {
    nrf_cli_process(&m_cli_rtt);
}

set(TARGET_NAME cli)

add_library(${TARGET_NAME} STATIC
    ${SDK_DIR}/components/libraries/cli/nrf_cli.c
    ${SDK_DIR}/components/libraries/cli/rtt/nrf_cli_rtt.c
    ${SDK_DIR}/components/libraries/pwr_mgmt/nrf_pwr_mgmt.c
    ${SDK_DIR}/components/libraries/queue/nrf_queue.c
)

target_include_directories(${TARGET_NAME} SYSTEM PUBLIC
    ${SDK_DIR}/components/drivers_nrf/hal
    ${SDK_DIR}/components/libraries/cli/rtt
    ${SDK_DIR}/components/libraries/mutex
    ${SDK_DIR}/components/libraries/pwr_mgmt
)

target_link_libraries(${TARGET_NAME} log segger_rtt timer)

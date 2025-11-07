set(TARGET_NAME bsp)

add_library(${TARGET_NAME} STATIC
    ${SDK_DIR}/components/boards/boards.c
    ${SDK_DIR}/components/drivers_nrf/common/nrf_drv_common.c
    ${SDK_DIR}/components/drivers_nrf/gpiote/nrf_drv_gpiote.c
    ${SDK_DIR}/components/libraries/bsp/bsp.c
    ${SDK_DIR}/components/libraries/bsp/bsp_btn_ble.c
    ${SDK_DIR}/components/libraries/button/app_button.c
)

target_include_directories(${TARGET_NAME} SYSTEM PUBLIC
    ${SDK_DIR}/components/boards
    ${SDK_DIR}/components/drivers_nrf/gpiote
    ${SDK_DIR}/components/drivers_nrf/hal
    ${SDK_DIR}/components/libraries/bsp
    ${SDK_DIR}/components/libraries/button
)

target_link_libraries(${TARGET_NAME} log timer)

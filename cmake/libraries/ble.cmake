set(TARGET_NAME ble)
add_library(${TARGET_NAME} STATIC
    ${SDK_DIR}/components/ble/ble_advertising/ble_advertising.c
    ${SDK_DIR}/components/ble/common/ble_advdata.c
    ${SDK_DIR}/components/ble/common/ble_conn_params.c
    ${SDK_DIR}/components/ble/common/ble_conn_state.c
    ${SDK_DIR}/components/ble/common/ble_srv_common.c
    ${SDK_DIR}/components/ble/nrf_ble_gatt/nrf_ble_gatt.c
    ${SDK_DIR}/components/softdevice/common/nrf_sdh_ble.c
)

target_include_directories(${TARGET_NAME} SYSTEM PUBLIC
    ${SDK_DIR}/components/ble/ble_advertising
)

target_compile_definitions(${TARGET_NAME} PUBLIC
    -DCONFIG_GPIO_AS_PINRESET # Not needed?
    -DNRF_SD_BLE_API_VERSION=5
    -DSWI_DISABLE0 # Not needed?
)

target_link_libraries(${TARGET_NAME} log peer_manager timer util)

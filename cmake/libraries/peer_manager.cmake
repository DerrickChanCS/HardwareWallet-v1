set(TARGET_NAME peer_manager)

add_library(${TARGET_NAME} STATIC
    ${SDK_DIR}/components/ble/peer_manager/gatt_cache_manager.c
    ${SDK_DIR}/components/ble/peer_manager/gatts_cache_manager.c
    ${SDK_DIR}/components/ble/peer_manager/id_manager.c
    ${SDK_DIR}/components/ble/peer_manager/peer_data_storage.c
    ${SDK_DIR}/components/ble/peer_manager/peer_database.c
    ${SDK_DIR}/components/ble/peer_manager/peer_id.c
    ${SDK_DIR}/components/ble/peer_manager/peer_manager.c
    ${SDK_DIR}/components/ble/peer_manager/pm_buffer.c
    ${SDK_DIR}/components/ble/peer_manager/pm_mutex.c
    ${SDK_DIR}/components/ble/peer_manager/security_dispatcher.c
    ${SDK_DIR}/components/ble/peer_manager/security_manager.c
    ${SDK_DIR}/components/libraries/fds/fds.c
)

target_include_directories(${TARGET_NAME} SYSTEM PUBLIC
    ${SDK_DIR}/components/ble/common
    ${SDK_DIR}/components/ble/nrf_ble_gatt
    ${SDK_DIR}/components/ble/peer_manager
    ${SDK_DIR}/components/libraries/fds
)

target_link_libraries(${TARGET_NAME} ble fstorage)

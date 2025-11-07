set(TARGET_NAME nfc_hal)
add_library(${TARGET_NAME} STATIC
    ${SDK_DIR}/components/drivers_nrf/clock/nrf_drv_clock.c
    ${SDK_DIR}/components/nfc/t2t_lib/hal_t2t/hal_nfc_t2t.c
)
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC
    ${SDK_DIR}/components/drivers_nrf/clock
    ${SDK_DIR}/components/drivers_nrf/hal
    ${SDK_DIR}/components/nfc/t2t_lib
)
target_link_libraries(${TARGET_NAME} log)

set(TARGET_NAME nfc_t2t)
add_library(${TARGET_NAME} STATIC IMPORTED)
set_target_properties(${TARGET_NAME} PROPERTIES
    IMPORTED_LOCATION ${SDK_DIR}/components/nfc/t2t_lib/nfc_t2t_lib_gcc.a
    INTERFACE_LINK_LIBRARIES nfc_hal
)

set(TARGET_NAME micro_ecc)
add_library(${TARGET_NAME} STATIC IMPORTED)
set_target_properties(${TARGET_NAME} PROPERTIES
    IMPORTED_LOCATION ${SDK_DIR}/external/micro-ecc/${FAMILY_L}_armgcc/armgcc/micro_ecc_lib_${FAMILY_L}.a
)


set(TARGET_NAME nfc)
add_library(${TARGET_NAME} STATIC
    ${SDK_DIR}/components/drivers_nrf/rng/nrf_drv_rng.c
    ${SDK_DIR}/components/libraries/ecc/ecc.c
    ${SDK_DIR}/components/nfc/ndef/connection_handover/ac_rec/nfc_ac_rec.c
    ${SDK_DIR}/components/nfc/ndef/connection_handover/ble_oob_advdata/nfc_ble_oob_advdata.c
    ${SDK_DIR}/components/nfc/ndef/connection_handover/ble_pair_lib/nfc_ble_pair_lib.c
    ${SDK_DIR}/components/nfc/ndef/connection_handover/ble_pair_msg/nfc_ble_pair_msg.c
    ${SDK_DIR}/components/nfc/ndef/connection_handover/common/nfc_ble_pair_common.c
    ${SDK_DIR}/components/nfc/ndef/connection_handover/ep_oob_rec/nfc_ep_oob_rec.c
    ${SDK_DIR}/components/nfc/ndef/connection_handover/hs_rec/nfc_hs_rec.c
    ${SDK_DIR}/components/nfc/ndef/connection_handover/le_oob_rec/nfc_le_oob_rec.c
    ${SDK_DIR}/components/nfc/ndef/generic/message/nfc_ndef_msg.c
    ${SDK_DIR}/components/nfc/ndef/generic/record/nfc_ndef_record.c
    #${SDK_DIR}/external/micro-ecc/micro-ecc/uECC.c # Causes BLE_GAP_SEC_DHKEY_FAILURE?
)

target_include_directories(${TARGET_NAME} SYSTEM PUBLIC
    ${SDK_DIR}/components/drivers_nrf/rng
    ${SDK_DIR}/components/libraries/ecc
    ${SDK_DIR}/components/nfc/ndef/connection_handover/ac_rec
    ${SDK_DIR}/components/nfc/ndef/connection_handover/ble_oob_advdata
    ${SDK_DIR}/components/nfc/ndef/connection_handover/ble_pair_lib
    ${SDK_DIR}/components/nfc/ndef/connection_handover/ble_pair_msg
    ${SDK_DIR}/components/nfc/ndef/connection_handover/common
    ${SDK_DIR}/components/nfc/ndef/connection_handover/ep_oob_rec
    ${SDK_DIR}/components/nfc/ndef/connection_handover/hs_rec
    ${SDK_DIR}/components/nfc/ndef/connection_handover/le_oob_rec
    ${SDK_DIR}/components/nfc/ndef/generic/message
    ${SDK_DIR}/components/nfc/ndef/generic/record
    ${SDK_DIR}/external/micro-ecc/micro-ecc
)

target_link_libraries(${TARGET_NAME} log bsp ble nfc_t2t micro_ecc)

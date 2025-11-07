if (NOT SOFTDEVICE)
    message(FATAL_ERROR "SOFTDEVICE must be set")
endif()
if (NOT EXISTS ${SDK_DIR})
    message(FATAL_ERROR "SDK_DIR must be set")
endif()

set(SOFTDEVICE_HEX_DIR "${SDK_DIR}/components/softdevice/${SOFTDEVICE_L}/hex")
set(SOFTDEVICE_HEX_FILE "${SOFTDEVICE_L}_${DEVICE_L}_${SOFTDEVICE_VERSION}_softdevice.hex")

add_definitions(
    -D${SOFTDEVICE}
    -DSOFTDEVICE_PRESENT
)
include_directories(
    ${SDK_DIR}/components/softdevice/common
    ${SDK_DIR}/components/softdevice/${SOFTDEVICE_L}/headers
)
list(APPEND SRC_FILES
    ${SDK_DIR}/components/softdevice/common/nrf_sdh.c
    ${SDK_DIR}/components/softdevice/common/nrf_sdh_soc.c
)

add_custom_target(flash_softdevice
    COMMAND nrfjprog -f ${FAMILY} --program ${SOFTDEVICE_HEX_DIR}/${SOFTDEVICE_HEX_FILE} --chiperase --verify
    COMMAND nrfjprog -f ${FAMILY} --reset
)

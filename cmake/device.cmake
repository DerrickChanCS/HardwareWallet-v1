if (NOT EXISTS ${SDK_DIR})
    message(FATAL_ERROR "SDK_DIR must be set")
endif()

string(TOLOWER "${FAMILY}" FAMILY_L)
string(TOLOWER "${DEVICE}" DEVICE_L)
string(TOLOWER "${VARIENT}" VARIENT_L)
string(TOLOWER "${BOARD}" BOARD_L)
string(TOLOWER "${SOFTDEVICE}" SOFTDEVICE_L)

link_directories(${PROJECT_SOURCE_DIR})
if (SOFTDEVICE)
    set(DEVICE_LD "${SOFTDEVICE_L}_${DEVICE_L}_${VARIENT_L}.ld")
else()
    set(DEVICE_LD "-T${SDK_DIR}/components/toolchain/gcc/${DEVICE_L}_${VARIENT_L}.ld")
    include_directories(${SDK_DIR}/components/drivers_nrf/nrf_soc_nosd)
endif()
string(APPEND CMAKE_EXE_LINKER_FLAGS " -T${DEVICE_LD}")

list(APPEND SRC_FILES
    ${SDK_DIR}/components/toolchain/gcc/gcc_startup_${DEVICE_L}.S
    ${SDK_DIR}/components/toolchain/system_${DEVICE_L}.c
)

string(APPEND COMMON_FLAGS " -mcpu=cortex-m4 -mthumb -mabi=aapcs -mfloat-abi=hard -mfpu=fpv4-sp-d16")
add_definitions(
    -D${DEVICE}_${VARIENT}
    -DBOARD_${BOARD}
)

add_custom_target(upload
    COMMAND nrfjprog -f ${FAMILY} --program ${OUT}.hex --sectorerase --verify --fast
    COMMAND nrfjprog -f ${FAMILY} --reset
    DEPENDS ${OUT}
)

add_custom_target(reset
    COMMAND nrfjprog -f ${FAMILY} --reset
)

add_custom_target(erase
    COMMAND nrfjprog -f ${FAMILY} --eraseall
)

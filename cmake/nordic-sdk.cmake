if (NOT DEVICE OR NOT VARIENT)
    message(FATAL_ERROR "DEVICE and VARIENT must be set")
endif()
if (NOT EXISTS ${SDK_DIR})
    message(FATAL_ERROR "SDK_DIR must be set")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG_NRF)
endif()

link_directories(
    "${SDK_DIR}/components/toolchain/gcc"
)
include_directories(
    ${SDK_DIR}/components/device
    ${SDK_DIR}/components/drivers_nrf/common
    ${SDK_DIR}/components/drivers_nrf/hal
    ${SDK_DIR}/components/libraries/util
    ${SDK_DIR}/components/toolchain
    ${SDK_DIR}/components/toolchain/cmsis/include
    ${SDK_DIR}/config
)

add_custom_target(ctags
    COMMAND ctags -f ${PROJECT_SOURCE_DIR}/nordic-sdk.ctags --exclude=examples -R ${SDK_DIR}
)

add_custom_target(micro-ecc
    COMMAND cd ${SDK_DIR}/external/micro-ecc\; git clone https://github.com/kmackay/micro-ecc\; cd ${FAMILY_L}_armgcc/armgcc\; make GNU_INSTALL_ROOT=''
)

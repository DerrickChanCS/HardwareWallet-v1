set(TARGET_NAME util)

add_library(${TARGET_NAME} STATIC
    ${SDK_DIR}/components/libraries/hardfault/hardfault_implementation.c
    ${SDK_DIR}/components/libraries/util/app_error.c
    ${SDK_DIR}/components/libraries/util/app_error_weak.c
    ${SDK_DIR}/components/libraries/util/app_util_platform.c
    ${SDK_DIR}/components/libraries/util/nrf_assert.c
    ${SDK_DIR}/components/libraries/util/sdk_mapped_flags.c
)

target_link_libraries(${TARGET_NAME} log)

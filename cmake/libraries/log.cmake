set(TARGET_NAME log)

add_library(${TARGET_NAME} STATIC
    ${SDK_DIR}/components/libraries/balloc/nrf_balloc.c
    ${SDK_DIR}/components/libraries/experimental_log/src/nrf_log_frontend.c
    ${SDK_DIR}/components/libraries/experimental_log/src/nrf_log_str_formatter.c
    ${SDK_DIR}/components/libraries/experimental_memobj/nrf_memobj.c
    ${SDK_DIR}/components/libraries/strerror/nrf_strerror.c
    ${SDK_DIR}/external/fprintf/nrf_fprintf.c
    ${SDK_DIR}/external/fprintf/nrf_fprintf_format.c
)

target_include_directories(${TARGET_NAME} SYSTEM PUBLIC
    ${SDK_DIR}/components/libraries/atomic
    ${SDK_DIR}/components/libraries/balloc
    ${SDK_DIR}/components/libraries/cli
    ${SDK_DIR}/components/libraries/experimental_log
    ${SDK_DIR}/components/libraries/experimental_log/src
    ${SDK_DIR}/components/libraries/experimental_memobj
    ${SDK_DIR}/components/libraries/experimental_section_vars
    ${SDK_DIR}/components/libraries/queue
    ${SDK_DIR}/components/libraries/strerror
    ${SDK_DIR}/external/fprintf
)

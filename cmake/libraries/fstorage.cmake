set(TARGET_NAME fstorage)

add_library(${TARGET_NAME} STATIC
    ${SDK_DIR}/components/libraries/fstorage/nrf_fstorage.c
    ${SDK_DIR}/components/libraries/fstorage/nrf_fstorage_sd.c
    ${SDK_DIR}/components/libraries/experimental_section_vars/nrf_section_iter.c
    ${SDK_DIR}/components/libraries/atomic_fifo/nrf_atfifo.c
)

target_include_directories(${TARGET_NAME} SYSTEM PUBLIC
    ${SDK_DIR}/components/libraries/fstorage
    ${SDK_DIR}/components/libraries/experimental_section_vars
    ${SDK_DIR}/components/libraries/atomic
    ${SDK_DIR}/components/libraries/atomic_fifo
)

target_link_libraries(${TARGET_NAME} log)

set(TARGET_NAME rand)

add_library(${TARGET_NAME} STATIC
    ${SDK_DIR}/components/drivers_nrf/rng/nrf_drv_rng.c
)

target_include_directories(${TARGET_NAME} SYSTEM PUBLIC
    ${SDK_DIR}/components/drivers_nrf/rng
)

target_link_libraries(${TARGET_NAME} log)

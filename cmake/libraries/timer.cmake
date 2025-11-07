set(TARGET_NAME timer)

add_library(${TARGET_NAME} STATIC
    ${SDK_DIR}/components/libraries/timer/app_timer.c
)

target_include_directories(${TARGET_NAME} SYSTEM PUBLIC
    ${SDK_DIR}/components/drivers_nrf/delay
    ${SDK_DIR}/components/libraries/timer
)

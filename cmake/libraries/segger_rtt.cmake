set(TARGET_NAME segger_rtt)

add_library(${TARGET_NAME} STATIC
    ${SDK_DIR}/external/segger_rtt/SEGGER_RTT.c
    #${SDK_DIR}/external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c
    #${SDK_DIR}/external/segger_rtt/SEGGER_RTT_printf.c
)

target_include_directories(${TARGET_NAME} SYSTEM PUBLIC
    ${SDK_DIR}/external/segger_rtt
)

set(TARGET_NAME mpack)
set(MPACK_RELEASE 0.8.2)

add_library(${TARGET_NAME} STATIC
    external/mpack-amalgamation-${MPACK_RELEASE}/src/mpack/mpack.c
)

target_include_directories(${TARGET_NAME} SYSTEM PUBLIC
    external/mpack-amalgamation-${MPACK_RELEASE}/src/mpack
)

add_custom_target(mpack_install
    COMMAND
)

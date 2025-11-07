set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_OSX_DEPLOYMENT_TARGET "")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(TOOLCHAIN "arm-none-eabi")
set(CMAKE_C_COMPILER "${TOOLCHAIN}-gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN}-g++")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN}-gcc")
set(CMAKE_RANLIB "${TOOLCHAIN}-ranlib")
set(CMAKE_AR "${TOOLCHAIN}-ar")

# Fix arch_paths_first warning
SET(CMAKE_C_LINK_FLAGS)
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

# Remove unused code
string(APPEND COMMON_FLAGS " -std=gnu99 -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin --short-enums")
string(APPEND CMAKE_EXE_LINKER_FLAGS " --specs=nano.specs -Wl,--gc-sections,-Map=${OUT}.map")

set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} -x assembler-with-cpp")

set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

set(CMAKE_EXECUTABLE_SUFFIX .elf)
set(OUT_FILE "${OUT}${CMAKE_EXECUTABLE_SUFFIX}")

add_executable(${OUT} ${SRC_FILES})
add_custom_command(TARGET ${OUT} POST_BUILD
    COMMAND ${TOOLCHAIN}-objcopy -O ihex ${OUT_FILE} ${OUT}.hex
    COMMAND ${TOOLCHAIN}-size ${OUT_FILE}
    COMMAND ctags -f ${PROJECT_SOURCE_DIR}/tags --exclude=build -R ${PROJECT_SOURCE_DIR}
)

set(JLINK_FLAGS -device ${DEVICE}_${VARIENT} -if swd -speed 4000)
add_custom_target(jlink
    COMMAND JLinkExe ${JLINK_FLAGS} -AutoConnect 1
)

add_custom_target(debug-server
    COMMAND JLinkGDBServer ${JLINK_FLAGS}
)

add_custom_target(debug
    COMMAND ${TOOLCHAIN}-gdb -q -se ${OUT_FILE} -ex 'target remote localhost:2331'
    DEPENDS ${OUT}
)

add_custom_target(rtt
    COMMAND stty -echo erase ^C\; JLinkRTTClient
)

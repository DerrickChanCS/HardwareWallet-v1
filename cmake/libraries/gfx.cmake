set(TARGET_NAME gfx)
add_library(${TARGET_NAME} STATIC
    ${SDK_DIR}/components/drivers_nrf/spi_master/nrf_drv_spi.c
    ${SDK_DIR}/components/libraries/gfx/nrf_gfx.c
    ${SDK_DIR}/components/libraries/spi_mngr/nrf_spi_mngr.c
    ${SDK_DIR}/external/thedotfactory_fonts/orkney8pts.c
)

target_include_directories(${TARGET_NAME} SYSTEM PUBLIC
    ${SDK_DIR}/components/drivers_nrf/spi_master
    ${SDK_DIR}/components/libraries/gfx
    ${SDK_DIR}/components/libraries/spi_mngr
    ${SDK_DIR}/external/thedotfactory_fonts
)

target_link_libraries(${TARGET_NAME} log)


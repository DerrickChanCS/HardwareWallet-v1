#pragma once
#include <sdk_errors.h>
#include <nrf_lcd.h>

// Details concerning used OLED display. The one used in this project is 128x64 pixels.
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


// Functions below are required by nrf_gfx library

/**
 * @brief Function for initializing an SPI transaction manager and the OLED controller.
 */
ret_code_t ssd1306_init(void);


/**
 * @brief Function for uninitializing an SPI transaction manager instance and for clearing
 * the OLED display buffer
 *
 */
void ssd1306_uninit(void);


/**
 * @brief Function for drawing a single pixel in the display buffer.
 *
 * @param[in] x             Horizontal coordinate of the pixel.
 * @param[in] y             Vertical coordinate of the pixel.
 * @param[in] color         Color of the pixel in OLED accepted format.
 */
void ssd1306_pixel_draw(uint16_t x, uint16_t y, uint32_t color);


/**
 * @brief Function for drawing a filled rectangle in the display buffer.
 *
 * @param[in] x             Horizontal coordinate of the point where to start drawing the rectangle.
 * @param[in] y             Vertical coordinate of the point where to start drawing the rectangle.
 * @param[in] width         Width of the image.
 * @param[in] height        Height of the image.
 * @param[in] color         Color with which to fill the rectangle in OLED accepted format.
 */
void ssd1306_rect_draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);


/**
 * @brief Function for displaying data from a display buffer
 *
 */
void ssd1306_display_screen(void);


/**
 * @brief Function for setting inversion of colors on the screen.
 *
 * @param[in] invert        If true, inversion will be set.
 */
void ssd1306_display_invert(bool invert);


/**
 * @brief Function for clearing the display buffer and the OLED screen
 *
 */
void ssd1306_clear_display(void);

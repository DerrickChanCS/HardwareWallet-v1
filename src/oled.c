#include <nrf_lcd.h>
#include <nrf_gfx.h>
#include "util.h"
#include "oled.h"

extern const nrf_gfx_font_desc_t orkney_8ptFontInfo;

static lcd_cb_t m_lcd_cb =
{
    .state = NRF_DRV_STATE_UNINITIALIZED,
    .height = SCREEN_HEIGHT,
    .width = SCREEN_WIDTH
};

/* Dummy function. Rotation is not used but the function is required
 * by nrf_gfx module. */
static void dummy_lcd_rotation_set(nrf_lcd_rotation_t rotation)
{
    UNUSED_PARAMETER(rotation);
}

/* Below functions are used by NRF GFX library. Rotation is not needed in this example. */
static const nrf_lcd_t m_nrf_lcd =
{
    .lcd_init   = ssd1306_init,
    .lcd_uninit = ssd1306_uninit,
    .lcd_pixel_draw = ssd1306_pixel_draw,
    .lcd_rect_draw = ssd1306_rect_draw,
    .lcd_display = ssd1306_display_screen,
    .lcd_rotation_set = dummy_lcd_rotation_set,
    .lcd_display_invert = ssd1306_display_invert,
    .p_lcd_cb = &m_lcd_cb
};

void oled_init()
{
    APP_ERROR_CHECK(nrf_gfx_init(&m_nrf_lcd));
    oled_clear();
    oled_redraw();
}

void oled_clear() {
    ssd1306_clear_display();
}

void oled_redraw() {
    ssd1306_display_screen();
}

void oled_invert(bool invert) {
    nrf_gfx_invert(&m_nrf_lcd, invert);
}

void oled_write(uint8_t x, uint8_t line, const char *text)
{
    nrf_gfx_point_t const text_point =
    {
        .x = x,
        .y = line*LINE_HEIGHT
    };
    APP_ERROR_CHECK(nrf_gfx_print(&m_nrf_lcd, &text_point, 1, text, &orkney_8ptFontInfo, true));
}

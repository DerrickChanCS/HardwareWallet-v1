#include <nrf_delay.h>
#include <nrf_gpio.h>
#include <nrf_spi_mngr.h>
#include <nrf_log.h>
#include "ssd1306.h"

#define SSD1306_PIN_RESET NRF_GPIO_PIN_MAP(0, 14)
#define SSD1306_PIN_DC NRF_GPIO_PIN_MAP(0, 15)
#define SSD1306_PIN_SCK NRF_GPIO_PIN_MAP(0, 27)
#define SSD1306_PIN_MOSI NRF_GPIO_PIN_MAP(0, 2)
#define SSD1306_PIN_SS NRF_DRV_SPI_PIN_NOT_USED
//#define SSD1306_PIN_SS NRF_GPIO_PIN_MAP(0, 0) //NRF_DRV_SPI_PIN_NOT_USED

#define SSD1306_PAGE_HEIGHT 8
#define SSD1306_PAGES (SCREEN_HEIGHT / SSD1306_PAGE_HEIGHT)

#define SSD1306_DC_CMD 0
#define SSD1306_DC_DATA 1

#define SSD1306_MODE_HORIZ 0
#define SSD1306_MODE_VERT 1
#define SSD1306_MODE_PAGE 2

#define SSD1306_CONTRAST 0x81
#define SSD1306_ALLON 0xA4
#define SSD1306_ALLON_FORCE 0xA5
#define SSD1306_DISPLAY_NORMAL 0xA6
#define SSD1306_DISPLAY_INVERSE 0xA7
#define SSD1306_DISPLAY_OFF 0xAE
#define SSD1306_DISPLAY_ON 0xAF

#define SSD1306_COL_L 0x00
#define SSD1306_COL_H 0x10
#define SSD1306_ADDRESS_MODE 0x20
#define SSD1306_ADDRESS_HORIZ 0
#define SSD1306_ADDRESS_VERT 1
#define SSD1306_ADDRESS_PAGE 2
#define SSD1306_SET_COL 0x21
#define SSD1306_SET_PAGE 0x22
#define SSD1306_PAGE 0xB0

#define SSD1306_START_LINE 0x40
#define SSD1306_SEG_NORM 0xA0
#define SSD1306_SEG_INV 0xA1
#define SSD1306_MUX_RATIO 0xA8
#define SSD1306_COM_NORM 0xC0
#define SSD1306_COM_INV 0xC8
#define SSD1306_OFFSET 0xD3
#define SSD1306_COM_HW 0xDA

#define SSD1306_CLK_DIV 0xD5
#define SSD1306_PRECHRG 0xD9
#define SSD1306_DESELECT 0xDB
#define SSD1306_NOP 0xE3

#define SSD1306_CHARGE_PUMP 0x8D
#define SSD1306_CHARGE_PUMP_OFF 0x10
#define SSD1306_CHARGE_PUMP_ON 0x14

#define SSD1306_SPI_QUEUE_LENGTH 6
#define SSD1306_SPI_INSTANCE_ID 0

#define contrast 127

NRF_SPI_MNGR_DEF(m_nrf_spi_mngr, SSD1306_SPI_QUEUE_LENGTH, SSD1306_SPI_INSTANCE_ID);

// Buffer for monochromatic display based on SSD1306 driver.
// Each bit of a buffer corresponds to different pixel on a screen.
static uint8_t m_display_buffer[SCREEN_WIDTH * SSD1306_PAGES];

// Buffer with commands used to display m_display_buffer on a screen.
// Moves "cursor" to 0,0 so that the whole screen can be sent
static NRF_SPI_MNGR_BUFFER_LOC_IND uint8_t m_command_buffer[] =
{
    SSD1306_SET_COL, 0, SCREEN_WIDTH-1,
    SSD1306_SET_PAGE, 0, SSD1306_PAGES-1,
};

// Initialization commands for SSD1306 driver.
static NRF_SPI_MNGR_BUFFER_LOC_IND uint8_t m_initialization[] =
{
    SSD1306_MUX_RATIO, SCREEN_HEIGHT-1,
    SSD1306_OFFSET, 0,
    SSD1306_COL_L | 0,
    SSD1306_START_LINE | 0,
    SSD1306_SEG_INV, SSD1306_COM_INV,
    SSD1306_COM_HW, 0x12,
    SSD1306_ADDRESS_MODE, SSD1306_ADDRESS_HORIZ,
    SSD1306_CONTRAST, contrast,
    SSD1306_ALLON, SSD1306_DISPLAY_NORMAL,
    SSD1306_CLK_DIV, 0x80,
    SSD1306_CHARGE_PUMP, SSD1306_CHARGE_PUMP_ON,
    SSD1306_DISPLAY_ON
};


// Function called before SPI command is send to the OLED display. It is used to drive D/C pin low.
static void ssd1306_set_command_dc(void * p_user_data)
{
    nrf_gpio_pin_clear(SSD1306_PIN_DC);
}


// Function called before SPI data is send to the OLED display. It is used to drive D/C pin high.
static void ssd1306_set_data_dc(void * p_user_data)
{
    nrf_gpio_pin_set(SSD1306_PIN_DC);
}


static ret_code_t init_spi0_master(void)
{
    // SPI0 (with transaction manager) initialization.
    nrf_drv_spi_config_t config = NRF_DRV_SPI_DEFAULT_CONFIG;
    config.sck_pin   = SSD1306_PIN_SCK;
    config.mosi_pin  = SSD1306_PIN_MOSI;
    config.ss_pin    = SSD1306_PIN_SS;
    config.frequency = NRF_DRV_SPI_FREQ_8M;
    config.mode      = NRF_DRV_SPI_MODE_0;
    config.bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST;
    return nrf_spi_mngr_init(&m_nrf_spi_mngr, &config);
}


void ssd1306_clear_display(void)
{
    memset((void *)m_display_buffer, 0x00, sizeof(m_display_buffer));
}


ret_code_t ssd1306_init(void)
{
    ret_code_t err_code;
    err_code = init_spi0_master();

    if (err_code == NRF_SUCCESS)
    {
        memset((void *)m_display_buffer, 0x00, sizeof(m_display_buffer));

        nrf_gpio_pin_dir_set(SSD1306_PIN_DC, NRF_GPIO_PIN_DIR_OUTPUT);
        nrf_gpio_pin_dir_set(SSD1306_PIN_RESET, NRF_GPIO_PIN_DIR_OUTPUT);

        nrf_delay_ms(10);
        nrf_gpio_pin_clear(SSD1306_PIN_RESET);
        nrf_delay_ms(10);
        nrf_gpio_pin_set(SSD1306_PIN_RESET);
        nrf_delay_ms(10);

        nrf_spi_mngr_transfer_t const transfers[] =
        {
            NRF_SPI_MNGR_TRANSFER(m_initialization, sizeof(m_initialization), NULL, 0)
        };

        nrf_gpio_pin_clear(SSD1306_PIN_DC); // Enter CMD mode
        err_code = nrf_spi_mngr_perform(&m_nrf_spi_mngr, transfers, ARRAY_SIZE(transfers), NULL);
    }

    return err_code;
}


void ssd1306_uninit(void)
{
    nrf_spi_mngr_uninit(&m_nrf_spi_mngr);
}


void ssd1306_pixel_draw(uint16_t x, uint16_t y, uint32_t color)
{
    ASSERT(x < SCREEN_WIDTH);
    ASSERT(y < SCREEN_HEIGHT);

    uint8_t page;
    uint8_t y_disp;
    uint8_t * p_byte;

    page = (uint8_t)(y >> 3);
    y_disp = y % 8;
    p_byte = &m_display_buffer[(SCREEN_WIDTH - 1) * page + x + page];

    // A monochromatic OLED is used. Color > 0 sets pixel and color == 0 clears it.
    if (color)
    {
        SET_BIT(*p_byte, y_disp);
    }
    else
    {
        CLR_BIT(*p_byte, y_disp);
    }
}


void ssd1306_rect_draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    if ((x + width) >= SCREEN_WIDTH)
    {
        return;
    }
    if ((y + height) >= SCREEN_HEIGHT)
    {
        return;
    }

    for (uint8_t idx_y = y; idx_y < (y + height); idx_y++)
    {
        for (uint8_t idx_x = x; idx_x < (x + width); idx_x++)
        {
            ssd1306_pixel_draw(idx_x, idx_y, color);
        }
    }
}


void ssd1306_display_screen(void)
{
    ret_code_t ret_val;

    // Below structures have to be "static" - they cannot be placed on stack
    // since the transaction is scheduled and these structures most likely
    // will be referred after this function returns
    static nrf_spi_mngr_transfer_t NRF_SPI_MNGR_BUFFER_LOC_IND transfer_cmd[] =
    {
        NRF_SPI_MNGR_TRANSFER(m_command_buffer, sizeof(m_command_buffer), NULL, 0),
    };
    static nrf_spi_mngr_transaction_t const transaction_cmd[] =
    {
        {
            .begin_callback      = ssd1306_set_command_dc,
            .end_callback        = NULL,
            .p_user_data         = NULL,
            .p_transfers         = transfer_cmd,
            .number_of_transfers = 1,
            .p_required_spi_cfg  = NULL
        }
    };
    static nrf_spi_mngr_transfer_t NRF_SPI_MNGR_BUFFER_LOC_IND transfer_data[] =
    {
        /* WIDTH*PAGES = 1024 for our 128x64 screen.
         * SPI Only allows sending 256 bytes at a time so we have to split it up */
        NRF_SPI_MNGR_TRANSFER(&m_display_buffer[   0], 255, NULL, 0),
        NRF_SPI_MNGR_TRANSFER(&m_display_buffer[ 255], 255, NULL, 0),
        NRF_SPI_MNGR_TRANSFER(&m_display_buffer[ 510], 255, NULL, 0),
        NRF_SPI_MNGR_TRANSFER(&m_display_buffer[ 765], 255, NULL, 0),
        NRF_SPI_MNGR_TRANSFER(&m_display_buffer[1020],   4, NULL, 0),
    };
    static nrf_spi_mngr_transaction_t const transaction_data[] =
    {
        {
            .begin_callback      = ssd1306_set_data_dc,
            .end_callback        = NULL,
            .p_user_data         = NULL,
            .p_transfers         = &transfer_data[0],
            .number_of_transfers = 1,
            .p_required_spi_cfg  = NULL
        },
        {
            .begin_callback      = ssd1306_set_data_dc,
            .end_callback        = NULL,
            .p_user_data         = NULL,
            .p_transfers         = &transfer_data[1],
            .number_of_transfers = 1,
            .p_required_spi_cfg  = NULL
        },
        {
            .begin_callback      = ssd1306_set_data_dc,
            .end_callback        = NULL,
            .p_user_data         = NULL,
            .p_transfers         = &transfer_data[2],
            .number_of_transfers = 1,
            .p_required_spi_cfg  = NULL
        },
        {
            .begin_callback      = ssd1306_set_data_dc,
            .end_callback        = NULL,
            .p_user_data         = NULL,
            .p_transfers         = &transfer_data[3],
            .number_of_transfers = 1,
            .p_required_spi_cfg  = NULL
        },
        {
            .begin_callback      = ssd1306_set_data_dc,
            .end_callback        = NULL,
            .p_user_data         = NULL,
            .p_transfers         = &transfer_data[4],
            .number_of_transfers = 1,
            .p_required_spi_cfg  = NULL
        }
    };
    ret_val = nrf_spi_mngr_schedule(&m_nrf_spi_mngr, transaction_cmd);
    if (ret_val == NRF_SUCCESS)
    {
        APP_ERROR_CHECK(nrf_spi_mngr_schedule(&m_nrf_spi_mngr, &transaction_data[0]));
        APP_ERROR_CHECK(nrf_spi_mngr_schedule(&m_nrf_spi_mngr, &transaction_data[1]));
        APP_ERROR_CHECK(nrf_spi_mngr_schedule(&m_nrf_spi_mngr, &transaction_data[2]));
        APP_ERROR_CHECK(nrf_spi_mngr_schedule(&m_nrf_spi_mngr, &transaction_data[3]));
        APP_ERROR_CHECK(nrf_spi_mngr_schedule(&m_nrf_spi_mngr, &transaction_data[4]));
    }
}


void ssd1306_display_invert(bool invert)
{
    static uint8_t NRF_SPI_MNGR_BUFFER_LOC_IND command[] = {
        SSD1306_DISPLAY_NORMAL, SSD1306_DISPLAY_INVERSE
    };

    static nrf_spi_mngr_transfer_t const transfer_cmd[] =
    {
        NRF_SPI_MNGR_TRANSFER(&command[0], 1, NULL, 0),
        NRF_SPI_MNGR_TRANSFER(&command[1], 1, NULL, 0),
    };
    static nrf_spi_mngr_transaction_t const transaction_cmd[] =
    {
        {
            .begin_callback      = ssd1306_set_command_dc,
            .end_callback        = NULL,
            .p_user_data         = NULL,
            .p_transfers         = &transfer_cmd[0],
            .number_of_transfers = 1,
            .p_required_spi_cfg  = NULL
        },
        {
            .begin_callback      = ssd1306_set_command_dc,
            .end_callback        = NULL,
            .p_user_data         = NULL,
            .p_transfers         = &transfer_cmd[1],
            .number_of_transfers = 1,
            .p_required_spi_cfg  = NULL
        },
    };
    APP_ERROR_CHECK(nrf_spi_mngr_schedule(&m_nrf_spi_mngr, &transaction_cmd[invert]));
}


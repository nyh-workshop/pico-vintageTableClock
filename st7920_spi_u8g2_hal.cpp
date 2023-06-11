#include "st7920_spi_u8g2_hal.h"

void st7920_writeReg_SPI(uint8_t aByte)
{
    uint8_t cmdBuf[3];
    cmdBuf[0] = 0b11111000;
    cmdBuf[1] = aByte & 0xf0;
    cmdBuf[2] = (aByte & 0x0f) << 0x04;
    spi_write_blocking(SPI_PORT, cmdBuf, sizeof(cmdBuf));
}

void st7920_writeData_SPI(uint8_t aByte)
{
    uint8_t cmdBuf[3];
    cmdBuf[0] = 0b11111010;
    cmdBuf[1] = aByte & 0xf0;
    cmdBuf[2] = (aByte & 0x0f) << 0x04;
    spi_write_blocking(SPI_PORT, cmdBuf, sizeof(cmdBuf));
}

uint8_t u8x8_gpio_and_delay_pico(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        gpio_init(PIN_RST);
        gpio_init(PIN_CS);
        
        gpio_set_dir(PIN_RST, GPIO_OUT);
        gpio_set_dir(PIN_CS, GPIO_OUT);

        gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
        gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
        spi_init(SPI_PORT, SPI_SPEED);

        gpio_put(PIN_RST, 0);
        sleep_ms(100);
        gpio_put(PIN_RST, 1);

        gpio_put(PIN_CS, 1);

        break;
    case U8X8_MSG_DELAY_NANO: // delay arg_int * 1 nano second
        sleep_us(arg_int);    // 1000 times slower, though generally fine in practice given rp2040 has no `sleep_ns()`
        break;
    case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
        sleep_us(arg_int);
        break;
    case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
        sleep_us(arg_int * 10);
        break;
    case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
        sleep_ms(arg_int);
        break;
    case U8X8_MSG_GPIO_CS: // CS (chip select) pin: Output level in arg_int
        gpio_put(PIN_CS, arg_int);
        break;
    case U8X8_MSG_GPIO_DC: // DC (data/cmd, A0, register select) pin: Output level
        break;
    case U8X8_MSG_GPIO_RESET:       // Reset pin: Output level in arg_int
        gpio_put(PIN_RST, arg_int); // printf("U8X8_MSG_GPIO_RESET %d\n", arg_int);
        break;
    default:
        u8x8_SetGPIOResult(u8x8, 1); // default return value
        break;
    }
    return 1;
}

uint8_t u8x8_byte_pico_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    uint8_t *data;
    switch (msg)
    {
    case U8X8_MSG_BYTE_SEND:
        spi_write_blocking(SPI_PORT, (uint8_t*)arg_ptr, arg_int);
        break;
    case U8X8_MSG_BYTE_INIT:
        break;
    case U8X8_MSG_BYTE_SET_DC:
        break;
    case U8X8_MSG_BYTE_START_TRANSFER:
        gpio_put(PIN_CS, 1);
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        gpio_put(PIN_CS, 0);
        break;
    default:
        return 0;
    }
    return 1;
}

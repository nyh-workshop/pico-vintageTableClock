#ifndef ST7920_SPI_U8G2_HAL_H
#define ST7920_SPI_U8G2_HAL_H

// Reference: https://github.com/olikraus/u8g2/issues/2159

#include <u8g2.h>
#include <u8g2lib.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

extern "C"
{
#include "pico/util/datetime.h"
#include "hardware/rtc.h"
}

// ADD THE INIT DISPLAY 
#define SPI_PORT spi0
#define PIN_CS 5 // RS
#define PIN_SCK 2 // E 
#define PIN_MOSI 3 // RW
#define SPI_SPEED 800 * 1000
#define PIN_RST 13

void st7920_writeReg_SPI(uint8_t aByte);
void st7920_writeData_SPI(uint8_t aByte);
uint8_t u8x8_gpio_and_delay_pico(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_byte_pico_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

class U8G2_ST7920_128X64_1_HW_SPI_CUSTOM : public U8G2 {
  public: U8G2_ST7920_128X64_1_HW_SPI_CUSTOM(const u8g2_cb_t *rotation) : U8G2() {
    u8g2_Setup_st7920_s_128x64_f(&u8g2, rotation, u8x8_byte_pico_hw_spi, u8x8_gpio_and_delay_pico);
    //u8x8_SetPin_ST7920_HW_SPI(getU8x8(), cs, reset);
  }
};
#endif
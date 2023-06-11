#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/pio.h"

#include "st7920_spi_u8g2_hal.h"
#include "digitalClockDisplay.h"
#include "helperRTC/helperRTC.h"
#include "RotaryEncoder/rotaryEncoder.h"

constexpr uint8_t PLAY_CHIME = 0x80;
constexpr uint8_t PLAY_MELODY = 0x00;
constexpr uint8_t PLAY_WESTMINSTER = 0x01;

#endif
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
//#include <tusb.h>
#include "pico/stdlib.h"
#include "pico/float.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"

#include <dht.h>
#include "st7920_spi_u8g2_hal.h"
#include "digitalClockDisplay.h"
#include "helperRTC/helperRTC.h"
#include "RotaryEncoder/rotaryEncoder.h"
#include "ds3231/ds3231.h"
#include "MakerPicoBuzzer/MakerPicoBuzzer.h"

//#define MAKER_NANO_RP2040

const dht_model_t DHT_MODEL = DHT11;

#ifdef MAKER_NANO_RP2040
const uint8_t DHT_DATA_PIN = 7;
#else
const uint8_t DHT_DATA_PIN = 11;
#endif

const uint8_t PICO_I2C_SDA_PIN = 8;
const uint8_t PICO_I2C_SCL_PIN = 9;

const uint8_t SET_PIN = 14;
const uint8_t RENCODER_CLK_PIN = 15;
const uint8_t RENCODER_DT_PIN = 16;

#ifdef MAKER_NANO_RP2040
const uint8_t PICO_PWM_BUZZER_PIN = 22;
#else
const uint8_t PICO_PWM_BUZZER_PIN = 18;
#endif

constexpr uint8_t PLAY_CHIME = 0x80;
constexpr uint8_t PLAY_MELODY = 0x00;
constexpr uint8_t PLAY_WESTMINSTER = 0x01;

#endif
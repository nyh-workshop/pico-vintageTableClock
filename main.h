#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/float.h"
#include "hardware/spi.h"
#include "hardware/pio.h"

#include <dht.h>
#include "st7920_spi_u8g2_hal.h"
#include "digitalClockDisplay.h"
#include "helperRTC/helperRTC.h"
#include "RotaryEncoder/rotaryEncoder.h"

const dht_model_t DHT_MODEL = DHT11;
const uint8_t DHT_DATA_PIN = 11;

constexpr uint8_t PLAY_CHIME = 0x80;
constexpr uint8_t PLAY_MELODY = 0x00;
constexpr uint8_t PLAY_WESTMINSTER = 0x01;

#endif
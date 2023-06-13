#ifndef DS3231_H
#define DS3231_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

const uint8_t DS3231_EOSC = 1 << 7;
const uint8_t DS3231_BBSQW = 1 << 6;
const uint8_t DS3231_CONV = 1 << 5;
const uint8_t DS3231_RS2 = 1 << 4;
const uint8_t DS3231_RS1 = 1 << 3;
const uint8_t DS3231_INTCN = 1 << 2;
const uint8_t DS3231_A2IE = 1 << 1;
const uint8_t DS3231_A1IE = 1 << 0;

const uint8_t DS3231_OSF = 1 << 7;
const uint8_t DS3231_EN32KHZ = 1 << 3;
const uint8_t DS3231_BSY = 1 << 2;
const uint8_t DS3231_A2F = 1 << 1;
const uint8_t DS3231_A1F = 1 << 0;

const uint8_t DS3231_CONTROL = 0x0E;
const uint8_t DS3231_STATUS = 0x0F;

const uint8_t DS3231_I2C_ADDR = 0x68;
const uint32_t I2C_TIMEOUT_US = 250000;

// Basic DS3231 driver for Raspberry Pico:
// Reference for bcdToDec and decToBcd: https://github.com/raburton/esp8266/blob/master/drivers/ds3231.c
class PicoDS3231 {
    public:
        PicoDS3231(uint8_t SDA, uint8_t SDL, uint32_t clkFreq);
        int32_t readFromAddr(uint8_t aAddr, uint8_t* aData);
        int32_t writeToAddr(uint8_t aAddr, uint8_t aData);
        inline uint8_t bcdToDec(uint8_t bcd) { return(((bcd / 16) * 10) + (bcd % 16)); }
        inline uint8_t decToBcd(uint8_t dec) { return(((dec / 10) * 16) + (dec % 10)); }
        void saveToDS3231(datetime_t* aDateTime);
        void retrieveFromDS3231(datetime_t* aDateTime);

    private:

};



#endif
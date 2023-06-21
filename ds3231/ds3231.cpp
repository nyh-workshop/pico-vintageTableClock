#include "ds3231.h"

PicoDS3231::PicoDS3231(uint8_t SDA, uint8_t SCL, uint32_t clkFreq)
{
    // Starting up DS3231 first:
    // I2C0 default.    
    gpio_init(SDA);
    gpio_init(SCL);
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SDA);
    gpio_pull_up(SCL);
    i2c_init(i2c0, clkFreq);

    //writeToAddr(DS3231::CONTROL_ADDR, DS3231::ctrl::INTCN);
}

int32_t PicoDS3231::readFromAddr(uint8_t aAddr, uint8_t* aData)
{
    int32_t result = i2c_write_timeout_us(i2c0, DS3231_I2C_ADDR, &aAddr, 1, true, I2C_TIMEOUT_US);

    result = i2c_read_timeout_us(i2c0, DS3231_I2C_ADDR, aData, 1, false, I2C_TIMEOUT_US);

    return result;
}

int32_t PicoDS3231::writeToAddr(uint8_t aAddr, uint8_t aData)
{
    uint8_t buf[2];

    buf[0] = aAddr;
    buf[1] = aData;

    int32_t result = i2c_write_timeout_us(i2c0, DS3231_I2C_ADDR, buf, 2, false, I2C_TIMEOUT_US);

    return result;
}

void PicoDS3231::retrieveFromDS3231(datetime_t* aDateTime)
{
    uint8_t buf[7];
    uint8_t val = 0x00;
    
    i2c_write_timeout_us(i2c_default, DS3231_I2C_ADDR, &val, 1, true, I2C_TIMEOUT_US ); // true to keep master control of bus
    i2c_read_timeout_us(i2c_default, DS3231_I2C_ADDR, buf, 7, false, I2C_TIMEOUT_US);

    aDateTime->sec = bcdToDec(buf[0]);
    aDateTime->min = bcdToDec(buf[1]);
    aDateTime->hour = bcdToDec(buf[2]);
    aDateTime->dotw = bcdToDec(buf[3]) - 1;
    aDateTime->day = bcdToDec(buf[4]);
    aDateTime->month = bcdToDec(buf[5]);
    aDateTime->year = bcdToDec(buf[6]) + 2000;    
}

void PicoDS3231::saveToDS3231(datetime_t* aDateTime)
{
    // DS3231's dotw is 1-7!
    writeToAddr(0x00, decToBcd(aDateTime->sec));
    writeToAddr(0x01, decToBcd(aDateTime->min));
    writeToAddr(0x02, decToBcd(aDateTime->hour));
    writeToAddr(0x03, decToBcd(aDateTime->dotw) + 1);
    writeToAddr(0x04, decToBcd(aDateTime->day));
    writeToAddr(0x05, decToBcd(aDateTime->month));
    writeToAddr(0x06, decToBcd(aDateTime->year - 2000));
}
#ifndef DIGITALCLOCKDISPLAY_H
#define DIGITALCLOCKDISPLAY_H

#include "st7920_spi_u8g2_hal.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/float.h"

#include "main.h"

extern U8G2_ST7920_128X64_1_HW_SPI_CUSTOM u8g2;

enum whichDigitToBlink
{
  blinkColon = 0,
  blinkHour,
  blinkMinute,
  blinkDay,
  blinkMonth,
  blinkYear,
  blinkDOTW,
  blinkChimeSelect,
  blinkNone,
};

enum digitalClockEvent
{
    None = 0,
    setButton = 1
};

class digitalClockDisplay
{
public:
    inline void resetSetTimeCount() { setTimeCountSeconds = 0; }
    inline void resetBlinkCount();
    inline whichDigitToBlink getBlinkSelect() { return blinkSelect; }
    inline void setBlinkSelect(whichDigitToBlink aBlinkSelect) { blinkSelect = aBlinkSelect; }
    inline void incrementSetTimeCountSecs() { setTimeCountSeconds++; }
    inline uint8_t getSetTimeCountSecs() { return setTimeCountSeconds; }
    void drawClockDisplay(datetime_t *aDateTime, whichDigitToBlink aBlinkSelect);
    inline digitalClockEvent getEvent() { return event; }
    inline void setEvent(digitalClockEvent aEvent) { event = aEvent; }
    inline void setChimeSettings(uint8_t aChimeSettings) { chimeSettings = aChimeSettings; }
    uint8_t getChimeSettings() { return chimeSettings; }

private:
    uint8_t setButtonEvent = 0;
    uint8_t setTimeCountSeconds = 0;
    whichDigitToBlink blinkSelect;
    digitalClockEvent event = None;
    uint8_t chimeSettings = 0;
};

inline void digitalClockDisplay::resetBlinkCount()
{
    //setButtonEvent = 0;
    event = None;
    setTimeCountSeconds = 0;
    blinkSelect = blinkColon;
}

#endif
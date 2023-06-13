#ifndef HELPERRTC_H
#define HELPERRTC_H

#include "../main.h"

extern class digitalClockDisplay clkDisp;

void rtc_adjustForDay(datetime_t& t);
void setTimeUpdate(int8_t PlusOrMinus);

// This adjusts the hourly chime from 6am - 9pm only!
inline void adjustHourlyChimeForDay(datetime_t* timeOfDay, datetime_t* alarmTime)
{
    if (timeOfDay->hour >= 6 && timeOfDay->hour <= 21)
    {
        alarmTime->hour = timeOfDay->hour + 1;
    }

    if (timeOfDay->hour < 6 || timeOfDay->hour > 21)
    {
        alarmTime->hour = 6;
    }       
}

#endif
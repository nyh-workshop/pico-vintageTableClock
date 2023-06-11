#include "helperRTC.h"

void rtc_adjustForDay(datetime_t& t)
{
  if ((t.month == 1) || (t.month == 3) || (t.month == 5) || (t.month == 7) ||
      (t.month == 8) || (t.month == 10) || (t.month == 12))
  {
    if (t.day > 31)
      t.day = 1;
    if (t.day < 1)
      t.day = 31;  
  }  
  else if ((t.month == 2) && !(t.year % 4))
  {
    if (t.day > 29)
      t.day = 1;
    if (t.day < 1)
      t.day = 29;  
  }
  else if ((t.month == 2) && (t.year % 4))
  {
    if (t.day > 28)
      t.day = 1;
    if (t.day < 1)
      t.day = 28;  
  }
  else
  {
    if (t.day > 30)
      t.day = 1;
    if (t.day < 1)
      t.day = 30;  
  }
}

void setTimeUpdate(int8_t PlusOrMinus)
{
  datetime_t t;
  datetime_t a = {
        .year  = -1,
        .month = -1,
        .day   = -1,
        .dotw  = -1,
        .hour  = 6,
        .min   = 0,
        .sec   = 0
  };

  rtc_get_datetime(&t);

  if (PlusOrMinus == 1)
  {
    switch (clkDisp.getBlinkSelect())
    {
    case blinkHour:
      t.hour = t.hour + 1;
      if (t.hour > 23)
          t.hour = 0;
      break;
    case blinkMinute:
      t.min = t.min + 1;
      if (t.min > 59)
          t.min = 0;
      break;
    case blinkDay:
      t.day = t.day + 1;
      rtc_adjustForDay(t);
      break;
    case blinkMonth:
      t.month = t.month + 1;
      if (t.month > 12)
          t.month = 1;
      rtc_adjustForDay(t);
      break;
    case blinkYear:
      t.year = t.year + 1;
      if (t.year > 2050)
          t.year = 2020;
      rtc_adjustForDay(t);
      break;
    case blinkDOTW:
      t.dotw = t.dotw + 1;
      if (t.dotw > 6)
          t.dotw = 0;
      break;
    case blinkChimeSelect:
      clkDisp.setChimeSettings(PLAY_WESTMINSTER);
      break;
    default:
      break;
    }
    adjustHourlyChimeForDay(&t, &a);
    rtc_set_datetime(&t);
  }
  else if (PlusOrMinus == -1)
  {
    switch (clkDisp.getBlinkSelect())
    {
    case blinkHour:
      t.hour = t.hour - 1;
      if (t.hour < 0)
          t.hour = 23;
      break;
    case blinkMinute:
      t.min = t.min - 1;
      if (t.min < 0)
          t.min = 59;
      break;
    case blinkDay:
      t.day = t.day - 1;
      rtc_adjustForDay(t);
      break;
    case blinkMonth:
      t.month = t.month - 1;
      if (t.month < 1)
          t.month = 12;
      rtc_adjustForDay(t);
      break;
    case blinkYear:
      t.year = t.year - 1;
      if (t.year < 2020)
          t.year = 2050;
      rtc_adjustForDay(t);
      break;
    case blinkDOTW:
      t.dotw = t.dotw - 1;
      if (t.dotw < 0)
          t.dotw = 6;
      break;
    case blinkChimeSelect:
      clkDisp.setChimeSettings(PLAY_MELODY);
      break;
    default:
      break;
    }
    adjustHourlyChimeForDay(&t, &a);
    rtc_set_datetime(&t);
  }
  else
    return;
}
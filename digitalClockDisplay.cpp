#include "digitalClockDisplay.h"

const char *dayOfWeek3letter[8] = {
    "MON",
    "TUE",
    "WED",
    "THU",
    "FRI",
    "SAT",
    "SUN"
};

const char *months3letter[13] = {
    "   ",
    "JAN",
    "FEB",
    "MAR",
    "APR",
    "MAY",
    "JUN",
    "JUL",
    "AUG",
    "SEP",
    "OCT",
    "NOV",
    "DEC"
};

void digitalClockDisplay::drawClockDisplay(datetime_t* aDateTime, whichDigitToBlink aBlinkSelect)
{
    char time_str[16];
    char date_str[16];
    char day_str[4];

    rtc_get_datetime(aDateTime);

    uint8_t hour12 = (aDateTime->hour > 12) ? abs(aDateTime->hour - 12) : aDateTime->hour;

    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_waffle_t_all);
    u8g2.drawGlyph(0, 10, getChimeSettings() == PLAY_MELODY ? 0xe272 : 0xe277);
    u8g2.setFont(u8g2_font_logisoso34_tn);
    snprintf(time_str, sizeof(time_str), "%02d:%02d", aDateTime->hour, aDateTime->min);
    u8g2.drawStr(14, 48, time_str);
    u8g2.setFont(u8g2_font_profont11_tf);
    snprintf(date_str, sizeof(date_str), "%02d-%s-20%02d",
           aDateTime->day, months3letter[aDateTime->month], aDateTime->year - 2000);
    snprintf(day_str, sizeof(date_str), "%s", dayOfWeek3letter[aDateTime->dotw]);
    u8g2.drawStr(15, 60, day_str);
    u8g2.drawStr(52, 60, date_str);

    u8g2.setDrawColor(0);
    switch (aBlinkSelect)
    {
    case blinkHour:
        u8g2.drawBox(14, 12, 43, 40);
        break;

    case blinkMinute:
        u8g2.drawBox(72, 12, 43, 40);
        break;

    case blinkDay:
        u8g2.drawBox(52, 52, 12, 10);
        break;

    case blinkMonth:
        u8g2.drawBox(70, 52, 18, 10);
        break;

    case blinkYear:
        u8g2.drawBox(93, 52, 26, 10);
        break;

    case blinkDOTW:
        u8g2.drawBox(15, 52, 20, 10);
        break;

    case blinkColon:
        u8g2.drawBox(58, 20, 10, 25);
        break;

    case blinkChimeSelect:
        u8g2.drawBox(0, 0, 12, 12);
        break;

    default:
        break;
    }
    u8g2.updateDisplay();
}
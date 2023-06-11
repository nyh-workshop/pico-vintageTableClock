# Raspberry Pico Vintage Style Table Clock

## For the submission of Cytron's RPi Pico Maker Party 2023

This is currently a code draft of the vintage style table clock.

Cytron's Maker Pi Pico is used with the Pico Debug Probe for the development of this code.

Currently, the features are:
- The sound module is called [Simple FM synth](https://github.com/nyh-workshop/pico-fmSynth) and runs on Core 1. The clock chimes every hour from 6am to 9pm. Hourly chime can be selected as Westminster or a small excerpt of Pachelbel's Canon in D.
- PCM5102 and MAX98357 can be used for the sound module.
- The screen is a generic ST7920 and it is driven by the [U8G2](https://github.com/olikraus/u8g2) library and connected to the Pico using SPI. You can check the submodule [here](https://github.com/nyh-workshop/pico-u8g2-st7920).
- Rotary Encoder to set the time.
- RP2040's internal RTC module to keep and tell the time.

Limitations:
- Currently no way to stop melody when it is currently playing.
- No battery backup for the internal RTC module.

Schematics coming soon! :D
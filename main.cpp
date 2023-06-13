#include "main.h"

const uint8_t SET_PIN = 14;
const uint8_t RENCODER_CLK_PIN = 15;
const uint8_t RENCODER_DT_PIN = 16;

U8G2_ST7920_128X64_1_HW_SPI_CUSTOM u8g2(U8G2_R0);
dht_t dht;
digitalClockDisplay clkDisp;

struct debounceInterruptTime {
    uint32_t current = 0;
    uint32_t last = 0;
};

volatile debounceInterruptTime di_SetPin;
volatile debounceInterruptTime di_REncoderClkPin;

#include "miditonesFile/sampleMidi.h"

#include "driver/fmSynth_picoI2sAudioDriver.h"

volatile uint8_t blinkSelect = None;
void gpio_callback_gpio(uint gpio, uint32_t events)
{
    if (gpio == SET_PIN)
    {
        // Debouncing code from https://forum.arduino.cc/index.php?topic=45000.0:
        di_SetPin.current = time_us_32();

        if ((di_SetPin.current - di_SetPin.last) > 250000)
        {
            clkDisp.setEvent(setButton);
            (blinkSelect > blinkDOTW) ? blinkSelect = blinkNone : blinkSelect++;

            if (blinkSelect == blinkNone)
            {
                clkDisp.setEvent(None);
                clkDisp.setBlinkSelect(blinkColon);
                clkDisp.resetSetTimeCount();
                blinkSelect = None;
            }
            else
            {
                clkDisp.setBlinkSelect((whichDigitToBlink)blinkSelect);
                clkDisp.resetSetTimeCount();
            }
        }
        di_SetPin.last = di_SetPin.current;
    }
    else if (gpio == RENCODER_CLK_PIN)
    {
        di_REncoderClkPin.current = time_us_32();

        if ((di_REncoderClkPin.current - di_REncoderClkPin.last) > 125000)
        {
            if (gpio_get(RENCODER_DT_PIN))
            {
                setTimeUpdate(CW);
                clkDisp.resetSetTimeCount();
            }
            di_REncoderClkPin.last = di_REncoderClkPin.current;
        }
    }
    else if (gpio == RENCODER_DT_PIN)
    {
        di_REncoderClkPin.current = time_us_32();

        if ((di_REncoderClkPin.current - di_REncoderClkPin.last) > 125000)
        {
            if (gpio_get(RENCODER_CLK_PIN))
            {
                setTimeUpdate(CCW);
                clkDisp.resetSetTimeCount();
            }
            di_REncoderClkPin.last = di_REncoderClkPin.current;
        }
    }
}

//-------------------------------------------------------
//  Core 1
//-------------------------------------------------------
void configureInterpLanes() {
	interp_config interpCfg = interp_default_config();

	// Lane 0 settings:
    interp_config_set_add_raw(&interpCfg, true);
    interp_config_set_cross_input(&interpCfg, 0);
    interp_config_set_mask(&interpCfg, 22, 31);
    interp_config_set_shift(&interpCfg, 0);
    interp_config_set_signed(&interpCfg, false);
    interp_set_config(interp1, 0, &interpCfg);
    // Lane 1 settings:
    interp_config_set_add_raw(&interpCfg, true);
    interp_config_set_cross_input(&interpCfg, 0);
    interp_config_set_mask(&interpCfg, 0, 9);
    interp_config_set_shift(&interpCfg, 22);
    interp_config_set_signed(&interpCfg, false);
    interp_set_config(interp1, 1, &interpCfg);
}

static mutex_t mPlayer_M;
static semaphore_t c1_semaphore;

volatile uint32_t durationMs = 0;
bool repeating_timer_callback_atCore1(struct repeating_timer *t) {
    if(durationMs >= ((fmSynthPicoI2s*)t->user_data)->getNoteDurationMs())
    {
        durationMs = 0;
        ((fmSynthPicoI2s*)t->user_data)->stepScore();
    }
    else
        durationMs++;
    return true;
}

volatile uint32_t numOfBellStrikes = 0;
volatile bool isPlayingBellStrikes = false;
bool repeating_timer_callback_atCore1_bellStrike(struct repeating_timer *t) {
    datetime_t tm;
    rtc_get_datetime(&tm);
    uint8_t hour12 = (tm.hour > 12) ? tm.hour - 12 : tm.hour;
    
    if( numOfBellStrikes++ <= hour12 - 1)
        ((fmSynthPicoI2s*)t->user_data)->noteOn(0, 64);
    else {
        isPlayingBellStrikes = false;
        numOfBellStrikes = 0;
    }    
    return true;
}

void core1_entry()
{
    printf("Entering core1! :D\n");
    
    // RP2040's interpolator module needs to be configured at the respective core before running.
    configureInterpLanes();

    // this fmSynth library using branch interp_OneCore hash 3a03b71
    // modified for this app.
    fmSynthPicoI2s tunePlayer;

    while (1)
    {
        // Wait for commands from Core0:
        uint8_t res = multicore_fifo_pop_blocking();

        if ((res & PLAY_CHIME) == PLAY_CHIME)
        {
            uint8_t playMusic = res & 0x0F;

            switch (playMusic)
            {
            case PLAY_MELODY:
                tunePlayer.selectInstrumentAllChan("E.PIANO");
                tunePlayer.playScore(canon);
                break;
            case PLAY_WESTMINSTER:
                tunePlayer.selectInstrumentAllChan("TBULARBELS");
                tunePlayer.playScore(westminsterChime);
                break;
            default:
                tunePlayer.selectInstrumentAllChan("TBULARBELS");
                tunePlayer.playScore(westminsterChime);
                break;
            }

            struct repeating_timer timerForPlayChime;            
            add_repeating_timer_ms(-1, repeating_timer_callback_atCore1, &tunePlayer, &timerForPlayChime);

            while(tunePlayer.isPlaying())
            {
                tunePlayer.playSamples();
            }
            cancel_repeating_timer(&timerForPlayChime);
            tunePlayer.stopScore();

            // Then strike the bell: 
            isPlayingBellStrikes = true;

            add_repeating_timer_ms(-2000, repeating_timer_callback_atCore1_bellStrike, &tunePlayer, &timerForPlayChime);

            tunePlayer.selectInstrumentAllChan("TBULARBELS");
            
            while(isPlayingBellStrikes)
            {
                tunePlayer.playSamples();
            }
            tunePlayer.noteOff(0);

            cancel_repeating_timer(&timerForPlayChime);
        }
    }
}
//-------------------------------------------------------

//-------------------------------------------------------
//  Core 0
//-------------------------------------------------------
int main()
{
    stdio_init_all();

    stdio_filter_driver(&stdio_usb);
    while (!tud_cdc_connected())
        sleep_ms(100);
    printf("\nUSB Serial connected!\n");

    // Example from the Pico SDK manual!
    datetime_t t = {
        .year = 2023,
        .month = 01,
        .day = 01,
        .dotw = 6,
        .hour = 12,
        .min = 00,
        .sec = 00
    };

    PicoDS3231 rtcExt(PICO_I2C_SDA_PIN, PICO_I2C_SCL_PIN, 100000);

    // Get OSF of DS3231:
    uint8_t DS3231_sreg;    
    int32_t DS3231_readResult = rtcExt.readFromAddr(DS3231_STATUS, &DS3231_sreg);
    
    if(DS3231_readResult == PICO_ERROR_TIMEOUT || DS3231_readResult == PICO_ERROR_GENERIC)
    {
        printf("DS3231 not detected or error! Check connection!\n");
    }
    else 
    {
        if ((DS3231_sreg & DS3231_OSF) == DS3231_OSF)
        {
            printf("DS3231 not running previously!\n");
            printf("Setting time to default...\n");
            rtcExt.writeToAddr(DS3231_STATUS, 0x00);
            rtcExt.saveToDS3231(&t);
         }
        else
        {
            printf("DS3231 is running!\n");
            rtcExt.retrieveFromDS3231(&t);
        }
    }
   
    gpio_init(SET_PIN);
    gpio_set_dir(SET_PIN, GPIO_IN);
    gpio_pull_up(SET_PIN);

    gpio_init(18);
    gpio_set_dir(18, GPIO_OUT);

    gpio_set_irq_enabled(RENCODER_CLK_PIN, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(RENCODER_DT_PIN, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled_with_callback(SET_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback_gpio);
    
    rtc_init();
    rtc_set_datetime(&t);

    multicore_launch_core1(core1_entry);

    // pio0 is occupied (audio i2s module).
    dht_init(&dht, DHT_MODEL, pio1, DHT_DATA_PIN, true /* pull_up */);

    u8g2.begin();
    u8g2.setBitmapMode(0);

    while (1)
    {
        if (clkDisp.getEvent() == setButton)
        {
            while(clkDisp.getSetTimeCountSecs() <= 3)
            {
                // Switch on the interrupts for the rotary encoder for this period!
                clkDisp.drawClockDisplay(&t, blinkNone);
                sleep_ms(500);
                clkDisp.drawClockDisplay(&t, clkDisp.getBlinkSelect());
                sleep_ms(500);
                clkDisp.incrementSetTimeCountSecs();
            }
            clkDisp.resetBlinkCount();
            blinkSelect = None;
            rtcExt.saveToDS3231(&t);
        }
        else
        {
            clkDisp.drawClockDisplay(&t, blinkNone);
            sleep_ms(500);
            clkDisp.drawClockDisplay(&t, blinkColon);            
            sleep_ms(500);

            if((t.hour >= 6 && t.hour <= 21) && t.sec == 00 && t.min == 00)
            {
                multicore_fifo_push_blocking(PLAY_CHIME | clkDisp.getChimeSettings());
            }
        }
    }

    return 0;
}
//-------------------------------------------------------
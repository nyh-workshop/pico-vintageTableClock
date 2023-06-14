#include "MakerPicoBuzzer.h"

void initBuzzer()
{
    // Start PWM module for the onboard buzzer!
    // reference: https://cocode.se/linux/raspberry/pwm.html
    // gpio_init(PICO_PWM_BUZZER_PIN);
    gpio_set_function(PICO_PWM_BUZZER_PIN, GPIO_FUNC_PWM);
    uint32_t slice_num = pwm_gpio_to_slice_num(PICO_PWM_BUZZER_PIN);
    pwm_set_clkdiv(slice_num, 125.0f);
    pwm_set_wrap(slice_num, 1111);
    pwm_set_gpio_level(PICO_PWM_BUZZER_PIN, 556);
    pwm_set_enabled(slice_num, false);
}

void enableBuzzer()
{
    uint32_t slice_num = pwm_gpio_to_slice_num(PICO_PWM_BUZZER_PIN);
    pwm_set_enabled(slice_num, true);
}

void disableBuzzer()
{
    uint32_t slice_num = pwm_gpio_to_slice_num(PICO_PWM_BUZZER_PIN);
    pwm_set_enabled(slice_num, false);
}

int64_t buzzer_callback(alarm_id_t id, void *user_data) {
    disableBuzzer();
    return 0;
}

void beep()
{
    enableBuzzer();
    add_alarm_in_ms(64, buzzer_callback, NULL, false);
}
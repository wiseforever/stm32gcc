#include "led.h"
#include "delay.h"


void led_test(void)
{
    led_on();
    delay_ms(300);

    led_off();
    delay_ms(300);
}
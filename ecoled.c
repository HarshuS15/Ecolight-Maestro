#include <LPC21xx.h>

#define LED (1<<10)     // P0.10

void led_init(void)
{
    IODIR0 |= LED;      // P0.10 output
    IOCLR0 = LED;
}

void led_on(void)
{
    IOSET0 = LED;       // LED ON
}

void led_off(void)
{
    IOCLR0 = LED;       // LED OFF
}
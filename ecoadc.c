#include <LPC21xx.h>

void adc_init(void)
{
    // SELECT AD0.1 ON P0.28

    PINSEL1 &= ~(3<<24);

    PINSEL1 |= (1<<24);

    // ADC CONFIGURATION

    ADCR = (1<<1) |      // SELECT CHANNEL 1
           (15<<8) |     // CLKDIV
           (1<<21);      // ADC ENABLE
}

unsigned int adc_read(void)
{
    unsigned int val;

    // START CONVERSION

    ADCR |= (1<<24);

    // WAIT FOR DONE BIT

    while((ADDR & 0x80000000) == 0);

    // READ ADC VALUE

    val = (ADDR >> 6) & 0x3FF;

    return val;
}
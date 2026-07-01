#include <LPC21xx.h>

void lcd_cmd(unsigned char);
void lcd_string(char *);

void eint0_isr(void) __irq
{
    lcd_cmd(0x01);

    lcd_string("INTERRUPT");

    EXTINT = 0x01;

    VICVectAddr = 0;
}

void interrupt_init(void)
{
    // Select EINT0 function

    PINSEL0 |= (1<<1);

    // Edge trigger mode

    EXTMODE = 0x01;

    // Falling edge

    EXTPOLAR = 0x00;

    // Enable Interrupt

    VICIntEnable = (1<<14);

    // Select ISR

    VICVectCntl0 = 0x20 | 14;

    VICVectAddr0 = (unsigned int)eint0_isr;
}
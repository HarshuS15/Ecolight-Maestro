#include <LPC21xx.h>
int validate_time(unsigned int h,
                  unsigned int m,
                  unsigned int s)
{
    if(h > 23)
        return 0;

    if(m > 59)
        return 0;

    if(s > 59)
        return 0;

    return 1;
}
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);

void rtc_init(void)
{
    // Reset RTC

    CCR = (1<<1);

    // RTC Prescaler for 15MHz PCLK

    PREINT  = 732;
    PREFRAC = 13004;
    VPBDIV = 0x01;
    // Initial Time

    SEC  = 0;

    MIN  = 0;

    HOUR = 15;

    DOM   = 20;

    MONTH = 5;

    YEAR  = 2026;

    // Enable RTC

    CCR = (1<<0);
}

void rtc_display(void)
{
    // TIME

    lcd_cmd(0x80);

    lcd_data((HOUR/10) + 48);
    lcd_data((HOUR%10) + 48);

    lcd_data(':');

    lcd_data((MIN/10) + 48);
    lcd_data((MIN%10) + 48);

    lcd_data(':');

    lcd_data((SEC/10) + 48);
    lcd_data((SEC%10) + 48);

    // DATE

    lcd_cmd(0xC0);

    lcd_data((DOM/10) + 48);
    lcd_data((DOM%10) + 48);

    lcd_data('/');

    lcd_data((MONTH/10) + 48);
    lcd_data((MONTH%10) + 48);

    lcd_data('/');

    lcd_data((YEAR/1000) + 48);
    lcd_data(((YEAR/100)%10) + 48);
    lcd_data(((YEAR/10)%10) + 48);
    lcd_data((YEAR%10) + 48);
}
#include <LPC21xx.h>

#define LCDDATA 0x00FF0000    // LCD Data pins connected to P0.16-P0.23

#define RS (1<<8)             // Register Select pin
#define EN (1<<9)             // Enable pin

// Software delay function

void delay_ms(unsigned int ms)
{
    unsigned int i,j;

    for(i=0;i<ms;i++)
    {
        for(j=0;j<6000;j++);
    }
}

// Send command to LCD

void lcd_cmd(unsigned char cmd)
{
    IOCLR0 = LCDDATA;

    IOSET0 = (cmd << 16);

    IOCLR0 = RS;

    IOSET0 = EN;

    delay_ms(2);

    IOCLR0 = EN;

    delay_ms(2);     // <-- Add this line
}
// Send data/character to LCD

void lcd_data(unsigned char data)
{
    IOCLR0 = LCDDATA;

    IOSET0 = (data << 16);

    IOSET0 = RS;

    IOSET0 = EN;

    delay_ms(2);

    IOCLR0 = EN;

    delay_ms(2);     // <-- Add this line
}
// Display string on LCD

void lcd_string(char *str)
{
    while(*str)
    {
        lcd_data(*str);
        str++;
    }
}
// Initialize LCD in 8-bit mode

void lcd_init(void)
{
    IODIR0 |= LCDDATA | RS | EN;   // Configure LCD pins as output

    delay_ms(20);

    lcd_cmd(0x38);                 // 8-bit, 2-line LCD

    lcd_cmd(0x0C);                 // Display ON, Cursor OFF

    lcd_cmd(0x06);                 // Auto increment cursor

    lcd_cmd(0x01);                 // Clear LCD
	delay_ms(5);
}

// Display integer number on LCD

void lcd_num(unsigned int num)
{
    char arr[5];

    int i=0;

    if(num==0)
    {
        lcd_data('0');

        return;
    }

    // Convert number into digits

    while(num)
    {
        arr[i++] = (num%10)+48;

        num = num/10;
    }

    // Display digits in correct order

    while(i)
    {
        lcd_data(arr[--i]);
    }
}
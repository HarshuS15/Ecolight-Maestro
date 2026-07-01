#include <LPC21xx.h>

// 4x3 PHONE KEYPAD

char keypad[4][3] =
{
    {'3','2','1'},
    {'6','5','4'},
    {'9','8','7'},
    {'#','0','*'}
};

void delay_ms(unsigned int);

void keypad_init(void)
{
    // ROWS -> OUTPUT

    IODIR1 |= (1<<16)|(1<<17)|(1<<18);

    // COLUMNS -> INPUT

    IODIR1 &= ~((1<<19)|(1<<20)|(1<<21)|(1<<22));
}

char keypad_scan(void)
{
    unsigned int row;

    for(row=0; row<3; row++)
    {
        // ALL ROWS HIGH

        IOSET1 = (0x07<<16);

        // ONE ROW LOW

        IOCLR1 = (1<<(16+row));

        // COLUMN A

        if(!(IOPIN1 & (1<<19)))
        {
            delay_ms(5);

            return keypad[0][row];
        }

        // COLUMN B

        if(!(IOPIN1 & (1<<20)))
        {
            delay_ms(5);

            return keypad[1][row];
        }

        // COLUMN C

        if(!(IOPIN1 & (1<<21)))
        {
            delay_ms(5);

            return keypad[2][row];
        }

        // COLUMN D

        if(!(IOPIN1 & (1<<22)))
        {
            delay_ms(5);

            return keypad[3][row];
        }
    }

    return 0;
}
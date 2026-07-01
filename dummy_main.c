#include <LPC21xx.h>

void lcd_init(void);
void lcd_cmd(unsigned char);
void lcd_string(char *);
void lcd_data(unsigned char);
void delay_ms(unsigned int);

void keypad_init(void);
char keypad_scan(void);

void rtc_init(void);
void rtc_display(void);
void rtc_menu(void);

void adc_init(void);
unsigned int adc_read(void);

void led_init(void);
void led_on(void);
void led_off(void);


//================ GLOBAL VARIABLES =================

char key;
char temp[3];
unsigned int ldr;

volatile unsigned char rtc_int_flag = 0;   // Interrupt flag

unsigned int day_no=0;

char day[7][4]={
"MON","TUE","WED",
"THU","FRI","SAT","SUN"
};


//================ WAIT RELEASE =================

void wait_release(void)
{
 delay_ms(100);

 while(keypad_scan()!=0);

 delay_ms(100);
}


//================ LCD BACKSPACE =================

void lcd_backspace(void)
{
 lcd_cmd(0x10);
 lcd_data(' ');
 lcd_cmd(0x10);
}


//================================================
//          EINT1 INTERRUPT FOR RTC EDIT
//              SWITCH ON P0.14
//================================================


__irq void EINT1_ISR(void)
{
 rtc_int_flag = 1;      // stop normal operation

 EXTINT = (1<<1);       // clear EINT1 interrupt

 VICVectAddr = 0;
}


// EINT1 INITIALIZATION

void eint1_init(void)
{

 // P0.14 configured as EINT1

 PINSEL0 &= ~(3<<28);
 PINSEL0 |=  (2<<28);


 // Edge triggered interrupt

 EXTMODE |= (1<<1);


 // Falling edge interrupt

 EXTPOLAR &= ~(1<<1);


 // VIC configuration

 VICIntSelect &= ~(1<<15);

 VICVectCntl0 = 0x20 | 15;

 VICVectAddr0 = 
 (unsigned long)EINT1_ISR;

 VICIntEnable |= (1<<15);

}


//================================================
//     INTERRUPT MENU DISPLAY ON LCD
//================================================

void rtc_interrupt_menu(void)
{

 lcd_cmd(0x01);

 lcd_cmd(0x80);

 lcd_string("1.EDIT RTC INFO");


 lcd_cmd(0xC0);

 lcd_string("2.EXIT");


 while(1)
 {

  key = keypad_scan();


  // EDIT RTC OPTION

  if(key=='1')
  {
   wait_release();

   rtc_menu();

   break;
  }


  // EXIT OPTION

  else if(key=='2')
  {
   wait_release();

   lcd_cmd(0x01);

   lcd_string("EXIT");

   delay_ms(500);

   break;
  }

 }

 lcd_cmd(0x01);

}
void get_2digit_value(unsigned int *value)
{
 unsigned char i=0;

 while(1)
 {
  key = keypad_scan();

  if((key>='0') && (key<='9'))
  {
   if(i<2)
   {
    temp[i]=key;
    lcd_data(key);
    i++;
    wait_release();
   }
  }

  else if(key=='*')
  {
   if(i>0)
   {
    i--;
    lcd_backspace();
    wait_release();
   }
  }

  else if(key=='#')
  {
   if(i==2)
   {
    wait_release();
    break;
   }
  }
 }

 *value=((temp[0]-48)*10)+(temp[1]-48);
 
}

//================ EDIT HOUR =================

void edit_hour(void)
{
 unsigned int value;

 while(1)
 {
  lcd_cmd(0x01);
  lcd_string("ENTER HOUR:");
  lcd_cmd(0xC0);

  get_2digit_value(&value);

  if(value<=23)
  {
   HOUR=value;
   break;
  }

  lcd_cmd(0x01);
  lcd_string("INVALID HOUR");
  delay_ms(1000);
 }

 lcd_cmd(0x01);
 lcd_string("HOUR UPDATED");
 delay_ms(1000);
}



//================ EDIT MINUTE =================

void edit_min(void)
{
 unsigned int value;

 while(1)
 {
  lcd_cmd(0x01);
  lcd_string("ENTER MIN:");
  lcd_cmd(0xC0);

  get_2digit_value(&value);

  if(value<=59)
  {
   MIN=value;
   break;
  }

  lcd_cmd(0x01);
  lcd_string("INVALID MIN");
  delay_ms(1000);
 }

 lcd_cmd(0x01);
 lcd_string("MIN UPDATED");
 delay_ms(1000);
}



//================ EDIT SECOND =================

void edit_sec(void)
{
 unsigned int value;

 while(1)
 {
  lcd_cmd(0x01);
  lcd_string("ENTER SEC:");
  lcd_cmd(0xC0);

  get_2digit_value(&value);

  if(value<=59)
  {
   SEC=value;
   break;
  }

  lcd_cmd(0x01);
  lcd_string("INVALID SEC");
  delay_ms(1000);
 }

 lcd_cmd(0x01);
 lcd_string("SEC UPDATED");
 delay_ms(1000);
}



//================ EDIT DATE =================

void edit_date(void)
{
 unsigned int value;

 while(1)
 {
  lcd_cmd(0x01);
  lcd_string("ENTER DATE:");
  lcd_cmd(0xC0);

  get_2digit_value(&value);

  if(value>=1 && value<=31)
  {
   DOM=value;
   break;
  }

  lcd_cmd(0x01);
  lcd_string("INVALID DATE");
  delay_ms(1000);
 }

 lcd_cmd(0x01);
 lcd_string("DATE UPDATED");
 delay_ms(1000);
}



//================ EDIT MONTH =================

void edit_month(void)
{
 unsigned int value;

 while(1)
 {
  lcd_cmd(0x01);
  lcd_string("ENTER MONTH:");
  lcd_cmd(0xC0);

  get_2digit_value(&value);

  if(value>=1 && value<=12)
  {
   MONTH=value;
   break;
  }

  lcd_cmd(0x01);
  lcd_string("INVALID MONTH");
  delay_ms(1000);
 }

 lcd_cmd(0x01);
 lcd_string("MONTH UPDATED");
 delay_ms(1000);
}



//================ EDIT YEAR =================

void edit_year(void)
{
 unsigned int value;

 lcd_cmd(0x01);
 lcd_string("ENTER YEAR:");
 lcd_cmd(0xC0);

 get_2digit_value(&value);

 // Example: enter 26 means 2026

 YEAR = 2000 + value;

 lcd_cmd(0x01);
 lcd_string("YEAR UPDATED");

 delay_ms(1000);
}



//================ EDIT DAY =================

void edit_day(void)
{
 lcd_cmd(0x01);

 lcd_string("SELECT DAY");

 lcd_cmd(0xC0);

 lcd_string("1-7");


 while(1)
 {
  key=keypad_scan();

  if(key>='1' && key<='7')
  {
   day_no=key-'1';

   wait_release();

   break;
  }
 }


 lcd_cmd(0x01);

 lcd_string("DAY UPDATED");

 delay_ms(1000);
}

void rtc_menu(void)
{
 unsigned char page=0;


 while(1)
 {

  lcd_cmd(0x01);


  switch(page)
  {

   case 0:

    lcd_cmd(0x80);
    lcd_string("1.HOUR");

    lcd_cmd(0xC0);
    lcd_string("2.MIN #NEXT");

    break;


   case 1:

    lcd_cmd(0x80);
    lcd_string("3.SEC");

    lcd_cmd(0xC0);
    lcd_string("4.DATE #NEXT");

    break;


   case 2:

    lcd_cmd(0x80);
    lcd_string("5.DAY");

    lcd_cmd(0xC0);
    lcd_string("6.MONTH  #NEXT");

    break;
	 
	  case 3:

    lcd_cmd(0x80);
    lcd_string("7.YEAR");

    lcd_cmd(0xC0);
    lcd_string("8.EXIT");

    break;

  }


  // read key

  while(1)
  {
   key=keypad_scan();

   if(key!=0)
   {
    wait_release();
    break;
   }
  }



  switch(key)
  {

   case '1':

    if(page==0)
     edit_hour();

    break;



   case '2':

    if(page==0)
     edit_min();

    break;



   case '3':

    if(page==1)
     edit_sec();

    break;



   case '4':

    if(page==1)
     edit_date();

    break;



case '5':

    if(page==2)
    {
        edit_day();
    }

    break;


case '6':

    if(page==2)
    {
        edit_month();
    }

    break;


case '7':

    if(page==3)
    {
        edit_year();
    }

    break;


case '8':

    if(page==3)
    {
        lcd_cmd(0x01);
        lcd_string("EXIT");

        delay_ms(500);

        lcd_cmd(0x01);

        return;
    }

    break;



   case '#':

    page++;

    if(page>3)
     page=0;

    break;

  }

 }

}

int main()
{
 lcd_init();

 keypad_init();

 rtc_init();

 adc_init();

 led_init();


 // interrupt initialization
 // switch connected P0.14

 eint1_init();


 lcd_cmd(0x80);
 lcd_string("STREET LIGHT");

 lcd_cmd(0xC0);
 lcd_string("SYSTEM");

 delay_ms(5000);

 lcd_cmd(0x01);


 while(1)
 {

  // CHECK INTERRUPT

  if(rtc_int_flag==1)
  {
   rtc_int_flag=0;

   rtc_interrupt_menu();
  }


  // NORMAL RTC DISPLAY

  rtc_display();


  // READ LDR

  ldr=adc_read();


  // DISPLAY DAY

  lcd_cmd(0x8B);

  lcd_string(day[day_no]);



  // LED CONTROL

  if((HOUR>=18)||(HOUR<6))
  {
   led_on();
  }

  else
  {
   led_off();
  }


  delay_ms(100);

 }

}

#include <reg51.h>

sbit RS = P3^2;
sbit EN = P3^3;

void delay_ms(unsigned int ms)
{
    unsigned int i, j;

    for(i = 0; i < ms; i++)
    {
        for(j = 0; j < 1000; j++);
    }
}

void lcd_pulse(void)
{
    EN = 1;
    delay_ms(1);

    EN = 0;
    delay_ms(1);
}

void lcd_send_nibble(unsigned char value)
{
    P3 = (P3 & 0x0F) | (value & 0xF0);

    lcd_pulse();
}

void lcd_cmd(unsigned char cmd)
{
    RS = 0;

    lcd_send_nibble(cmd & 0xF0);
    lcd_send_nibble((cmd << 4) & 0xF0);

    delay_ms(2);
}

void lcd_data(unsigned char value)
{
    RS = 1;

    lcd_send_nibble(value & 0xF0);
    lcd_send_nibble((value << 4) & 0xF0);

    delay_ms(2);
}

void lcd_init(void)
{
    RS = 0;
    EN = 0;

    delay_ms(20);

    lcd_send_nibble(0x30);
    delay_ms(5);

    lcd_send_nibble(0x30);
    delay_ms(2);

    lcd_send_nibble(0x30);
    delay_ms(2);

    lcd_send_nibble(0x20);
    delay_ms(2);

    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);

    delay_ms(5);
}

void lcd_goto(unsigned char row, unsigned char col)
{
    if(row == 0)
    {
        lcd_cmd(0x80 + col);
    }
    else
    {
        lcd_cmd(0xC0 + col);
    }
}

void lcd_puts(char *str)
{
    while(*str != '\0')
    {
        lcd_data(*str);
        str++;
    }
}

void lcd_print_number(unsigned int num)
{
    unsigned int divisor;
    unsigned char started;

    divisor = 10000;
    started = 0;

    if(num == 0)
    {
        lcd_data('0');
        return;
    }

    while(divisor > 0)
    {
        if((num / divisor) > 0 || started == 1)
        {
            lcd_data((num / divisor) + '0');
            started = 1;
        }

        num = num % divisor;
        divisor = divisor / 10;
    }
}

void main(void)
{
    lcd_init();

    lcd_goto(0, 0);
    lcd_puts("HI");

    lcd_goto(1, 3);
    lcd_puts("There");

    lcd_goto(0, 8);
    lcd_print_number(13);

    while(1);
}

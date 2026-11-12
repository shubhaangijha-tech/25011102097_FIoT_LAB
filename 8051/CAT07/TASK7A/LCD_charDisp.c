
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

void lcd_puts(char *str)
{
    while(*str != '\0')
    {
        lcd_data(*str);
        str++;
    }
}

void main(void)
{
    lcd_init();

    lcd_cmd(0x80);
    lcd_data('A');

    lcd_cmd(0xC0);
    lcd_data('B');

    lcd_cmd(0x80);
    lcd_puts("ABC");

    lcd_cmd(0xC0);
    lcd_puts("DEF");

    while(1);
}

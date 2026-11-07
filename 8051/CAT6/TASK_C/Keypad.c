#include <reg51.h>

sbit RS = P3^2;
sbit EN = P3^3;

sbit c1 = P2^3;
sbit c2 = P2^2;
sbit c3 = P2^1;
sbit c4 = P2^0;

sbit r1 = P2^7;
sbit r2 = P2^6;
sbit r3 = P2^5;
sbit r4 = P2^4;

unsigned char row = 0;
unsigned char col = 0;

void delay_ms(unsigned int ms)
{
    unsigned int i, j;

    for(i = 0; i < ms; i++)
    {
        for(j = 0; j < 1275; j++);
    }
}

unsigned char keypad_scan(void)
{
    r1 = 0;
    r2 = 1;
    r3 = 1;
    r4 = 1;

    if(c2 == 0)
    {
        delay_ms(20);
        while(c2 == 0);
        return 2;
    }

    r1 = 1;
    r2 = 0;
    r3 = 1;
    r4 = 1;

    if(c1 == 0)
    {
        delay_ms(20);
        while(c1 == 0);
        return 4;
    }

    if(c3 == 0)
    {
        delay_ms(20);
        while(c3 == 0);
        return 6;
    }

    r1 = 1;
    r2 = 1;
    r3 = 0;
    r4 = 1;

    if(c2 == 0)
    {
        delay_ms(20);
        while(c2 == 0);
        return 8;
    }

    return 255;
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

void lcd_putc(unsigned char value)
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

    delay_ms(2);
}

void lcd_goto(unsigned char r, unsigned char c)
{
    if(r == 0)
        lcd_cmd(0x80 + c);
    else
        lcd_cmd(0xC0 + c);
}

void move_character(unsigned char key)
{
    lcd_goto(row, col);
    lcd_putc(' ');

    if(key == 2)
    {
        row = 0;
    }
    else if(key == 8)
    {
        row = 1;
    }
    else if(key == 6)
    {
        if(col == 15)
            col = 0;
        else
            col++;
    }
    else if(key == 4)
    {
        if(col == 0)
            col = 15;
        else
            col--;
    }

    lcd_goto(row, col);
    lcd_putc('*');
}

void main(void)
{
    unsigned char key;

    P2 = 0xFF;
    P3 = 0x00;

    lcd_init();

    row = 0;
    col = 0;

    lcd_goto(row, col);
    lcd_putc('*');

    while(1)
    {
        key = keypad_scan();

        if(key == 2 || key == 4 || key == 6 || key == 8)
        {
            move_character(key);
        }
    }
}

#include <reg51.h>

sbit c1 = P2^3;
sbit c2 = P2^2;
sbit c3 = P2^1;
sbit c4 = P2^0;

sbit r1 = P2^7;
sbit r2 = P2^6;
sbit r3 = P2^5;
sbit r4 = P2^4;

sbit led1 = P1^1;
sbit led2 = P1^2;
sbit led3 = P1^3;
sbit led4 = P1^4;

int count = 0;
unsigned char new_key = 0;
unsigned char key_value = 0;

void delay(void)
{
    int i,j;

    for(i=0;i<300;i++)
    {
        for(j=0;j<400;j++)
        {
        }
    }
}

unsigned char seg[16] =
{
    0x3F,0x06,0x5B,0x4F,
    0x66,0x6D,0x7D,0x07,
    0x7F,0x6F,0x77,0x7C,
    0x39,0x5E,0x79,0x71
};

void display_digit(unsigned char value)
{
    P0 = seg[value];
}

void keypad_scan(void);
void led_control(unsigned char key);
void running_led(void);
void alt_led(void);
void bin_count(void);
void blink_all(void);

void led_control(unsigned char key)
{
    switch(key)
    {
        case 1:
            led1=0;
            led2=led3=led4=1;
            break;

        case 2:
            led2=0;
            led1=led3=led4=1;
            break;

        case 3:
            led3=0;
            led1=led2=led4=1;
            break;

        case 4:
            led4=0;
            led1=led2=led3=1;
            break;

        case 5:
            running_led();
            break;

        case 6:
            alt_led();
            break;

        case 7:
            count=0;
            bin_count();
            break;

        case 8:
            blink_all();
            break;

        case 9:
            led1=led2=led3=led4=1;
            break;

        case 0:
            led1=led2=led3=led4=1;
            count=0;
            display_digit(0);
            break;

        default:
            break;
    }
}

void running_led(void)
{
    while(1)
    {
        led1=0;
        led2=led3=led4=1;

        delay();
        keypad_scan();
        if(new_key) return;

        led2=0;
        led1=led3=led4=1;

        delay();
        keypad_scan();
        if(new_key) return;

        led3=0;
        led1=led2=led4=1;

        delay();
        keypad_scan();
        if(new_key) return;

        led4=0;
        led1=led2=led3=1;

        delay();
        keypad_scan();
        if(new_key) return;
    }
}

void alt_led(void)
{
    while(1)
    {
        led1=led3=0;
        led2=led4=1;

        delay();
        keypad_scan();
        if(new_key) return;

        led1=led3=1;
        led2=led4=0;

        delay();
        keypad_scan();
        if(new_key) return;
    }
}

void bin_count(void)
{
    int temp;

    while(1)
    {
        temp=count;

        led4=!(temp%2);
        temp/=2;

        led3=!(temp%2);
        temp/=2;

        led2=!(temp%2);
        temp/=2;

        led1=!(temp%2);

        delay();
        keypad_scan();
        if(new_key) return;

        ++count;

        if(count>15)
            count=0;
    }
}

void blink_all(void)
{
    while(1)
    {
        led1=led2=led3=led4=0;

        delay();
        keypad_scan();
        if(new_key) return;

        led1=led2=led3=led4=1;

        delay();
        keypad_scan();
        if(new_key) return;
    }
}

void keypad_scan(void)
{
    new_key=0;

    r1=0;
    r2=r3=r4=1;

    if(c1==0)
    {
        delay();
        while(c1==0);
        key_value=1;
        new_key=1;
        return;
    }

    if(c2==0)
    {
        delay();
        while(c2==0);
        key_value=2;
        new_key=1;
        return;
    }

    if(c3==0)
    {
        delay();
        while(c3==0);
        key_value=3;
        new_key=1;
        return;
    }

    r1=1;
    r2=0;
    r3=r4=1;

    if(c1==0)
    {
        delay();
        while(c1==0);
        key_value=4;
        new_key=1;
        return;
    }

    if(c2==0)
    {
        delay();
        while(c2==0);
        key_value=5;
        new_key=1;
        return;
    }

    if(c3==0)
    {
        delay();
        while(c3==0);
        key_value=6;
        new_key=1;
        return;
    }

    r1=1;
    r2=1;
    r3=0;
    r4=1;

    if(c1==0)
    {
        delay();
        while(c1==0);
        key_value=7;
        new_key=1;
        return;
    }

    if(c2==0)
    {
        delay();
        while(c2==0);
        key_value=8;
        new_key=1;
        return;
    }

    if(c3==0)
    {
        delay();
        while(c3==0);
        key_value=9;
        new_key=1;
        return;
    }

    r1=1;
    r2=1;
    r3=1;
    r4=0;

    if(c2==0)
    {
        delay();
        while(c2==0);
        key_value=0;
        new_key=1;
        return;
    }
}

void main(void)
{
    led1=led2=led3=led4=1;

    display_digit(0);

    while(1)
    {
        if(new_key==0)
            keypad_scan();

        if(new_key==1)
        {
            display_digit(key_value);

            new_key=0;

            led_control(key_value);
        }
    }
}

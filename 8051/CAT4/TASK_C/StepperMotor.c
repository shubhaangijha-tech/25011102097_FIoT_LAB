
#include <reg51.h>

sbit direction = P3^0;
sbit speed1 = P3^1;
sbit speed2 = P3^2;

unsigned char step_index = 0;
unsigned char step_delay = 100;
unsigned char count = 0;
unsigned char step_due = 0;

unsigned char step_pattern[4] = {0x08, 0x04, 0x02, 0x01};

void timer0_isr(void) interrupt 1
{
    TH0 = 0xFC;
    TL0 = 0x66;

    count++;

    if(count >= step_delay)
    {
        count = 0;
        step_due = 1;
    }
}

void main()
{
    TMOD = 0x01;

    TH0 = 0xFC;
    TL0 = 0x66;

    P2 = 0x00;

    ET0 = 1;
    EA = 1;
    TR0 = 1;

    while(1)
    {
        if(speed1 == 1 && speed2 == 1)
        {
            step_delay = 100;
        }
        else if(speed1 == 1 && speed2 == 0)
        {
            step_delay = 50;
        }
        else
        {
            step_delay = 10;
        }

        if(step_due == 1)
        {
            step_due = 0;

            P2 = step_pattern[step_index];

            if(direction == 0)
            {
                step_index++;

                if(step_index >= 4)
                {
                    step_index = 0;
                }
            }
            else
            {
                if(step_index == 0)
                {
                    step_index = 3;
                }
                else
                {
                    step_index--;
                }
            }
        }
    }
}

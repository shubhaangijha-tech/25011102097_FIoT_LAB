
#include <reg51.h>

sbit led1 = P1^1;

int count = 0;
int duty = 0;
int fade_count = 0;
int increasing = 1;

void main()
{
    TMOD = 0x01;

    TH0 = 0xFA;
    TL0 = 0x9C;

    TR0 = 1;

    led1 = 1;

    while(1)
    {
        if(TF0 == 1)
        {
            TF0 = 0;

            TH0 = 0xFA;
            TL0 = 0x9C;

            count++;

            if(count >= 100)
            {
                count = 0;
            }

            if(count < duty)
            {
                led1 = 0;
            }
            else
            {
                led1 = 1;
            }

            fade_count++;

            if(fade_count >= 533)
            {
                fade_count = 0;

                if(increasing == 1)
                {
                    duty = duty + 10;

                    if(duty >= 100)
                    {
                        duty = 100;
                        increasing = 0;
                    }
                }
                else
                {
                    duty = duty - 10;

                    if(duty <= 0)
                    {
                        duty = 0;
                        increasing = 1;
                    }
                }
            }
        }
    }
}

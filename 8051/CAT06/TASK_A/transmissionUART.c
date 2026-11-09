#include <reg51.h>

void uart_init()
{
    TMOD = 0x20;
    TH1 = 0xFD;
    SCON = 0x50;
    TR1 = 1;
}

void main()
{
    uart_init();

    while(1)
    {
        SBUF = 'A';

        while(TI == 0);

        TI = 0;
    }
}
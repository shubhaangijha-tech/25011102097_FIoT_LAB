#include <reg51.h>
void uart_init(){
	TMOD=0x20;
	TH1=0xFD;
	SCON=0x50;
	TR1=1;
	TI=0;		
	RI=0;
}

void main(){
	char ch;
	uart_init();
	while(1){
		while (RI==0);
		ch=SBUF;
RI=0;
SBUF=ch;    
  while(TI == 0);  
  TI = 0;            
	//delay();
	}
}

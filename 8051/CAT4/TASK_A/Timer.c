#include <reg51.h> 
sbit led1=P1^1; 
int count=0; 
void main(){ 
	TMOD=0x01; 
		TH0=0xFA; 
		TL0=0x9C; 
		TR0=1; 
	led1=1; 
	while(1){ 
		 
		if(TF0==1){ 
			TF0=0; //once overflow detected set the TF0=0 
			count++; 
			if(count>533){ 
				led1=~(led1); 
				count=0; 
				 
			} 
			TH0 = 0xFA; //reload after overflow 
				TL0 = 0x9C; 
		} 
	} 
}

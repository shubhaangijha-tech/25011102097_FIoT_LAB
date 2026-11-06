#include <reg51.h>
sbit led1 = P1^1;
void delay(){
	int i,j;
	for(i=0;i<350;i++)
	{
		for(j=0;j<500;j++)
		{
		}
		
	}
}

 void main(void){
	 while(1){
	 led1=1;
	 delay();
	 led1=0;
	 delay();
	 }
 }
 
	 
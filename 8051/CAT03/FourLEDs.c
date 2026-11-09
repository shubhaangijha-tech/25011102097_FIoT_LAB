#include <reg51.h>
sbit ip=P2^3;
sbit op=P2^5;
sbit led1=P1^1;
sbit led2=P1^3;
sbit led3=P1^5;
sbit led4=P1^7;
int mode=0;
int count=0;

void delay(){
	int i,j;
	for(i=0;i<150;i++)
	{
		for(j=0;j<150;j++)
		{
		}
	}
}

void mode1()
{
	led2=led3=led4=0;
  led1=1;
  delay();

  led1=led3=led4=0;
  led2=1;
  delay();

  led1=led2=led4=0;
	led3=1;
  delay();

	led1=led2=led3=0;
	led4=1;
  delay();
}

void mode2(){
	int temp;
	temp=count;
	led4=(temp%2);
	temp/=2;
	led3=(temp%2);
	temp/=2;
	led2=(temp%2);
	temp/=2;
	led1=(temp%2);
	delay();
	++count;
	if(count>15)
	{
		count=0;
	}
}

void mode3(){
	led1=led3=0;
	led2=led4=1;
	delay();
	led1=led3=1;
	led2=led4=0;
	delay();
}

void mode4(){
	led1=led2=led3=led4=0;
	delay();
	led1=led2=led3=led4=1;
	delay();
}
	
void main()
{
	op=0;
	while(1){
		if(ip==0)
		{
			//delay();
			while(ip==0);
			//delay();
			mode++;
		}
		if(mode>4)
		{
			mode=1;
		}
		if(mode==1)
		{
			mode1();
		}
		if(mode==2)
		{
			mode2();
		}
		if(mode==3)
		{
			mode3();
		}
		if(mode==4)
		{
			mode4();
		}
}
	
}
		
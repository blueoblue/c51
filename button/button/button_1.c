#include<reg52.h>

sbit led=P2^0;
sbit button=P3^0;

void delay(){
	TMOD=0x01;
	TH0=0xDB;
	TL0=0xFF;
	TR0=1;

	while(!TF0);
	TF0=0;
	TR0=0;
}
void main(){
	while(1){
		if(button==0){
			delay();
			if(button==0){
				led=~led;
			}
		}
	}
}
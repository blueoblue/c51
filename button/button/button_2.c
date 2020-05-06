#include<reg52.h>
typedef unsigned uchar;

bit keyState=1;
sbit led=P2^0;
sbit button=P3^1;
uchar keyBuf=0xFF;


void main(){
	bit backup=1;
	TMOD=0X01;
	TH0=0XF8;
	TL0=0XCB;

	EA=1;
	ET0=1;
	TR0=1;
	
	while(1){
		if(keyState!=backup){
			// 本次按键状态为1，按键弹开
			if(keyState){
				led=~led;
			}
			backup=keyState;
		}
	}
}

void interruptT() interrupt 1{
	TH0=0XF8;
	TL0=0XCB;

	keyBuf=(keyBuf << 1) | button;
	if(keyBuf==0x00 || keyBuf==0xFF){
		keyState=button;
	}
}
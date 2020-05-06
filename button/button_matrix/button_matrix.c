/**********************************
< Describe  : 用第一行按键控制led灯P2^0-3
< Time      : 2020-05-03 10:12:29
**********************************/

#include<reg52.h>
typedef unsigned char uchar;

sbit button_in=P1^7;
sbit button0=P1^0;
sbit button1=P1^1;
sbit button2=P1^2;
sbit button3=P1^3;
sbit led0=P2^0;
sbit led1=P2^1;
sbit led2=P2^2;
sbit led3=P2^3;

uchar keyState[]={1,1,1,1};
uchar keyBuf[]={0xFF,0xFF,0xFF,0xFF};

void main(){
	uchar i;
	uchar backup[]={1,1,1,1};
	TMOD=0x01;
	// 定时器为2ms，每次扫描四个按键，每个按键扫描次数为8次，共8x2=16ms.
	TH0=0XF8;
	TL0=0XCB;

	ET0=1;
	EA=1;
	TR0=1;
	button_in=0;
	
	while(1){
		for(i=0;i<4;i++){
			if(keyState[i]!=backup[i]){
				if(keyState[i]==1){
					switch(i){
						case 0: led0=~led0;break;
						case 1: led1=~led1;break;
						case 2: led2=~led2;break;
						case 3: led3=~led3;break;
					}
				}
				backup[i]=keyState[i];
			}
		}
	}
}

void interruptT() interrupt 1{
	uchar i;
	TH0=0XF8;
	TL0=0XCB;

	for(i=0;i<4;i++){
		// 先进行检测，然后判断状态
		keyBuf[i]=(keyBuf[i] << 1) | ((P1 >> (~i & 0x03)) & 0x01);

		if((keyBuf[i])==0){
			keyState[i]=0;
		}else if((keyBuf[i])==1){
			keyState[i]=1;
		}
 	}
}
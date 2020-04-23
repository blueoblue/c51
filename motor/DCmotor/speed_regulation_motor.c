#include<reg52.h>

typedef unsigned char uchar;
typedef unsigned int uint;

sbit led=P2^0;
sbit led1=P2^1;
sbit motor=P1^0;
// Ton维持的时间
uchar Time;
// 总调节时间
const uchar allTime=100;

void pwm(bit state);
void run(uchar Ton);
void delay(uchar t);

int main()
{
	EA=1;
	run(20);
	return 0;
}


void pwm(bit state){
	/*
	 Description：该段时间内，motor启动/停止
	 state：motor的状态（启动/停止）
	 */
	uint count;
	count=65535-(Time/10)*((11059200/12)/1000);
	motor=state;

	TMOD=(TMOD & 0xFC) | 0x01;
	TH0=(uchar)(count >> 8);
	TL0=(uchar)count;

	ET0=1;
	TR0=1;

	while(!TF0);
}

void interruptT0() interrupt 1{
	if(motor){
		Time=allTime-Time;
		pwm(0);
	}else{
		TR0=0;
	}
}

void run(uchar Ton){
	/*
	 description：转速调节
	 */
	while(1){
		Time=Ton;
		pwm(1);
	}

}

void delay(uchar t){
	/*
	 由于是从0开始计数，所以不需要初始化TH1、TL1
	 */
	uchar i=0;
	TMOD=(TMOD & 0x0F) | 0x10;


	for(;i<t;i++){
		TR1=1;
		while(!TF1);
		TR1=0;
		TF1=0;
	}
}
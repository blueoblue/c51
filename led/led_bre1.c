#include<reg52.h>

typedef unsigned char uchar;
typedef unsigned int uint;

sbit led=P2^0;
// 亮/灭维持的时间
uchar Time;
// 总调节时间
const uchar allTime=100;

void pwm(bit state);
void breath_led(uchar cycle);

int main()
{
	EA=1;
	breath_led(2);
	return 0;
}


void pwm(bit state){
	/*
	 Description：该段时间内，led亮/灭时常
	 state：led的状态（亮/灭）
	 */
	uint count;
	// Time/10：参数Time为10倍的ms，0.5ms以浮点数不够精确
	count=65535-(Time/10)*((11059200/12)/1000);
	led=state;

	TMOD=(TMOD & 0xFC) | 0x01;
	TH0=(uchar)(count >> 8);
	TL0=(uchar)count;

	ET0=1;
	TR0=1;

	while(!TF0);
}

void interruptT0() interrupt 1{
	/*
	 》》》 定时器标志TF0触发中断后，会被硬件 置零。
	 description：该时间段内，从状态0（亮/灭）转换为状态1（灭/亮）；
	 */
	if(!led){
		Time=allTime-Time;
		pwm(1);
	}else{
		TR0=0;
	}
}

void breath_led(uchar cycle){
	/*
	 description：呼吸灯效果
	 cycle：每个阶段持续的周期数；越大变化越慢，反之越快
	 */
	// 第几个亮度等级
	uchar j;
	// 该亮度下第几周期
	uchar i;
	// 每隔0.5ms一个亮度等级
	uchar add=5;

	while(1){
		if(i==0){
			add=5;
		}else if(i==allTime){
			add=-5;
		}
		// Time=i; 
		// 如下，每个亮度阶段持续周期
		for(j=0;j<cycle;j++){
			Time=i;
			pwm(0);
		}
		i=i+add;
	}
}
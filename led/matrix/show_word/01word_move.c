#include<reg52.h>
typedef unsigned char uchar;

sbit IDATA=P3^4;
sbit OCLK=P3^5;
sbit ICLK=P3^6;

void delay();
void send(uchar datas);
void move(uchar t,uchar *row);

void main(){
	// 每个动态扫描持续时间大约为10ms，
	// move参数t表示动态扫秒持续的周期次数。
	// t越大表示当前位置下持续显示的时间越长，字符移动的越慢。
	uchar row[]={0x01,0x1C,0x15,0xF4,0x55,0x5C,0x01,0x00};
	move(10,row);
}

void delay(){
	TMOD=(TMOD & 0xF0) | 0x01;
	TH0=0xFB;
	TL0=0xAD;

	TR0=1;
	while(!TF0);
	TF0=0;
	TR0=0;
}

void send(uchar datas){
	uchar i;
	for(i=8;i>0;i--){
		IDATA=(datas >> (i-1)) & 0x01;
		ICLK=0;
		ICLK=1;
	}
	OCLK=0;
	OCLK=1;
}

void move(uchar t,uchar *row){
	uchar i;
	uchar j;
	uchar step=7;
	uchar count;
	uchar column[]={0x7F,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFF};

	while(1){
		i=step;
		// 显示多长时间：t*10(ms)
		for(j=0;j<t;j++){
			// 动态扫描显示字符
			for(count=8;count>0;count--){
				P0=*(column+count-1);
				send(*(row+i));
				delay();
				P0=0xFF;
				send(0x00);
				i-=1;
				i&=0x07;
			}
		}
		step-=1;
		step&=0x07;
	}
}
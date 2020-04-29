#include<reg52.h>
typedef unsigned char uchar;

sbit IDATA=P3^4;
sbit OCLK=P3^5;
sbit ICLK=P3^6;

void delay();
void send(uchar datas);

void main(){
	uchar i=7;
	uchar row[]={0x01,0x1C,0x15,0xF4,0x55,0x5C,0x01,0x00};
	uchar column[]={0x7F,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFF};
	while(1){
		P0=column[i];
		send(row[i]);
		delay();
		i-=1;
		P0=0xFF;
		send(0x00);
		i&=0x07;
	}
}

void delay(){
	uchar i=0;
	TMOD=0x01;
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

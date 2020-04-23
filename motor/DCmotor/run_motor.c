#include<reg51.h>
typedef unsigned char uchar;
sbit motor=P1^0;

void delay(uchar t);
void motor_run();

int main(){
	motor_run();

	return 0;
}

void delay(uchar t){
	uchar i=0;
	TMOD=(TMOD & 0x0F) | 0x10;

	for(;i<t;i++){
		TR1=1;
		while(!TF1);
		TR1=0;
		TF1=0;
	}
}

void motor_run(){
	while(1){
		motor=0;
		led0=0;
		delay(14);
		led0=1;
		motor=1;
		delay(14);
	}
}
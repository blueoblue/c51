#include<reg52.h>

sbit led=P2^0;
sbit button=P3^1;

void main(){
	while(1){
		if(button==0){
			led=~led;
		}
	}
}
#include <reg51.h>

typedef unsigned char uchar;

sbit SER=P3^4;
sbit RCLK=P3^5;
sbit SRCLK=P3^6;

void send(uchar data1)
{
	uchar a;

	for(a=8;a>0;a--)
	{
		SER=(data1 >> (a-1)) & 0x01;
		SRCLK=0;		
		SRCLK=1;
	}
	RCLK=0;
	RCLK=1;
}

int main()
{
	send(0x01);
	P0=0x7F;
	return 0;
}
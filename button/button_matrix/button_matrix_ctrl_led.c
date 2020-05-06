/**********************************
< Describe  : 矩阵按键控制led点阵
< Time      : 2020-05-03 10:25:09
**********************************/

#include<reg52.h>
typedef unsigned char uchar;

sbit led=P2^0;
// 四行：低电平输入控制端，其中P1^5连接蜂鸣器，扫描是有噪声；需改变扫描方式
sbit bt0_in=P1^7;
sbit bt1_in=P1^6;
sbit bt2_in=P1^5;
sbit bt3_in=P1^4;

// 列控制端
// sbit columnCtrl=P0;

// led点阵列数据输入控制端
sbit IDATA=P3^4;
sbit OCLK=P3^5;
sbit ICLK=P3^6;

// 当前检测第几行
uchar index=0;

uchar keyState[4][4];
uchar keyBuf[4][4];

// 按键响应
void turn();

// 初始化按键状态矩阵：4x4
void init(uchar (*arr)[4],uchar value);
// led点阵显示
void send(uchar datas);
void ctrl(uchar x,uchar y);

void main(){
	init(keyState,1);
	init(keyBuf,0xFF);
	turn();
}

void init(uchar (*arr)[4],uchar value){
	uchar i;
	uchar j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			*(*(arr+i)+j)=value;
		}
	}
}

void turn(){
	uchar i;
	uchar j;
	uchar backup[4][4];
	init(backup,1);

	TMOD=0x01;
	TH0=0xFC;
	TL0=0x67;

	EA=1;
	ET0=1;
	TR0=1;

	while(1){
		for(i=0;i<4;i++){
			for(j=0;j<4;j++){
				// 第(i,j)个按键
				if(keyState[i][j]!=backup[i][j]){
					if(keyState[i][j]){
						led=~led;
						ctrl(i,j);
					}
					backup[i][j]=keyState[i][j];
				}
			}
		}
	}
}

void interruptT() interrupt 1{
	uchar i;
	TH0=0xFC;
	TL0=0x67;

	for(i=0;i<4;i++){
		// 读取当前行的4个列按键状态
		// 其中 (~(i-1)) ：由于按键连接线图从左到右为P1^3-0位逆序，因取反，0-3，1-2，2-1，3-0；
		// --相应的ctrl()中，间ctrl注释
		keyBuf[index][i]=((keyBuf[index][i] << 1) | ((P1 >> (~i & 0x03)) & 0x01)) & 0x0F;

		// 判断按键状态
		if((keyBuf[index][i])==0x00){
			keyState[index][i]=0;
		}else if((keyBuf[index][i])==0x0F){
			keyState[index][i]=1;
		}
	}

	// 准备检测下一行
	index+=1;
	index&=0x03;
	// 只改变行输入端P1^7-4
	// P1=(P1 & 0x0F) | (~(0x80 >> index) & 0xF0);
    switch (index){
        case 0: bt3_in = 1; bt0_in = 0; break;
        case 1: bt0_in = 1; bt1_in = 0; break;
        case 2: bt1_in = 1; bt2_in = 0; break;
        case 3: bt2_in = 1; bt3_in = 0; break;
        default: break;
    }
}
void send(uchar datas){
	uchar i=0;
	for(i=8;i>0;i--){
		IDATA=(datas >> (i-1)) & 0x01;
		ICLK=0;
		ICLK=1;
	}
	OCLK=0;
	OCLK=1;
}

void ctrl(uchar x,uchar y){
	uchar rowValue;
	uchar columnValue;
	// 由于keyState中，(0,0)位置控制为列最高位控制端，x=0时，移位为：7-0=7，其他同理
	// 列方向控制
	if(x<2){
		rowValue=0xFF;
		// 第一行控制 列高四位
		if(!x){
			columnValue=~(0x01 << (7-y));
		// 第二行控制 列低四位
		}else{
			columnValue=~(0x01 << 3-y);
		}
	// 行方向控制
	}else{
		columnValue=0x00;
		// 第三行控制 行高四位
		if(x==2){
			rowValue=0x01 << (7-y);
		// 第四行控制 行低四位
		}else{
			rowValue=(0x01 << 3-y);
		}
	}
	P0=columnValue;
	send(rowValue);	
}
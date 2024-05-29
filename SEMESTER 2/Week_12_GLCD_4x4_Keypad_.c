#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "GLCD_Eng.h"

#define KEY_DDR DDRF
#define KEY_PORT PORTF
#define KEY_PIN PINF

unsigned char key_scan(unsigned char a);
int key_convert(unsigned char in);

int main(void)
{
	//키패드관련
	unsigned char in,in0,in1,in2,in3;

	//GLCD 관련
	_delay_ms(50);                                 // wait for system stabilization
	GLCD_Port_Init();                             // initialize MCU and kit
	GLCD_Init();
	int sum=0;

	//메뉴 출력
	GLCD_String(0,0,"1.Americano 150");
	GLCD_String(1,0,"2.Latte 300");
	GLCD_String(2,0,"3.Juice 400");
	GLCD_String(3,0,"4.Reset");
	GLCD_String(5,0,"Total: ");
	

	while (1) 
	{
		// 키패드 첫번째줄 스캔
		in0 = key_scan(0x01);
		// 키패드 두번째줄 스캔
		in1 = key_scan(0x02);
		// 키패드 세번째줄 스캔
		in2 = key_scan(0x04);
		// 키패드 네번째줄 스캔
		in3 = key_scan(0x08);

		in = (in3 | in2 | in1 | in0); //4줄 입력 값 취합

		in = key_convert(in);   //키 입력 값을 숫자로 변환

		if(in >= 1 && in <=16){
			switch(in){
			case 1:
				sum+=150;
				break;
			case 2:
				sum+=300;
				break;
			case 3:
				sum+=400;
				break;
			case 4:
				sum=0;
				break;
			default:
				break;
			}			
			GLCD_xy(5, 7);  //합계만 변경하기 위해 이동
			GLCD_4DigitDecimal(sum);
		}
		
		//키패드 여러번 인식되지 않도록 적절히 딜레이
		_delay_ms(200);
	}
}

//키패드에 각 줄에 입력 값이 있는지 스캔하는 함수
unsigned char key_scan(unsigned char a) 
{
    unsigned char in;

    KEY_DDR = 0xFF;    //all out
    KEY_PORT = a;      //키패드 줄 위치
    KEY_DDR = 0x0F;    //Port 7bit~4bit(In) 3bit~0bit(Out)
    _delay_us(50);

	// 키패드 입력 포트의 상위 4자리 값 추출
    in = (PINF & 0xF0);
    if(in != 0x00)
	{
        in |= a;
        return in;
    }
    return 0;
}

int key_convert(unsigned char in)
{
	int returnvalue;
	switch(in)	{
		case 0x18 : returnvalue = 1;  break;
		case 0x14 : returnvalue = 2;  break;
		case 0x12 : returnvalue = 3;  break;
		case 0x11 : returnvalue = 4;  break;
		case 0x28 : returnvalue = 5;  break;
		case 0x24 : returnvalue = 6;  break;
		case 0x22 : returnvalue = 7;  break;
		case 0x21 : returnvalue = 8;  break;       
		case 0x48 : returnvalue = 9;  break;
		case 0x44 : returnvalue = 10;  break;
		case 0x42 : returnvalue = 11;  break;
		case 0x41 : returnvalue = 12;  break;
		case 0x88 : returnvalue = 13;  break;
		case 0x84 : returnvalue = 14;  break;
		case 0x82 : returnvalue = 15;  break;
		case 0x81 : returnvalue = 16;  break;
		default : returnvalue = 0;  break;
	}
	return returnvalue;  
}

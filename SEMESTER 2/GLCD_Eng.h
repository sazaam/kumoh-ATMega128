#define F_CPU 16000000
#include <avr/delay.h>
#include "font_5x7.h"

#define GLCD_DATABUS PORTD
#define GLCD_CONTROL PORTC
#define GLCD_DATABUS_DDR DDRD
#define GLCD_CONTROL_DDR DDRC

#define E 4
#define DI 3

// x: ��������, y: ��Ʈ��ȣ(��Ʈ�̸�)
// ��: SetBit(PORTC, DIN)
#define SetBit(x,y)  (x |= (1<<y))
#define ClrBit(x,y)  (x &= ~(1<<y))

#define GLCD_CS1 0x02		 // GLCD_CS1 Select 0000 0010
#define GLCD_CS2 0x01		 // GLCD_CS2 Select 0000 0001
#define GLCD_CS1_2 0x00		 // GLCD_CS1, GLCD_CS2 Select 0000 0011
#define ENB_ON 0x10
#define DATA_ON 0x08

#define GLCD_START_LINE 0xC0
#define GLCD_SET_X_ADDR 0xB8
#define GLCD_SET_Y_ADDR 0x40

#define DISPON 0x3F
#define DISPOFF 0x3E

unsigned char xcharacter,ycharacter;
unsigned char cursor_flag,xcursor,ycursor;

void GLCD_Port_Init(void)
{
	GLCD_DATABUS_DDR = 0xFF;
	GLCD_CONTROL_DDR = 0xFF;
}

void GLCD_Init()
{
	GLCD_Command(GLCD_CS1_2, DISPON);
	GLCD_Command(GLCD_CS1_2, GLCD_START_LINE);
	GLCD_Command(GLCD_CS1_2, GLCD_SET_X_ADDR);
	GLCD_Command(GLCD_CS1_2, GLCD_SET_Y_ADDR);
	GLCD_Clear();
}

void GLCD_Command(unsigned char signal, unsigned char command)
{
	GLCD_CONTROL = (signal | GLCD_CS1_2) ;	//DI=0 (Command)
	SetBit(GLCD_CONTROL, E);
	GLCD_DATABUS = command;	
	_delay_us(10);
	ClrBit(GLCD_CONTROL, E);
	_delay_us(10);
	GLCD_CONTROL=0x00;
	_delay_ms(1);
}

void GLCD_Data(unsigned char signal, unsigned char character)
{
	GLCD_CONTROL = (signal | GLCD_CS1_2) | SetBit(GLCD_CONTROL, DI); //DI=1 (Data)	
	SetBit(GLCD_CONTROL, E);
	GLCD_DATABUS = character;	
    _delay_us(10);
	ClrBit(GLCD_CONTROL, E);
  	_delay_us(10);
	GLCD_CONTROL=0x00;
  	_delay_ms(1);
}


void GLCD_Clear()
{
	unsigned char i,j,x;

	GLCD_CONTROL = 0x00;
	GLCD_Command(GLCD_CS1_2, DISPON);
	GLCD_Command(GLCD_CS1_2, GLCD_START_LINE);

	x = GLCD_SET_X_ADDR;

	for(i=0;i<=7;i++)
	{
		GLCD_Command(GLCD_CS1_2, x);
		GLCD_Command(GLCD_CS1_2, GLCD_SET_Y_ADDR);
		for(j=0;j<=63;j++)
		{
			GLCD_Data(GLCD_CS1_2, 0x00);
		}
		x++;
	}
}

void GLCD_xy(unsigned char x, unsigned char y)
{
	xcharacter = x;
	ycharacter = y;

	GLCD_Command(GLCD_CS1_2, GLCD_SET_X_ADDR+xcharacter);

	if(ycharacter <= 9)
		GLCD_Command(GLCD_CS1, GLCD_SET_Y_ADDR+ycharacter*6+4);
	else
		GLCD_Command(GLCD_CS2, GLCD_SET_Y_ADDR+(ycharacter-10)*6);
}

void GLCD_Character(unsigned char character)
{
	unsigned char i, signal, font_data;

	GLCD_xy(xcharacter, ycharacter);

	if(ycharacter <= 9)
		signal = GLCD_CS1;
	else
		signal = GLCD_CS2;
	
	for(i=0;i<=4;i++)
	{
		if((cursor_flag == 1) && (xcharacter == xcursor) && (ycharacter == ycursor))
			GLCD_Data(signal,(font[character-0x20][i])|0x80);
		else
			GLCD_Data(signal,font[character-0x20][i]);
	}
	GLCD_Data(signal,0x00);

	ycharacter++;
	if(ycharacter == 20)
	{
		ycharacter = 0;
		xcharacter++;
	}
}

void GLCD_String(unsigned char x, unsigned char y, unsigned char *string)
{
	xcharacter = x;
	ycharacter = y;

	while(*string != '\0')
	{
		GLCD_Character(*string);
		string++;
	}
}


// Display floating-point number xx.x
// 00.0�� �Ҽ����� ǥ���մϴ�.
void GLCD_2Dot1Float(float number)		       
{
  unsigned int i, j;
  
  j = (int)(number*10);
  i = j / 100;					// 10^1
  if(i == 0) { GLCD_Character(' ');     }
  else       { GLCD_Character(i + '0'); }

  j = j % 100;					// 10^0
  i = j / 10;
  GLCD_Character(i + '0');
  GLCD_Character('.');

  i = j % 10;					// 10^-1
  GLCD_Character(i + '0');
}


// display 1-digit decimal number
// 1�ڸ��� 10���� ���� ǥ���մϴ�.
unsigned char GLCD_1DigitDecimal(unsigned char number, unsigned char flag)
{
  number %= 10;               // 10^0
  
  if ((number == 0) && (flag == 0))
  { 
    GLCD_Character(' '); 
    return 0;
  } 
  
  GLCD_Character(number + '0');
  return 1;
}


// display 2-digit decimal number
// 2�ڸ��� 10���� ���� ǥ���մϴ�.
void GLCD_2DigitDecimal(unsigned char number)
{
  unsigned int i;
  unsigned char flag;
   
  flag = 0;
  number = number % 100;                        
  i = number/10;
  flag = GLCD_1DigitDecimal(i, flag); // 10^1

  i = number % 10;                              
  GLCD_Character(i + '0');              // 10^0
}


// display 3-digit decimal number
// 3�ڸ��� 10���� ���� ǥ���մϴ�.
void GLCD_3DigitDecimal(unsigned int number)
{
  unsigned int i;
  unsigned char flag;

  flag = 0;
  number = number % 1000; 
  i = number/100;                      
  flag = GLCD_1DigitDecimal(i, flag); // 10^2

  number = number % 100;               
  i = number/10;
  flag = GLCD_1DigitDecimal(i, flag); // 10^1

  i = number % 10;                              
  GLCD_Character(i + '0');              // 10^0
}


// display 4-digit decimal number
// 4�ڸ��� 10���� ���� ǥ���մϴ�.
void GLCD_4DigitDecimal(unsigned int number)
{
  unsigned int i;
  unsigned char flag;

  flag = 0;
  number = number % 10000;
  i = number/1000;                              
  flag = GLCD_1DigitDecimal(i, flag); // 10^3

  number = number % 1000;                       
  i = number/100;
  flag = GLCD_1DigitDecimal(i, flag); // 10^2

  number = number % 100;                        
  i = number/10;
  flag = GLCD_1DigitDecimal(i, flag); // 10^1

  i = number % 10;                              
  GLCD_Character(i + '0');              // 10^0
}


// display 1-digit hex number
// 1�ڸ��� 16���� ���� ǥ���մϴ�.
void GLCD_1DigitHex(unsigned char i)
{
  i &= 0x0F;  // 16^0
  if(i <= 9) { GLCD_Character(i + '0');      }
  else       { GLCD_Character(i - 10 + 'A'); }
}


// display 2-digit hex number
// 2�ڸ��� 16���� ���� ǥ���մϴ�.
void GLCD_2DigitHex(unsigned char number)
{
  GLCD_1DigitHex(number >> 4);    // 16^1
  GLCD_1DigitHex(number     );    // 16^0
}


// display 4-digit hex number
// 4�ڸ��� 16���� ���� ǥ���մϴ�.
void GLCD_4DigitHex(unsigned int number)
{
  GLCD_1DigitHex(number >> 12);   // 16^3
  GLCD_1DigitHex(number >> 8 );   // 16^2
  GLCD_1DigitHex(number >> 4 );   // 16^1
  GLCD_1DigitHex(number      );   // 16^0
}


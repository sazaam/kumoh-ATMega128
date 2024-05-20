#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

// 비트번호에 이름 정의
#define DIN 2
#define CS 1
#define CLK 0
#define DOT_PORT PORTG
#define DOT_DDR DDRG

// x: 레지스터, y: 비트번호(비트이름)
//예: SetBit(PORTC, DIN)

#define SetBit(x,y) (x |= (1<<y))
#define ClrBit(x,y) (x &= ~(1<<y))

unsigned char Mini_num[10][8] = {
	{0x70,0x50,0x50,0x50,0x70,0x00,0x00,0x00}, //0
	{0x20,0x60,0x20,0x20,0x70,0x00,0x00,0x00}, //1
	{0x70,0x10,0x70,0x40,0x70,0x00,0x00,0x00}, //2
	{0x70,0x10,0x70,0x10,0x70,0x00,0x00,0x00}, //3
	{0x50,0x50,0x70,0x10,0x10,0x00,0x00,0x00}, //4
	{0x70,0x40,0x70,0x10,0x70,0x00,0x00,0x00}, //5
	{0x70,0x40,0x70,0x50,0x70,0x00,0x00,0x00}, //6
	{0x70,0x50,0x50,0x10,0x10,0x00,0x00,0x00}, //7
	{0x70,0x50,0x70,0x50,0x70,0x00,0x00,0x00}, //8
	{0x70,0x50,0x70,0x10,0x70,0x00,0x00,0x00} //9
};

void SPI_Write_Byte(unsigned char num) {
	
	unsigned char t, Mask, Flag;
	
	Mask = 128;
	for (t=0; t<8; t++) {

		ClrBit(DOT_PORT, CLK);
		Flag = num & Mask;
		
		if(Flag == 0)
			ClrBit(DOT_PORT, DIN);
		else
			SetBit(DOT_PORT, DIN);
		
		SetBit(DOT_PORT, CLK);
		Mask = Mask >> 1;
	}
}

void MAX7219_INIT() {
	
	DOT_DDR=0xFF;
	
	// Set BCD decode mode
	ClrBit(DOT_PORT, CS); // CS pin is pulled LOW
	SPI_Write_Byte(0x09); // Select Decode Mode register
	SPI_Write_Byte(0x00); // Select BCD mode for digits DIG0-DIG7
	SetBit(DOT_PORT, CS); // CS pin is pulled HIGH
	// Set display brighness
	ClrBit(DOT_PORT, CS); // CS pin is pulled LOW
	SPI_Write_Byte(0x0A); // Select Intensity register
	SPI_Write_Byte(0x00); // Set brightness (0x00~0x0F)
	SetBit(DOT_PORT, CS); // CS pin is pulled HIGH
	// Set display refresh
	ClrBit(DOT_PORT, CS); // CS pin is pulled LOW
	SPI_Write_Byte(0x0B); // Select Scan-Limit register
	SPI_Write_Byte(0x07); // Select digits DIG0-DIG3
	SetBit(DOT_PORT, CS); // CS pin is pulled HIGH
	// Turn on the display
	ClrBit(DOT_PORT, CS); // CS pin is pulled LOW
	SPI_Write_Byte(0x0C);
	SPI_Write_Byte(0x01);
	SetBit(DOT_PORT, CS); // CS pin is pulled HIGH
	// Disable Display-Test
	ClrBit(DOT_PORT, CS); // CS pin is pulled LOW
	SPI_Write_Byte(0x0F); // Select Display-Test register
	SPI_Write_Byte(0x00); // Disable Display-Test
	SetBit(DOT_PORT, CS); // CS pin is pulled HIGH

}



void Write_Byte(unsigned char myColumn, unsigned char myValue){
	ClrBit(DOT_PORT, CS); // select max7219.
	SPI_Write_Byte(myColumn); // send myColumn value to max7219 (digit place).
	SPI_Write_Byte(myValue); // send myValue value to max7219 (digit place).
	SetBit(DOT_PORT, CS); // deselect max7219.
}


// This is clear matrix function.
void Clear_Matrix(void){
	unsigned char x;
	for(x=1; x<9; x++){
		Write_Byte(x,0x00);
	}
}


// 매개변수: 두 자리까지의 정수
void Write_Two_Mini_Num(unsigned char myChar){
	unsigned char Column, Ten_num, One_num;

	Clear_Matrix();
	
	Ten_num = (myChar/10)%10;
	One_num = myChar%10;
	
	for(Column=0; Column<8; Column++){
		Write_Byte(Column+1, Mini_num[Ten_num][Column] | (Mini_num[One_num][Column] >> 4));
	}
}



#define KEY_DDR DDRF
#define KEY_PORT PORTF
#define KEY_PIN PINF


unsigned char key_scan(unsigned char a) ;
int key_convert(unsigned char i) ;

void settings(){
	MAX7219_INIT() ;
}



unsigned char key_scan(unsigned char a){
	unsigned char in;

	KEY_DDR 	= 0xFF ;
	KEY_PORT 	= a ;
	KEY_DDR 	= 0x0F ;
	
	_delay_us(50) ;
	

	in = (PINF & 0xF0) ;

	if(in != 0x00){
		in |= a ;
		return in ;
	}return  0 ;
}


int key_convert(unsigned char in){

	int val ;

	switch(in){
	
		case 0x18 : val = 1 ; break ;
		case 0x16 : val = 2 ; break ;
		case 0x12 : val = 3 ; break ;
		case 0x11 : val = 4 ; break ;

		case 0x28 : val = 5 ; break ;
		case 0x26 : val = 6 ; break ;
		case 0x22 : val = 7 ; break ;
		case 0x21 : val = 8 ; break ;

		case 0x48 : val = 9 ; break ;
		case 0x46 : val = 10 ; break ;
		case 0x42 : val = 11 ; break ;
		case 0x41 : val = 12 ; break ;

		case 0x88 : val = 13 ; break ;
		case 0x86 : val = 14 ; break ;
		case 0x82 : val = 15 ; break ;
		case 0x81 : val = 16 ; break ;
		default: 	val = 0  ; break ;
	}
	return val ;

}


int main(void){
	

	
	settings() ;

	Clear_Matrix() ;

	DDRA = 0xFF ;

	unsigned char in, in0, in1, in2, in3 ;

	while(1){
		
		
		in0 = key_scan(0x01) ;
		in1 = key_scan(0x02) ;
		in2 = key_scan(0x04) ;
		in3 = key_scan(0x08) ;
		
		//PORTA = 0xFF;
		
		in = (in3 | in2 | in1 | in0) ;
		in = key_convert(in) ;
		
		PORTA = in ;

		if(in >=1 && in <= 16){
			Write_Two_Mini_Num(in) ;
		}
		//Write_Two_Mini_Num(16) ;
	}
	
	return 0 ;
}


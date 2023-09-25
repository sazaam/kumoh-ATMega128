
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000
#include <util/delay.h>

unsigned char digit [10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};
unsigned char fnd_sel[4] = {0x01, 0x02, 0x04, 0x08};

volatile int ct = 0 ;


ISR(INT4_vect){
	
	ct++;
	_delay_ms(20);

}


void display(int ct){
	int i, fnd[4] ;
	fnd[3] = ct / 1000 % 10 ;
	fnd[2] = ct / 100 % 10 ;
	fnd[1] = ct / 10 % 10 ;
	fnd[0] = ct % 10 ;


	for(i = 0 ; i < 4 ; i++){
		
		PORTC = digit[fnd[i]];
		PORTG = 0x01 << i ;
		_delay_ms(2);
	}

	
}


int main(){
	DDRC 		= 0xFF ;
	DDRG 		= 0x00 ;
	DDRE 		= 0xEF ;
	
	EIMSK 		= 0x10 ;
	EICRB 		= 0x02 ;

	sei();
	while(1) display(ct) ;

}

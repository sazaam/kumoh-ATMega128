
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#include <util/delay.h>
#include <math.h>


#define STOP 0
#define GO 1


volatile int cur_time = 0 ;
volatile int stop_time = 0 ;
volatile int state = STOP ;

unsigned char digit [10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};
unsigned char fnd_sel[4] = {0x01, 0x02, 0x04, 0x08};

volatile int ct = 0;

ISR(INT5_vect){ // ISR = SIGNAL, same
	cli();

	

	ct++;
	PORTA = ct;

	_delay_ms(20);
	sei();
}

void init(){
	
	DDRC 		= 0xFF ; // FND Data
	DDRG 		= 0x00 ; // FND Selector
	//DDRE 		= 0xEF ; // INT 4
	DDRE 		= 0xCF ; // INT 5 + INT 4
	//DDRE 		= 0xDF ; // INT 5 ONLY
	
			
	PORTC 		= digit[0];
	PORTG 		= 0x0F;

	EIMSK 		= 0x30 ; // interrupt enable -> int 4/5 Trigger int4 falling 
	EICRB 		= 0x0A ; // falling edge -> int 4/5 falling High Bit 1010

	sei();
}

int display(int ct){

	int i, fnd[4] ;

	for(i = 0 ; i < 4 ; i++){
		int mp = pow(10,i) ;
		fnd[i] = (ct / mp) % 10 ;	
		
		PORTC = digit[fnd[i]];
		PORTG = 0x01 << i ;
		_delay_ms(2);
	}
	return state ;
}


int main(){
	init() ;

	while(1){
		//if (display(state ? cur_time : stop_time)) cur_time++;
		display(ct) ;

	}


}

















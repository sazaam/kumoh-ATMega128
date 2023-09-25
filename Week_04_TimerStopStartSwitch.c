
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#include <util/delay.h>



#define STOP 0
#define GO 1


volatile int cur_time = 0 ;
volatile int stop_time = 0 ;
volatile int state = STOP ;

unsigned char digit [10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};
unsigned char fnd_sel[4] = {0x01, 0x02, 0x04, 0x08};



ISR(INT4_vect){
	
	if(state){
		stop_time = cur_time ;
	}
	state = !state ;
	_delay_ms(20);

}

void init(){
	
	DDRC 		= 0xFF ;
	DDRG 		= 0x00 ;
	DDRE 		= 0xEF ;
	
	PORTC 		= digit[0];
	PORTG 		= 0x0F;


	EIMSK 		= 0x30 ;
	EICRB 		= 0x0A ;

	sei();
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
	init() ;

	while(1){
		 display(state ? cur_time : stop_time) ;
		cur_time++;
	}


}

















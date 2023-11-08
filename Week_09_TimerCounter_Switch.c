#include <avr/io.h>
#include <avr/interrupt.h>


volatile unsigned int tick_cnt=0;
volatile unsigned int speed=500;


ISR(INT4_vect){ // ISR = SIGNAL, same
	cli();	

	speed += 50 ;
	_delay_ms(1);
	sei();
}
ISR(INT5_vect){ // ISR = SIGNAL, same
	cli();

	

	speed -= 50 ;
	(speed <= 0 && speed = 1);

	_delay_ms(1);
	sei();
}




ISR(TIMER0_OVF_vect)
{
	TCNT0=0x06;

	if(++tick_cnt >= speed){
		tick_cnt=0;
		PORTA ^= 0xFF;
	}
}


int settings(){
	
	/////////////  FND
	DDRC 		= 0xFF ;
	DDRG 		= 0x00 ;
	
	///////////// INT Switch 4 + 5
	DDRE 		= 0xCF ;
	
	/* Interrupt Enable + Mask */
	EIMSK 		= 0x10 ;
	EICRB 		= 0x02 ;
	
	
	DDRA=0xFF;
	TCNT0=0x06;
	TCCR0 = 4;
	TIMSK=1;

	return 0;
}


int main() {

	
	sei( );
	while(1) {
		
		

	}
}

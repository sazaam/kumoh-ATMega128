
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000
#include <util/delay.h>


#define OFF 0
#define ON 1


volatile int FLAG = OFF ;




SIGNAL(INT4_vect){

	cli();
	
	PORTA = (FLAG == OFF) ? 0xFF : 0x00 ;
	
	FLAG = !FLAG ;
	_delay_ms(100);
	sei();
}


int main(){
	DDRA = 0xFF ;
	PORTA = 0x00 ;
	DDRE = 0xEF ;
	EICRA = 0x00 ;
	EICRB = 0x02 ;
	EIMSK = 0x10 ;
	sei();
	while(1);

}

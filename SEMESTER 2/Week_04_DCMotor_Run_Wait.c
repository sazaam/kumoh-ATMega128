#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define M_CW 0x80
#define M_CCW 0x40
#define M_ST_1 0x00
#define M_ST_2 0xC0

void delay(int sec){

	int dur = 1000 * sec ;

	for(int i = 0 ; i < dur ; i++){
		_delay_ms(1) ;
	}
	
}


int ON = 1 ;

int main(void){
	
	DDRB = 0xFF ;
	
	while(ON){
		
		PORTB = M_CW ;
		delay(3) ;

		PORTB = M_ST_1 ;
		delay(3) ;

		PORTB = M_CCW ;		
		delay(3) ;

		PORTB = M_ST_2 ;
		delay(3) ;

	}

}



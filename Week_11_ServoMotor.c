#include <avr/io.h>

#define F_CPU 16000000UL

#include <util/delay.h>

#define MOTOR_CW	0x80	// 10
#define MOTOR_CCW	0x40	// 01
#define MOTOR_STOP1 0x00	// 00
#define MOTOR_STOP2 0xC0	// 11


void delay_sec(int sec){

	for(int i = 0 ; i < 1000 * sec ; i++){
		_delay_ms(1) ;
	}


}

int main(){
	
	DDRB = 0xFF ;

	while(1){
		PORTB = MOTOR_CW ;
		_delay_ms(3) ;
		PORTB = MOTOR_STOP1 ;
		_delay_ms(3) ;
		PORTB = MOTOR_CCW ;
		_delay_ms(3) ;
		PORTB = MOTOR_STOP2 ;
		_delay_ms(3) ;

	}

}

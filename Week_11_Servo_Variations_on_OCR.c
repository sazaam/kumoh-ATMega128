#include <avr/io.h>

#define F_CPU 16000000 UL

#include <util/delay.h>

#define LO		128
#define MID		77
#define HI 		26

volatile unsigned int ct = 0 ;


ISR(TIMER2_COMP_VECT){
	ct ++;
}

void motor(int sec, int speed){
	
	OCR2 = speed ;
	TCNT2 = 0 ;
	PORTA = speed ;

	while(ct != 1000 * sec) ;
	ct = 0;


}

int main(){
	
	DDRA = 0xFF ;
	DDRB = 0xFF ;

	TCCR2 = 0x7B ;	
	TIMSK = 0x80 ;
	sei() ;

	motor(LO) ;
	motor(LO) ;
	motor(LO) ;
	
	TCCR2 = 0x01;
	PORTA = 0x00 ;

}

#include <avr/io.h>

#define F_CPU 16000000 UL

#include <avr/interrupt.h>

volatile unsigned int ct = 0 ;


ISR(TIMER2_COMP_vect){
	ct ++;
}

void motor(int sec, int duty_cycle){
	

	255 / 10 * 1 = 26;
	255 / 10 * 2 = 51;
	

	int speed = (255 / 10 * duty_cycle) ;

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
	
	

	for(int n_duty = 0 ; n_duty < 10 ; ++n_duty){
		
		motor(5, n_duty) ;
			
	}

	TCCR2 = 0x01;
	PORTA = 0x00 ;

}

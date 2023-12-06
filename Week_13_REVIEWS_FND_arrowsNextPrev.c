#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>

volatile unsigned int tick_cnt=0;
volatile unsigned int speed=700;

int fnd[2] = {112, 70} ;


#define MAX 4
#define UP 1
#define DOWN 0
#define IDLE 2

volatile unsigned int ind = -1;
volatile unsigned int way = IDLE , lastway = IDLE ;
volatile unsigned already = -1 ;

int test = 0x01 ;

//////////////////////////  SWITCHES
ISR(INT4_vect){ 
	cli();

	way = way == DOWN ? IDLE : DOWN ;
	_delay_ms(1);
	
	
	sei();
}
ISR(INT5_vect){ 
	cli();
	way = way == UP ? IDLE : UP ;	
		
	_delay_ms(1);

	sei();
}
////////////////////////// END SWITCHES



////////////////// DIRECTION

void changeindex(int n){ ind = (MAX + (ind + n)) % MAX ; } ;
void upindex() { changeindex(1) ; };
void downindex() { changeindex(-1) ; };

////////////////// END DIRECTION



////////////////// TIMER INTERRUPT
ISR(TIMER0_OVF_vect)
{
	TCNT0=0x06;
	
	if(++tick_cnt >= speed){
		tick_cnt=0;

		
		// if enabled
		if(way != IDLE){
			// update index
			way == UP ? upindex() : downindex() ;
			lastway = way ;
		}

	}
	
	
}
////////////////// END TIMER INTERRUPT



void displayArrows(){
	
	
	for(int i = 0 ; i < MAX ; i++){
	
		if(i == ind) display(i, fnd[lastway]) ;
		else display(i, 64);
	
	}
}




void display(int n, int d){
	
	PORTG = 0x01 << n ;
	PORTC = d ;

}



////////////////// SETTINGS
int settings(){

	DDRA=0xFF;

	/////////////  FND
	DDRC 		= 0xFF ;
	DDRG 		= 0x00 ;
	
	///////////// INT Switch 4 + 5
	DDRE 		= 0xCF ;
	
	/* Interrupt Enable + Mask */
	EIMSK 		= 0x30 ;
	EICRB 		= 0x0B ;
	
	
	TCNT0=0x06;
	TCCR0 = 4;
	TIMSK=1;
	
	sei( );
	
	return 0;
}
////////////////// END SETTINGS


int main() {
	
	settings() ;
	while(1){
		displayArrows() ;
	};


}

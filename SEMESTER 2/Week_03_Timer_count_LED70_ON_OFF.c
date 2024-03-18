#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>






//////// SETTINGS


///////////// BLINKING LEDS
void initLED(){

	DDRA = 0xFF ;
	PORTA = 0b10000001 ;

}
///////////// END BLINKING LEDS




///////////// FND
#define FND_LEN 4
unsigned char digit[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};
volatile unsigned int fnd[4];
int ON = 1 ; // ON Variable to turn off infinit rendering loop

void initFND(){
	
	DDRC = 0xFF ;
	DDRG = 0x0F ;

}
///////////// END FND




///////////// TIMER
void initTIMER(){

	TCNT0 = 0x06 ;
	
	TCCR0 = 4;
	TIMSK = 1;

}

///////////// END TIMER

//////// END SETTINGS






///////////// LED TOGGLING
volatile int tick = 0, ct = 0;

int LED_seq[6] = {
	0b10000001, 
	0b00000001,
	0b10000001,
	0b00000000,
	0b10000000,
	0b00000000,
} ;

void LED_Toggle(){
	
	ct = ct % 6 ;
	
	

	if(++tick >= 500){
		
		tick = 0 ;
		
		
				
		//PORTA ^= 0xFF ;
		ct++ ;
		PORTA = LED_seq[ct] ;


	}

}
///////////// END LED TOGGLING


void fnd_arr(unsigned int arr[], int n){
	
	int one = (PORTA & 0b10000000) > 0 ;
	arr[3] = one ;
	
	int two = 0 ;
	arr[2] = two ;
	
	int res = 0 ;
	arr[1] = 0;

	arr[0] = (PORTA & 0b00000001) > 0 ;

	return 0 ;
}


void FND_Count(){
	fnd_arr(fnd, ct) ;
}



//////// INTERRUPT TIMER
ISR(TIMER0_OVF_vect){

	
	TCNT0 =  0x06;
	LED_Toggle() ;
	FND_Count() ;

}

//////// END INTERRUPT TIMER





displayFND(){

	
		/////// FND DISPLAY
		int max = FND_LEN - 1 ;
		
		
		for(int l = max ; l >= 0 ; --l){
		
			PORTC = digit[fnd[l]] ;
			PORTG = 0x01 << l;

			if(l % 2){
				_delay_ms(2) ;
			}else{
				_delay_ms(3) ;
			}
		}
		_delay_ms(1) ;
}


int main(){
	
	initLED() ;
	initFND() ;

	initTIMER() ;
	
	sei() ;
	

	

	while(1){
		
		displayFND() ;
		
	}

	return 0;
}

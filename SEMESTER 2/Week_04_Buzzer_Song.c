#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>


unsigned char digit[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};
unsigned char fnd[4];


enum Note { DO, RE, MI, FA, SOL, LA, SI, DDO, SLEEP};
char f_table[9] = {17, 43, 66, 77, 97, 114, 130, 137, 255};


#define MAX 26
#define EOS -1
unsigned char song[MAX] = {SOL, MI, MI, SOL, MI, DO, RE, MI, RE, DO, MI, SOL, DDO, SOL, DDO, SOL, DDO, SOL, MI, SOL, RE, FA, MI, RE, DO, EOS};

#define ON 0
#define OFF 1

volatile int state = OFF, note = 0 ;



/// If state ON → OFF, buzzer OFF, else (OFF) → ON, buzzer ON

////////////////////////// BUZZ TIMER
ISR(TIMER0_OVF_vect) {
	
	if (state == ON) {
		
		//DDRB = 0x00;
		PORTB = 0x00;
		
		state = OFF;
	} else {
		//DDRB = 0x10;
		
		PORTB = 0x10;
		state = ON;
	}
	TCNT0 = f_table[note];

}
////////////////////////// END BUZZ TIMER



void initLED(){
	
	DDRA = 0xFF ;

}

void initFND(){
	
	DDRC = 0xFF;
	DDRG = 0x0F;
	
}

void startBUZZER(int cond){
	
	DDRB = 0x00;
	PORTB = 0x00;
	TCCR0 = cond ? 0x03 : 0x00 ; // 32 unit blocks
	TIMSK = cond ? 0x01 : 0x00; // choose Overflow	
}


int main(){
	
	initLED();
	initFND();

	//startBUZZER(1);
	
	sei() ;

	int pattern = 0xFF ;
	

	unsigned int cnt = 0;

	while(++cnt){
		
		
		

		////////// CALCULATIONS
		

		cnt = cnt % (MAX + 1) ;
		
		if(cnt == 0) {
			startBUZZER(1);
			//sei() ;
		}		
		

		if(cnt >= (MAX - 1)) { // In order to have a pause 1 seconds before the song restarts
			PORTA = 0x0 ;
			PORTG = 0x0 ;
			
			startBUZZER(0);

			_delay_ms(1000);

			continue ;
		}
		
		note = song[cnt] ;
		
		///////// LED
		pattern = (0x01 << (7 - note)) ;		
		PORTA = pattern ;
		///////// END LED 
		

		///////// FND
		// switching digit display
		int s = 0x01;

		// finding right digit in bytelist
		int d = digit[note + 1] ;
		
		PORTC = d ;  
		PORTG = s ;
		///////// END FND

		_delay_ms(500);		

	};

   



}

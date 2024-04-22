#include <avr/io.h>

#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>



/*

UltraSonic Sensor

*/

///////////////////////////////////// SYSTEM DEFINITIONS

#define TRIG 			2
#define ECHO 			3

#define FND_DB_DDR		DDRC
#define FND_DB_PORT		PORTC

#define FND_CT_DDR		DDRG
#define FND_CT_PORT		PORTG

#define SONIC_DDR		DDRB
#define SONIC_PORT		PORTB
#define SONIC_PIN		PINB

#define TRIG_HIGH		(1 << TRIG)
#define TRIG_LOW		~(1 << TRIG)
#define ECHO_HIGH		(1 << ECHO)
#define ECHO_LOW		~(1 << ECHO)

///////////////////////////////////// END SYSTEM DEFINITIONS



unsigned int ON = 1 ; // ON Variable to turn off infinite rendering loop
unsigned int dist ;



///////////// FND
#define FND_LEN 4
unsigned char digit[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};
volatile unsigned int fnd[4];



void fnd_arr(volatile unsigned int arr[], int n){
	
	arr[3] = (n / 1000) % 10 ;
	arr[2] = (n / 100) % 10 ;
	arr[1] = (n / 10) % 10 ;
	arr[0] = (n / 1) % 10 ;
}



void FND_Count(){
	fnd_arr(fnd, dist) ;
}



void displayFND(){

	int max = FND_LEN - 1 ;
	
	
	for(int l = max ; l >= 0 ; --l){
	
		FND_DB_PORT = digit[fnd[l]] ;
		FND_CT_PORT = 0x01 << l;

		if(l % 2){
			_delay_ms(2) ;
		}else{
			_delay_ms(3) ;
		}
	}
	_delay_ms(1) ;
}







//////// SETTINGS


void initFND(){
	
	FND_DB_DDR = 0xFF ;
	FND_CT_DDR = 0x0F ;

}
///////////// END FND


///////////// LEDS
void initLED(){

	DDRA = 0xFF ;

}
///////////// END LEDS


void initSonic(){
	
	SONIC_DDR |= (TRIG_HIGH & ECHO_LOW) ;

}



//////// END SETTINGS



void settings(){
	initLED() ;
	initFND() ;
	initSonic();

}

int main(){
	
	settings() ;
	
	while(ON){

		SONIC_PORT &= ~TRIG_HIGH ; // set to LOW
		_delay_us(10) ;
		
		SONIC_PORT |= TRIG_HIGH ; // set to HIGH
		_delay_us(10) ;

		SONIC_PORT &= ~TRIG_HIGH ; // set back to LOW
		_delay_us(10) ;
		

		TCCR1B = 0x02 ;


		while(!(SONIC_PIN & ECHO_HIGH)) ; // wait until ECHO gets to HIGH
		TCNT1 = 0x0000 ;

		while(SONIC_PIN & ECHO_HIGH) ; // wait until ECHO gets to LOW again
		TCCR1B = 0x00 ;
		

		dist = (unsigned int) (TCNT1 / 2 / 58) ;
		
		FND_Count() ;
		displayFND() ;
	}

	return 0;
}

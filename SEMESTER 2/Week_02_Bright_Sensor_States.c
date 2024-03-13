#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

/* STD IO printf Implementation */
//////////////////////////////////////////////// PRINTF
#include <stdio.h>

static int putchar0(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(putchar0, NULL, _FDEV_SETUP_WRITE);

volatile unsigned char c=0;	
int putchar0(char c, FILE *stream){
	if(c == '\n') putchar0('\r',stream);
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
	return 0;
}
//////////////////////////////////////////////// END PRINTF



/*

Extra : FND Display of the sensor Value

Today I wanted to see if I could also display the value in the FND leds,
So ended up putting the display function in the ADC Interrupt, 
and transform _delay_ms(500) into 
a more fancy loop calculation (10ms * 50)
Since I need more frequency for the display loop.

*/






//////// SETTINGS


///////////// BLINKING LEDS
void initLED(){

	DDRA = 0xFF ;

}
///////////// END BLINKING LEDS


///////////// SERIAL CONNECTIVITY
void initUART(){

	
	UBRR0H=0;
	UBRR0L=8;  // 16MHz, 115200 baud
	UCSR0B=0x98; // Rx Complete Interrupt Enable, RX/TX Enable
	UCSR0C=0x06; // UART Mode, 8 bit data, No parity, 1 Stop bit
	stdout = &mystdout;
	sei();

}
///////////// END SERIAL CONNECTIVITY


///////////// ADC 
void initADC(){
	
	ADMUX = 0x00 ;
	ADCSRA = 0x8F ;

}
///////////// END ADC 



///////////// FND

unsigned char digit[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};
volatile unsigned int fnd[4];
int ON = 1 ; // ON Variable to turn off infinit rendering loop

void initFND(){
	
	DDRC = 0xFF ;
	DDRG = 0x0F ;

}
///////////// END FND

//////// END SETTINGS


void fnd_arr(unsigned int arr[], int n){
	double l = 3 ;
	//while(l--){
		//arr[(int) l] = (int) (n / pow(10, l)) % 10 ; // replace multiple lines with calculations through powers of 10 => 10^3 = 1000 , 10^0 = 1
		// But beware !! The arguments and the output of pow() are of type DOUBLE !!!
	//}

	// 0~1023
	
	arr[3] = (n / 1000) % 10 ;
	arr[2] = (n / 100) % 10 ;
	arr[1] = (n / 10) % 10 ;
	arr[0] = (n / 1) % 10 ;

	return 0 ;
}




unsigned short adc_lo = 0 ;
unsigned short adc_hi = 0 ;



void printADC(unsigned short val, int state, int diff){
	
	switch(state){
	
		case 0 :
			printf("Stable = %d, (diff = %d)\n", val, diff) ;
		break;
		case 1 :
			printf("Darker = %d, (diff = %d)\n", val, diff) ;
		break;
		case 2 :
			printf("Brighter = %d, (diff = %d)\n", val, diff) ;
		break;
	}
}


void showADC(unsigned short val){
	
	PORTA = val / 4;
}

volatile unsigned short prev ;




#define FND_LEN 4



//////// INTERRUPT 4 ADC
ISR(ADC_vect){

	cli() ;
	
	
	
	
	
	
		
	/// FND	
	int max = FND_LEN - 1 ;
	
	
	// Final Loop in displaying FND will take ~10ms so multiply by 50 times in order to have an approx 500ms timespan between displays
	for(int i = 0 ; i < 50 ; i++){
		if (i == 0) {
			

			//////////////////////////// Sensor Incoming Value Handling
			adc_lo = ADCL ;
			adc_hi = ADCH ;
	
			unsigned short n = (adc_hi << 8 ) | adc_lo ;
				/*
		
		
				Implemented a Safe Margin State Handler
				Within a 50 units range, Brighter or Darker will not trigger 
				but will remain under Stable state.
		
				*/
			int margin = 50 ;
			int state = 0 ;
			if(n >= prev + margin){
				state = 2 ;
				prev = n ;
			}else if(n <= prev - margin){
				state = 1 ;
				prev = n ;
			}
	
			///// LED Debug display
			showADC(n) ;

			///// Send into Serial Print Interface
			printADC(n, state, n - (int)prev) ;
			
			

			////// Write value into fnd Array
			fnd_arr(fnd, n) ;
		}
		

		/////// FND DISPLAY

		int l = max ;
		for(; l >= 0 ; --l){
		
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
	

	sei() ;
}





int main(){
	
	initLED() ;
	initUART() ;
	initADC() ;
	
	initFND() ;

	sei() ;
	

	

	while(1){
		// Reset SENSOR
		ADCSRA |= (1 << ADSC) ;

	}

	return 0;
}

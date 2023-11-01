#include <avr/io.h>


#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>


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




//////// SETTINGS

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

//////// END SETTINGS




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

//////// INTERRUPT 4 ADC
ISR(ADC_vect){

	cli() ;
	
	adc_lo = ADCL ;
	adc_hi = ADCH ;
	
	unsigned short n = (adc_hi << 8 ) | adc_lo ;
	
	int margin = 50 ;
	int state = 0 ;
	if(n >= prev + margin){
		state = 2 ;
		prev = n ;
	}else if(n <= prev - margin){
		state = 1 ;
		prev = n ;
	}
	
	
	//showADC(n) ;
	printADC(n, state, n - (int)prev) ;
	
	_delay_ms(500) ;
	
	sei() ;
}




int main(){
	
	DDRA = 0xFF ;


	initUART() ;
	initADC() ;

	sei() ;

	while(1){
		ADCSRA |= (1 << ADSC) ;
	}

	return 0;
}

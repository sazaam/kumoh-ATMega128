



#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000



///////////////////////////////////////// SERIAL COMM
#define NULL 0
static int putchar0(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(putchar0, NULL, _FDEV_SETUP_WRITE);
int putchar0(char c, FILE *stream){
	if(c == '\n') putchar0('\r',stream);
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
	return 0;
}
///////////////////////////////////////// END SERIAL COMM

#define INTERVAL 0x06 // 500ms


unsigned short readADC();
unsigned short adc_lo = 0 ;
unsigned short adc_hi = 0 ;
unsigned short value ;


////////////// TIMER

ISR(TIMER0_OVF_vect){

	TCNT0 = 0x06 ; 
	
	float r = (float)readADC() / 1023 ; 

	int n = ( r * 6 ) + 2 ;
	
	gugu(n) ;

}


void init_timer(){
	TCNT0 = 0x06 ;
	TCCR0 = 4 ;
	TIMSK = 1 ;
}
////////////// END TIMER

//////////////////////////////////// GUGU
int ct = 0 ;
int incr(){
	return ((ct = ct + 1) % 9 ) + 1 ;
}

int t = 0 ;
void gugu(int n){
	
	if(++t >= 500){
		int m = incr() ;
		printf("%d x %d = %d \n", n, m, n*m ) ;
		t = 0 ;
	}
	

}

///////////// ADC 
unsigned short readADC(){
	
	ADCSRA |= 0x40 ;
	while((ADCSRA & 0x10) != 0x10) ;
	adc_lo = ADCL ;
	adc_hi = ADCH ;
	value = (adc_hi << 8) | adc_lo ;
	return value ;
}

void init_adc(){

	ADMUX = 0x00 ;
	ADCSRA = 0x87 ;

}

///////////// END ADC
 

////////////// UART
void init_uart(){

	UBRR0H = 0;
	UBRR0L = 8; // 16MHz, 115200 Baud
	UCSR0B = 0x18; // use RXC Interrupt Rx & Tx enabled
	UCSR0C = 0x06; //UART Mode 8Bit , No Parity, Stop 1 bit

}
////////////// END UART



int settings(){
	
	stdout = &mystdout;

	init_adc() ;
	
	init_uart() ;
	
	init_timer() ;
	
	sei() ;


    return 0;
	
}


int main(){
	
	settings() ;
	
	printf("Welcome to the Gugudan Calculations !\n") ;

    while(1){		

    }
}







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



void printADC(int ct){
	PORTA = 0xFF ;
	if(ct > 0 ){
		printf("%d\n", ct) ;
	}else {
		printf("stopped...\n") ;
	}

}




#define LIMIT 871
#define INTERVAL 5000

volatile unsigned int stopped = 0 ;
unsigned short adc_lo = 0 ;
unsigned short adc_hi = 0 ;


//////// INTERRUPT 4 ADC
ISR(ADC_vect){

	cli() ;
	
	adc_lo = ADCL ;
	adc_hi = ADCH ;
	
	unsigned short n = (adc_hi << 8 ) | adc_lo ;
	
	
	stopped = n < LIMIT ;
	
	if(stopped){
		printADC(-1) ;
		UCSR0B = 0x00 ;
	} else { 
		UCSR0B = 0x18 ;
		printADC(n) ;
	}
	
	_delay_ms(INTERVAL) ;
	
	
	sei() ;
	
}





///////////// ADC 
void initADC(){
	
	ADMUX = 0x00 ;
	ADCSRA = 0x8F ;

}

void init_uart(){

	UBRR0H = 0;
	UBRR0L = 8; // 16MHz, 115200 Baud
	UCSR0B = 0x18; // use RXC Interrupt Rx & Tx enabled
	UCSR0C = 0x06; //UART Mode 8Bit , No Parity, Stop 1 bit

}


///////////// END ADC 

int settings(){
	

	stdout = &mystdout;

	initADC() ;
	init_uart() ;

	/* LED */
	DDRA 				= 0xFF ;
    DDRC 				= 0xCF ;

	/* FND */
	DDRC = 0xFF;
	DDRG = 0x0F;

	
	sei() ;


    return 0;
	
}


int main(){
	
	settings() ;
	
	printf("Welcome to the Sensor Program !\n") ;

    while(1){

		ADCSRA |= (1 << ADSC) ;
		_delay_ms(INTERVAL) ;
    }
}


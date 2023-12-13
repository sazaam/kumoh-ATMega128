#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char digit[4] = {0x76,0x79,0x38,0x73};
unsigned char fnd_sel[4] = {0x01,0x02,0x04,0x08};

static int putchar0(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(putchar0, NULL, _FDEV_SETUP_WRITE);





int putchar0(char c, FILE *stream)
{

	if(c == '\n')
		putchar0('\r',stream);

	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
	return 0;
}

volatile int dab = -1 ;
volatile int ans = 0 ;
volatile int numwins = 0 ;
volatile unsigned char c ;
/////////////////////// END HANDLING EXITING
int exiting(){
	printf("\n\t You are too good at this. Please stop... \n") ;
	
	UCSR0B = 0x0; 
	return 1 ;
}

/////////////////////// PROGRAM LOGIC

int checkCharNum(unsigned char c){ // returns 1 if anything other than space
    int max = 9 ;
	return (c > 0 && c <= max) ;
}

void erase(){
	dab = -1 ;
}

void save(unsigned char c){
	c = c - '0';
	dab = c ;
}

////////////////////////////////////////// ADC READING
void init_adc(){
	ADMUX = 0x00;
	ADCSRA = 0x87;
}

unsigned short read_adc(){

	unsigned char adc_lo, adc_hi;
	unsigned short value ;
	ADCSRA |= (1 << ADSC) ;
	
	while((ADCSRA & ( 1 << ADIF)) == 0);
	adc_lo = ADCL ;
	adc_hi = ADCH ;
	value = (adc_hi << 8)| adc_lo ;
	return value;
}
/////////////////////////////////////////// END ADC READING


void reset(){
	

	srand(read_adc()) ;

	dab = -1 ;
	
	ans = rand() % 10 ;
	printf("\nRandom Number : %d", ans) ;
	
    printf("\nTry to guess: (0~9)") ;
}

int evaluate(){
	int res = 0 ;
	if(numwins >= 2) return exiting();
	if(checkCharNum(dab)){
		if(dab == ans){
			res = 1 ;
			printf("\n >> Correct!") ;
		}else
			printf("\n >> Wrong!") ;	
	}else{
		printf("\n Please input a digit 0~9") ;
	}
	return res ;

}
/////////////////////// END PROGRAM LOGIC



ISR(USART0_RX_vect){
    cli();
    c = UDR0;
	
	switch(c){
		case '\r': 
			if(evaluate()) {
				numwins ++ ;
				reset() ; 
			}				
			break ;
		case 127: // ERASING
			printf("%c", c) ; 
			erase() ; 
			break ;
		default:
			printf("%c", c) ; 
			save(c) ; 
			break ;
    }

    sei();
}



/*
           7        6       5       4       3       2       1       0            
        RXCIEn   TXCIEn  UDRIEn   RXENn   TXENn   UCSZn2  RXB8n   TXB8n
           1        0       0       1       1       0       0       0

        here = 0x98 (= 128 + 8 + 16 = 152 )
*/


void init_uart()
{
    DDRA = 0xff;
	UBRR0H = 0;
	UBRR0L = 8; // 16MHz, 115200 Baud

	
	UCSR0B = 0x98; // use RXC Interrupt Rx & Tx enabled


	UCSR0C = 0x06; //UART Mode 8Bit , No Parity, Stop 1 bit
    sei(); // enable global interrupt
}



void settings(){

	init_uart() ;
	init_adc() ;

	

    stdout = &mystdout;

}
int main()
{
    settings(); 


	

	reset() ;

    while(1)
	{
		//
	}

}

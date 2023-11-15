#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define F_CPU 16000000UL
#include <util/delay.h>

/* DEBUG */
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





#define ON 0
#define OFF 1

enum Note { DO, RE, MI, FA, SOL, LA, SI, DDO, SLEEP};
char f_table[9] = {17, 43, 66, 77, 97, 114, 130, 137, 255};

volatile int state=OFF, tone=0;


////////////////////////// Buzz
ISR(TIMER0_OVF_vect) {
	
	if (state == ON) {
		PORTB = 0x00;
		state = OFF;
	} else {
		PORTB = 0x10;
		state = ON;
	}
	TCNT0 = f_table[tone];

}




////////////////////////// Key Press
volatile unsigned char c;

ISR(USART0_RX_vect){
	
	// Retrieve entered letter
	c = UDR0;
    printf("%c\n", c) ;
	

	switch(c){
		case 'a':
		case 'A':
			tone = DO ;
		break;
		case 's':
		case 'S':
			tone = RE ;
		break;
		case 'd':
		case 'D':
			tone = MI ;
		break;
		case 'f':
		case 'F':
			tone = FA ;
		break;
		case 'g':
		case 'G':
			tone = SOL ;
		break;
		case 'h':
		case 'H':
			tone = LA ;
		break;
		case 'j':
		case 'J':
			tone = SI ;
		break;
		case 'k':
		case 'K':
			tone = DDO ;
		break;
		default:
			tone = SLEEP ;
		break;	
	}
	PORTA = 0x01 << tone ;
	TIMSK = tone == SLEEP ? 0x00 : 0x01 ; 
}



////////////////////////// SETTINGS
void init_uart(){
	
	UBRR0H = 0;
	UBRR0L = 8; //16Mhz, 115200baud의 경우
	UCSR0B = 0x98; //RXC interrupt, Receive(Rx) Enable, Transmit(Tx) Enable
	UCSR0C = 0x06; //UART Mode, 8 Bi t Data, No Parity,1 Stop Bit

}


////////////////////////// MAIN
int main() {
	
	DDRA = 0xFF;
    stdout = &mystdout;


	DDRB = 0x10;
	TCCR0 = 0x03;// Normal Mode, 32분주
	
	init_uart();
	sei();
	while(1){
		///
	}

	return 0;
}

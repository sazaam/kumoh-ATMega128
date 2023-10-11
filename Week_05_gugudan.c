#include <avr/io.h>
#include <stdio.h>


#define NULL 0

static int putchar0( char c , FILE *stream ) ;

static FILE mystdout = FDEV_SETUP_STREAM(putchar0, NULL, _FDEV_SETUP_WRITE);






char nl = '\n', cr = '\r' ;


int putchar0( char c , FILE *stream ){

	
	if(c == nl){ putchar0(cr, stream); }

	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = c ;


	return 0;
}


char getchar0(){

	while(!(UCSR0A & (1 << RXC0)));
	return (UDR0) ;

}


void init_uart(){

	//////// Settings

	UBRR0H = 0;
	UBRR0L = 8;

	UCSR0B = 0x18;
	UCSR0C = 0x06;

	////////

}

int main(){
	
	char prompt[] = "JKIT-128-1> ";
	char *ptr;
	char c;

	//////// Settings

	init_uart();

	////////


	char c; int i;
	
	stdout = &mystdout;

	while(1){
		c = getchar0() ;

		putchar0(c);
		
		if(c == '\r'){
			putchar0('\n');

			ptr = prompt;
			puts0(ptr);
		}


	}
	

}

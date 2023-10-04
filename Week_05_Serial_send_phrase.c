#include <avr/io.h>
#define NULL 0

void putchar0(char c){
	
	// while(!(UCSR0A & 0x20)) ;

	while(!(UCSR0A & (1 << UDRE0))) ;
	
	UDR0 = c ;

}


int main(){
	




	int i ;
	char data[] = "Hi~\n\r";


	UBRR0H = 0 ; // means 12 bit
	UBRR0L = 8 ; // Refer to ATmega128 Datasheet, 16MHZ, 115200 Baud
	
	UCSR0B = 0x08 ; // Enabling Transmit (TX)
	UCSR0C = 0x06 ; // UART Mode, 8-bit Data, No Parity, 1 Stop Bit
	


	while(1){
		i = 0 ;

		while(data[i]!= NULL){ // Append (Send) Character until there is none left.
			putchar0(data[i++]);
		}
	
	}

}

#include <avr/io.h>
#define NULL 0

void putchar0(char c){
	
	// while(!(UCSR0A & 0x20)) ;

	while(!(UCSR0A & (1 << UDRE0))) ;
	
	UDR0 = c ;

}

/*
helpers JS console :

function h2b(hex){
    return ("00000000" + (parseInt(hex, 16)).toString(2)).substr(-8);
}
// todos 
// find other transdatas
*/


int main(){
	


/*

	// Example 1
 	
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

*/
	

	int i ;
	char data[] = "Seize the day!\n\r";

	
	// means 12 bit

	UBRR0H = 0 ; 
	UBRR0L = 103 ; // Refer to ATmega128 Datasheet, 16MHZ, 9600 Baud
	
	UCSR0B = 0x08 ; // Enabling Transmit (TX)
	UCSR0C = 0x2E ; // UART Mode, 8-bit Data, Even Parity, 2 Stop Bit
	


	while(1){
		i = 0 ;

		while(data[i]!= NULL){ // Append (Send) Character until there is none left.
			putchar0(data[i++]);
		}
	
	}

	

}

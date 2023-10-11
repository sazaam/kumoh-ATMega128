


#include <avr/io.h>



#define NULL 0



void putchar0( char c ){



	while(!(UCSR0A && (1 << UDRE0)));

	UDR0 = c ;



}





char getchar0(){



	while(!(UCSR0A && (1 << RXC0)));

	return (UDR0) ;



}



void puts0( char *ptr){


	while(*ptr != NULL){

		putchar0(*ptr++);

	}



}







int main(){


	char prompt[] = "JKIT-128-1> ";

	char *ptr;

	char c;



	//////// Settings



	UBRR0H = 0;

	UBRR0L = 8;



	UCSR0B = 0x18;

	UCSR0C = 0x06;



	////////





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

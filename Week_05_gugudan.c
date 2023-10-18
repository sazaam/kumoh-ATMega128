


/// INCLUDES

#include <avr/io.h>
#include <stdio.h>



/// PRINTF Implementation
char nl = '\n', cr = '\r' ;
static int putchar0( char c , FILE *stream ) ;
static FILE realstdout = FDEV_SETUP_STREAM(putchar0, NULL, _FDEV_SETUP_WRITE);

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
/// END PRINTF Implementation




//////// SETTINGS
void init_uart(){
	UBRR0H = 0;
	UBRR0L = 8;

	UCSR0B = 0x18;
	UCSR0C = 0x06;
}
//////// END SETTINGS

// check



int main(){
	
    char c; int i = 0, max = 10;
	//////// Settings
	init_uart();
    //////// Printf Localizing
    stdout = &realstdout;
	////////


	
	
	

	while(1){
        printf("Number for GUGUDAN : ");
		c = getchar0() ;
        printf("%c\n", c);
		
        c = c - 0;
		
		if(c > 1 && c < max){
			for(int i = 0 ; i < max; i++){
                printf("\t%d x %d = %d\n", c, i, c*i);
            }
		}


	}
	

}

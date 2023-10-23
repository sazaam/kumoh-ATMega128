#include <avr/io.h>
#include <stdio.h>
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
unsigned char digit[4] = {0x76,0x79,0x38,0x73};
unsigned char fnd_sel[4] = {0x01,0x02,0x04,0x08};

static int putchar0(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(putchar0, NULL, _FDEV_SETUP_WRITE);



volatile unsigned char c, oldc;
volatile unsigned char cta = 0, ctA = 0, cto = 0;

int putchar0(char c, FILE *stream)
{

	if(c == '\n')
		putchar0('\r',stream);

	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
	return 0;
}


char getchar0()
{
	while (!(UCSR0A & (1<<RXC0)));
	return(UDR0);
}

int check(unsigned char c){

    return 
        (c >= 'a' && c <= 'z') ? 1 :
        (c >= 'A' && c <= 'Z') ? 2 : 3 ;

}

ISR(USART0_RX_vect){
    cli();
    c = UDR0;
    int n = 0;
    if(c=='\r'){
        printf("\n[a-z] : %d\n", cta) ;
        printf("\n[A-Z] : %d\n", ctA) ;
        printf("\n[^a-Z] : %d\n", cto) ;
        cta = ctA = cto = 0;
        printf("\nPlease Input a Sentence :") ;
        
    } else {
        printf("%c", c) ;
        if(c == 127){
            // basckspace case
            n = check(oldc) ;
            if(n == 1) cta-- ;
            if(n == 2) ctA-- ;
            else cto--;
        }else{
            n = check(c) ;
            if(n == 1) cta++ ;
            if(n == 2) ctA++ ;
            else cto++;
        }
        oldc = c ;
        
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
    stdout = &mystdout;

}
int main()
{
    settings(); 

	/* Putty Settings : 
		COM3
		Data Bit 8
		Unit Bit 1
		Parity None
		Last None
	*/

	printf("\nPlease Input a Sentence :") ;    

    while(1)
	{
		//
	}

    
}

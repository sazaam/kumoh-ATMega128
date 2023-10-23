#include <avr/io.h>
#include <stdio.h>
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

/* // NOT REALLY NEEDED HERE
char getchar0()
{
	while (!(UCSR0A & (1<<RXC0)));
	return(UDR0);
}
*/



ISR(USART0_RX_vect){
    c = UDR0;
    printf("%c\n", c) ;
}


/*
           7        6       5       4       3       2       1       0            
        RXCIEn   TXCIEn  UDRIEn   RXENn   TXENn   UCSZn2  RXB8n   TXB8n
           1        0       0       1       1       0       0       0

        here = 0x98 (= 128 + 8 + 16 = 152 )
*/


void init_uart()
{
	UBRR0H = 0;
	UBRR0L = 8;

	UCSR0B = 0x18;
	UCSR0B = 0x98;


	UCSR0C = 0x06;
    sei(); // enable global interrupt
}

void settings(){
    init_uart() ;
	
    
    DDRA = 0xff;
    stdout = &mystdout;

}
int main()
{
    settings();
    
    printf("KIT Test\n");
    printf("1. All LED => ON \n");
    printf("2. All LED => OFF \n");
    printf("3. Exit\n");
	
    while(1)
	{
		// here c is known by USART0_RX_vect Interrupt
		//printf("Select : %c",c);
		
		// And this code (similar) could go in the ISR function codeblock
		if(c == '1'){			
			PORTA = 0xff;
			c = 0;
		} else if(c == '2'){
			PORTA = 0x00;
            c = 0;
			_delay_ms(3000);
		} else if(c == '3'){	
            printf("3. Exiting...\n");
			UCSR0B = 0x18 ;
            return ;
		}
	}

    
}

#include <avr/io.h>
#include <stdio.h>
#define F_CPU 16000000UL
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

char getchar0()
{
	while (!(UCSR0A & (1<<RXC0)));
	return(UDR0);
}




ISR(USART0_RX_vect){


}





void init_uart()
{
	UBRR0H = 0;
	UBRR0L = 8;

	UCSR0B = 0x18;
	UCSR0B = 0x98 // = 152 = 0x98 = 128 + 16 + 8 = 128 + 24 = 0b10011000


	UCSR0C = 0x06;
}


int main()
{

	/*
	
	
	*/
	sei();

	/*
	DDRA = 0xff;
	DDRC = 0xff;
	DDRG = 0x0f;

	int i,j;
	char c; 
	init_uart();
	stdout = &mystdout;
	while(1)
	{
		printf("KIT Test\n");
		printf("1. All LED => ON \n");
		printf("2. All LED => OFF \n");
		printf("3. Exit\n");
		printf("Select : %c",c);
		c = getchar0( );
		
		
		if(c == '1'){			
			PORTA = 0xff;
			_delay_ms(3000);
		} else if(c == '2'){
			PORTA = 0x00;	
			_delay_ms(3000);
		} else if(c == '3'){	
			/// heve to do here
		}
	}

	*/
}

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

void init_uart()
{
	UBRR0H = 0;
	UBRR0L = 8; // 115200  //// if 130 >> 9600

	UCSR0B = 0x18; // Enable RX / TX /// if only 0x08 >> Enables only TX
	UCSR0C = 0x06; // No Parity > 1 Bit
  /// UCSR0C = 0x2E; /// UART Mode, 8-bit Data, Even Parity, 2 Stop Bit
}


int main()
{
	DDRA = 0xff;
	DDRC = 0xff;
	DDRG = 0x0f;

	int i,j;
	char c; 
	init_uart();
	stdout = &mystdout;
	while(1)
	{
		printf("JKIT-128-1 Test Program\n");
		printf("1. Test LED\n");
		printf("2. Test FND\n");
		printf("3. Test All\n");
		printf("Select : %c",c);
		c = getchar0( );
		
		if(c == '1'){			
			PORTA = 0xff;	
			_delay_ms(3000);
			PORTA = 0x00;	
		}
		
		else if(c == '2'){	
			/// have to do here
		}	
	

		else if(c == '3'){	
			/// heve to do here
		}
	}	
}

#include <stdio.h>
#define NULL 0
static int putchar0(unsigned char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(putchar0, NULL, _FDEV_SETUP_WRITE);
volatile unsigned char c=0;	

// 수신: 키보드입력 받을 때 필요
ISR(USART0_RX_vect){
	cli();
	c=UDR0;
	sei();
}
// 송신: printf로 출력할 때 필요
static int putchar0(unsigned char c, FILE *stream)
{
	if(c=='\n')
		putchar0('\r', stream);
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0=c;
	return 0;
}

void init_uart(){
	// ****** 시리얼통신 설정
	UBRR0H=0;
	UBRR0L=8;  // 16MHz, 115200 baud
	UCSR0B=0x98; // Rx Complete Interrupt Enable, RX/TX Enable
	UCSR0C=0x06; // UART Mode, 8 bit data, No parity, 1 Stop bit
	stdout = &mystdout;
	sei();

}
int main()
{
	init_uart();
	while(1){}
}

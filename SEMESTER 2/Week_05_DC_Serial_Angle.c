#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define ANGLE_M_90 1400
#define ANGLE_Z 3000
#define ANGLE_P_90 4600





unsigned int ON = 1 ;


////////////////////////////////////////////////// SERIAL PRINT IMPLEMENTATION
#define NULL 0
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
////////////////////////////////////////////////// END SERIAL PRINT IMPLEMENTATION




void motor(int ang){

	OCR1A = 
		ang == 0 ?
			ANGLE_Z
			: ang == 1 ?
				ANGLE_M_90 :
				ANGLE_P_90 ;
	PORTA = ang ;
}



////////////////////////////////////////////////// UART LOGIC
void reset(){
    printf("Servo Motor Control Program\n 1. -90\n 2. 0\n 3. +90\n 4. Exit\n") ;
}


void end(){
	
	printf("\nProgram ended...") ;
	UCSR0B = 0x18 ;
	ON = 0 ;
}

void evaluate(unsigned char c){
	switch(c){
		
		case '1' :
			motor(0x01) ;		
			printf("Servo Motor Angle -90...") ;
		break ;
		case '2' :
			motor(0x00) ;
			printf("Servo Motor Angle 0...") ;
		break ;
		case '3' : 
			motor(0x02) ;
			printf("Servo Motor Angle +90...") ;
		break ;
		default :
			end() ;
	}
	
}

////////////////////////////////////////////////// END UART LOGIC

volatile unsigned char c, C ;
////////////////////////////////////////////////// UART INTERRUPT
ISR(USART0_RX_vect){
    cli();
    c = UDR0;
	
	switch(c){
		case '\r': 
			evaluate(C) ; 
			break ;
		default:
			printf("%c", c) ;
			C = c ;
			break ;
    }

    sei();
}


void initUART()
{
    
	UBRR0H		= 0;
	UBRR0L		= 8; // 16MHz, 115200 Baud

	UCSR0B		= 0x98; // use RXC Interrupt Rx & Tx enabled
	UCSR0C		= 0x06; //UART Mode 8Bit , No Parity, Stop 1 bit
    
	stdout		= &mystdout;
}

////////////////////////////////////////////////// END UART INTERRUPT


//////// SETTINGS


///////////// BLINKING LEDS
void initLED(){

	DDRA = 0xFF ;

}
///////////// END BLINKING LEDS






///////////// FND
#define FND_LEN 4
unsigned char digit[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};
volatile unsigned int fnd[4];


void initFND(){
	
	DDRC = 0xFF ;
	DDRG = 0x0F ;

}
///////////// END FND



//////// END SETTINGS
void fnd_arr(volatile unsigned int arr[], int n){
	
	arr[3] = (n / 1000) % 10 ;
	arr[2] = (n / 100) % 10 ;
	arr[1] = (n / 10) % 10 ;
	arr[0] = (n / 1) % 10 ;
}



void FND_Count(){
	//fnd_arr(fnd, ct) ;
}

//////// END INTERRUPT TIMER





void displayFND(){

	
		/////// FND DISPLAY
		int max = FND_LEN - 1 ;
		
		
		for(int l = max ; l >= 0 ; --l){
		
			PORTC = digit[fnd[l]] ;
			PORTG = 0x01 << l;

			if(l % 2){
				_delay_ms(2) ;
			}else{
				_delay_ms(3) ;
			}
		}
		_delay_ms(1) ;
}



///////////// DC MOTOR
void initDC(){
	
	DDRB |= (1 << PB5) ;	 // Motor Control Signal = 0C1A (PB5)
	
	// 14 HighSpeed PWM Mode
	// set COM1A = 10
	TCCR1A |= (1 << WGM11) | (1 << COM1A1) ; 

	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11) ;
	
	

}

void settings(){
	initLED() ;
	initDC() ;
	initUART() ;
	sei() ;
}

int main(){
	
	settings() ;
	reset() ;

	ICR1 = 39999 ;

	
	while(ON){
		
		
	}

	return 0;
}

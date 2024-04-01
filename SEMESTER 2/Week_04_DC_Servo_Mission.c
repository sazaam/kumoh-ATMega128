#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>

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

////////////////////////////////////////////////// TIMER
volatile unsigned ct = 0 ;
ISR(TIMER2_COMP_vect){
	ct++ ;
}

void wait(int s){
	int dur = 1000 * s ;
	while(ct <= dur){}
	ct = 0 ;
}
////////////////////////////////////////////////// END TIMER

////////////////////////////////////////////////// MOTOR INTERRUPT


void motor(int s, int speed){
	OCR2 	= speed ;
	TCNT2 	= 0 ;
	PORTA 	= speed ;

	wait(s) ;
}

////////////////////////////////////////////////// END MOTOR INTERRUPT

#define LO		128
#define MID		77
#define HI		26
#define STOP 	255



#define MAX_CHAR 25

////////////// REQUIRED
volatile unsigned char mem[MAX_CHAR] = {0} ;
volatile unsigned char c, oldc;


volatile unsigned int wnum = 0;
int ind = 0 ;


//////////////////////// MEMORY HANDLING
int mem_push(int n){
	
	for(int i = -1; i < MAX_CHAR ; i++){
		if(i == -1){
			oldc = mem[0] ;
		}else if(i == (MAX_CHAR-1)){
			mem[i] = n ;
		}else mem[i] = mem[i+1] ;
	}
	return n ;
}

int mem_back(){
	int n = 0;
	for(int i = MAX_CHAR; i >= 0 ; i--){
		if(i == 0) mem[0] = oldc ;
		else if(i == MAX_CHAR) n = mem[i-1] ;
		else mem[i] = mem[i-1] ;
	}
	return n ;
}

int mem_used_len(){ 
	int l = 0 ;
	for(int i = 0; i < MAX_CHAR ; i++) if(mem[i] != 0 && mem[i] != ' ') l++ ;
	return l;
}

void debug_mem(){ for(int i = 0; i < MAX_CHAR ; i++) printf("%c, ", mem[i]) ; }
void clear_mem(){ for(int i = 0; i < MAX_CHAR ; i++) mem[i] = 0 ; }
//////////////////////// END MEMORY HANDLING


//////////////////////// HANDLING EXITING
int checkEnd(){
	int len = 4 ;
	unsigned char word[4] = {'e', 'x', 'i', 't'} ;
	int flag = mem_used_len() <= len ;
	while(len--){
		char mem_c = mem[MAX_CHAR-(4-len)] ;
		int f = mem_c == word[len] ;
		flag &= f ;
	}
	return flag ;
}


void exiting(){
	printf("\n\t Program is disabled now. \n") ;
	//UCSR0B = 0x18;
	// or 0x18 will only disable what is needed
	UCSR0B = 0x0; // this disables the whole session it seems
}
/////////////////////// END HANDLING EXITING




////////////////////////////////////////////////// UART LOGIC
int checkCharType(unsigned char c){
    return 
        (c >= 'A' && c <= 'Z') ? 1 :
        (c >= 'a' && c <= 'z') ? 2 : 0 ;
}

int checkCharBlank(unsigned char c){ // returns 1 if anything other than space
    return (c == 32) ;
}

void erase(){
	int n = checkCharBlank(mem_back()) ;
	wnum -= n ;
}

void check(unsigned char c){
	int n = checkCharBlank(c) ;
	wnum += n ;
	mem_push(c) ;
}

void reset(){
	wnum = 0 ;
	ind = 0 ;
	clear_mem() ;
    printf("\n >> Motor Speed : ( 1:hi | 2:mid | 3:lo | 0:stop )\n Enter Order:") ;
}

int checkForce(unsigned int force){
	
	int flag = 0 ;

	char mem_c = mem[MAX_CHAR - 1] ;

	return mem_c ;
}


#define lo 0x01
#define mid 0x02
#define hi 0x04
#define stop 0x00

volatile int FORCE = 0 ;

void evaluate(){
	
	//debug_mem() ; // if needed

	if(checkEnd()) return exiting();
	
	FORCE =  mem[MAX_CHAR - 1] - '0' ;
	
	printf("\n >> Force Requested : %d\n", FORCE) ;

	int f ;

	switch(FORCE){
		case 1 :
			f = LO ;
		break ;
		case 2 :
			f = MID ;
		break ;
		case 3 :
			f = HI ;
		break ;
		default :
			f = STOP ;
	}

	OCR2 	= f;
	//TCNT2 	= 0 ;


}
////////////////////////////////////////////////// END UART LOGIC

////////////////////////////////////////////////// UART INTERRUPT
ISR(USART0_RX_vect){
    cli();
    c = UDR0;
	
	switch(c){
		case '\r': 
			evaluate() ; 
			reset() ; 
			break ;
		case 127: 
			printf("%c", c) ; 
			ind-- ;
			erase() ; 
			break ;
		default:
			ind++ ;
			printf("%c", c) ; 
			check(c) ; 
			break ;
    }

    sei();
}

////////////////////////////////////////////////// END UART INTERRUPT



void initLED(){
	DDRA = 0xFF;
}

void initUART()
{
    
	UBRR0H		= 0;
	UBRR0L		= 8; // 16MHz, 115200 Baud

	UCSR0B		= 0x98; // use RXC Interrupt Rx & Tx enabled
	UCSR0C		= 0x06; //UART Mode 8Bit , No Parity, Stop 1 bit
    
	stdout		= &mystdout;
}
void initServo(){
	
	DDRB		= 0xFF ;
	TCCR2		= 0x7B;
	TIMSK		= 0x80 ;

}

void settings(){
	
	initLED() ;
	initServo() ;
    initUART() ;

    
	sei(); // enable global interrupt

}



int ON = 1 ; // To Stop Main Loop if necessary

int main(void){
	
	settings() ;
	
	reset() ;
	
	PORTA = 0x00 ;
	OCR2 = LO ;
	
	int f ;
	while(ON){
		
		
		/*
		PORTB = M_CW ;
		delay(3) ;

		PORTB = M_ST_1 ;
		delay(3) ;

		PORTB = M_CCW ;		
		delay(3) ;

		PORTB = M_ST_2 ;
		delay(3) ;
		*/
	}
	
}



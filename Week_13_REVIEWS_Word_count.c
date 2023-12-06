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

#define MAX_CHAR 25

////////////// REQUIRED
volatile unsigned char mem[MAX_CHAR] = {0} ;
volatile unsigned char c, oldc;
volatile unsigned char cta = 0, ctA = 0, cto = 0;


volatile unsigned int wnum = 0;



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
	printf("\n\t exiting...\n") ;
	UCSR0B = 0x0; // this disables the whole session it seems
}
/////////////////////// END HANDLING EXITING


/////////////////////// PROGRAM LOGIC
int checkCharType(unsigned char c){
    return 
        (c >= 'A' && c <= 'Z') ? 1 :
        (c >= 'a' && c <= 'z') ? 2 : 0 ;
}

int checkCharBlank(unsigned char c){
    return (c == 32) ;
}

void erase(){
	// basckspace case
    
	//int n = checkCharType(mem_back()) ;
	
	int n = checkCharBlank(mem_back()) ;
	
	wnum -= !n ;

	/*
    if(n == 1) ctA-- ;
    else if(n == 2) cta-- ;
    else cto--;
	*/
}
void check(unsigned char c){
	// int n = checkCharType(c) ;
	int n = checkCharBlank(c) ;
	
	wnum += n ;

	/*
	if(n == 1) ctA++ ;
    else if(n == 2) cta++ ;
    else cto++;
	*/
	
	mem_push(c) ;
}
void reset(){
	cta = ctA = cto = 0 ;
	clear_mem() ;
    printf("\nPlease Input a Sentence :") ;
}
void evaluate(){
	
	// debug_mem() ; // if needed

	if(checkEnd()) return exiting();
	
	printf("\n[lowercase] : %d\n", cta) ;
	printf("\n[UPPERCASE] : %d\n", ctA) ;
	printf("\n[!@#$~] : %d\n", cto) ;
}
/////////////////////// END PROGRAM LOGIC



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
			erase() ; 
			break ;
		default:
			printf("%c", c) ; 
			check(c) ; 
			break ;
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

	reset() ;

    while(1)
	{
		//
	}

}

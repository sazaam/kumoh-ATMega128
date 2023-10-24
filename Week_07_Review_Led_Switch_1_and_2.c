
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#include <util/delay.h>


#define MAX 8
volatile unsigned char sw = 0, i = 0, led = 0;


ISR(INT4_vect){
    cli();
    sw = 1;

    sei();
}

ISR(INT5_vect){
    cli();
    sw = 2;
    
    sei();
}

void led1(){
    
    for(i = 0 , led = 0x01; i < MAX ; i++ ){
		PORTA = led ;
        
		led <<= 1 ;
        led |= 0x01 ;
        
        _delay_ms(1000) ;
        if(sw == 2) break ;
        
    }
}

void led2(){
	for(i = 0 , led = 0xC0; i < MAX ; i++ ){
		
		PORTA = led ;
		int forward = i < (MAX / 2) - 1 ;
		
        led  = forward ? led >> 2 : led << 2;
        led |= forward ? 0xC0 : 0x0 ;
        
        _delay_ms(1000) ;
        if(sw == 1) break ;
        
    }
    
}

int settings(){

	DDRA 				= 0xFF ;
    DDRC 				= 0xCF ;
	EICRB 				= 0x0A ;
	EIMSK 				= 0x30 ;

    return 0;
	
}


int main(){
	
	settings() ;

	sei() ;
    while(1){

        switch(sw){
            case 1:
                led1();
            break;
            case 2:
                led2();
            break;

        }
    }
}

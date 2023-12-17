
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#include <util/delay.h>



volatile unsigned char sw = 0, time = 0;

volatile int arr[4] = {-1,} ;
volatile int change = 0 ;
unsigned char digit[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};



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

void display(int n, int d){
	
	PORTG = 0x01 << n ;
	PORTC = d == -1 ? 0x0 : digit[d] ;
	//PORTG = 0x0;

}


void build(int ct){

	switch(sw){
			
		case 1 :
			arr[3] = (ct / 1000)	 %	10;
			arr[2] = (ct / 100)		 %	10;
			arr[1] = (ct / 10)		 %	10;
			arr[0] = (ct / 1)		 % 	10;
		break;
		case 2 :
			arr[3] = -1 ;
			arr[2] = -1 ;
			arr[1] = (ct / 1000) 	% 	10;
			arr[0] = (ct / 100)  	%   10;
		break ;/**/
		default:
			arr[3] = -1 ;
			arr[2] = -1 ;
			arr[1] = -1 ;
			arr[0] = -1 ;
	}
}







#define INTERVAL 50


void displayADC(int ct){
	
	int time = 0 ;

	while(1){
		build(ct) ;
		


		///////// 7-SEGMENT
		for(int i = 0 ; i < 4 ; i++){
			int val = arr[i] ;
			display(i, val) ;
			_delay_ms(2);
		}
		time ++;
		if(time % INTERVAL == 0){ time = 0 ;}
		///////// END 7-SEGMENT
		if(time >= INTERVAL-1)
			break ;
        
    }
}

unsigned short adc_lo = 0 ;
unsigned short adc_hi = 0 ;

//////// INTERRUPT 4 ADC
ISR(ADC_vect){

	cli() ;
	
	adc_lo = ADCL ;
	adc_hi = ADCH ;
	
	unsigned short n = (adc_hi << 8 ) | adc_lo ;
	
	displayADC(n) ;
	
	_delay_ms(1) ;
	
	sei() ;
	
}





///////////// ADC 
void initADC(){
	
	ADMUX = 0x00 ;
	ADCSRA = 0x8F ;

}
///////////// END ADC 

int settings(){

	initADC() ;
	/* LED */
	DDRA 				= 0xFF ;
    DDRC 				= 0xCF ;

	/* FND */
	DDRC = 0xFF;
	DDRG = 0x0F;


	/* INT */  //  Falling = 1-0 rising 1-1 (adds for each 4 & 5 switches) 
	EICRB 				= 0x0A ; //  5 + 4 Falling should be 10 => 0b1010
						 		 
	EIMSK 				= 0x30 ;
	
	sei() ;


    return 0;
	
}


int main(){
	
	settings() ;
	
    while(1){
		
		ADCSRA |= (1 << ADSC) ;
		_delay_ms(1) ;
		
    }
}


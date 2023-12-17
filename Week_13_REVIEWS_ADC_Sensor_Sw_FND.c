
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#include <util/delay.h>


#define MAX 8
volatile unsigned char sw = 0, ct1 = 2, ct2 = 98, time = 0;


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
	PORTC = d ;

}
volatile int arr[4] = {0,} ;
void build(int way, int ct){
	
	if(stage){
		arr[0] = ct / 1000 ;
		arr[1] = ct / 100 ;
		arr[2] = ct / 10 ;
		arr[3] = ct % 10 ;
	}else{
		arr[0] = ct / 1000 ;
		arr[1] = ct / 100 ;
		arr[2] = ct / 10 ;
		arr[3] = ct % 10 ;
		
	}
	
}


void displayADC(int stage, int ct){
	
	build(stage, ct) ;
	
	while(1){
		time++ ;
		for(int i = 0 ; i < 4 ; i++){
			int val = arr[i] ;
		
			if(val != -1) display(i, digit[val]) ;

			_delay_ms(1);
		}

		if(time >= 24) {	
			time = 0 ;
			break;
		
		}
		
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
	
	int margin = 50 ;
	int state = 0 ;
	if(n >= prev + margin){
		state = 2 ;
		prev = n ;
	}else if(n <= prev - margin){
		state = 1 ;
		prev = n ;
	}
	
	
	displayADC(n, state, n - (int)prev) ;
	
	_delay_ms(500) ;
	
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


	/* INT */
	EICRB 				= 0x0A ; //  5 + 4 Falling should be 10 => 0b1010
						 //  Falling = 1-0 rising 1-1 (adds for each 4 & 5 switches) 
	EIMSK 				= 0x30 ;
	
	sei() ;


    return 0;
	
}


int main(){
	
	settings() ;

    while(1){
		
		ADCSRA |= (1 << ADSC) ;

        switch(sw){
            case 1:
                counter1();
            break;
            case 2:
                counter2();
            break;

        }
		
		_delay_ms(1) ;
    }
}

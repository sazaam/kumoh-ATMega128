#include <avr/io.h>


#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#define CDS_1		93  			// 200 / (2000 + 200) * 1024 ==> 2000 K
#define CDS_10 		871  			// 200 / (35 + 200) * 1024 ==> 2000 K
#define CDS_100 	989  			// 200 / (7 + 20) * 1024 ==> 2000 K


void init_ADC(){
	
	ADMUX = 0x00 ;
	ADCSRA = 0x8F ;

}
unsigned short adc_lo = 0 ;
unsigned short adc_hi = 0 ;

void showADC(unsigned short val){
	PORTA = 
		(val <= CDS_1) 		? 0xFF :
		(val <= CDS_10) 	? 0x55 :
		(val <= CDS_100) 	? 0x11 :
							  0x00 ;
}


ISR(ADC_vect){

	cli() ;
	
	adc_lo = ADCL ;
	adc_hi = ADCH ;
	
	showADC((adc_hi << 8 ) | adc_lo) ;
	
	_delay_ms(500) ;
	
	sei() ;
}


int main(){
	DDRA = 0xFF ;
	init_ADC() ;
	sei() ;

	while(1){
		ADCSRA |= (1 << ADSC) ;
	}

	return 0;
}

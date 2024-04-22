
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL

#include <util/delay.h>



///////////////////////////////////// Brightness Levels
#define CDS_1	93
#define CDS_10	871
#define CDS_100 989
///////////////////////////////////// END Brightness Levels

///////////////////////////////////// RGB LED Settings
#define OCR1_R	OCR1A
#define OCR1_G	OCR1B
#define OCR1_B 	OCR1C
///////////////////////////////////// RGB LED Settings


void initTimer(){
	TCCR1A |= _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1) | _BV(COM1C1) ;
	TCCR1B |= _BV(WGM12) | _BV(CS11) ;
	setColor(0, 0, 0) ;
}

void setColor(unsigned int red, unsigned int green, unsigned int blue){
	OCR1_R 		= red ;
	OCR1_G 		= green ;
	OCR1_B 		= blue ;
}


volatile unsigned char adc_low, adc_high;
volatile unsigned short value;


///////////////////////////////////// ADC Sensor INTERRUPT
ISR(ADC_vect){
	cli();
	adc_low = ADCL;
	adc_high = ADCH;
	value = (adc_high << 8) | adc_low;
	show_adc(value);
	sei();
}

//////////////////////////// TODO Implement External LED (RGB) display instead of internal


void show_adc()
{
	/*
	if(value <= CDS_1) PORTA = 0xFF;
	else if(value <= CDS_10) PORTA = 0x55;
	else if(value <= CDS_100) PORTA = 0x11;
	else PORTA = 0x00;
	*/

	unsigned short val = 
		value <= CDS_1 ? 
			255 :
			value <= CDS_10 ? 
				150 : 
				value <= CDS_100 ?
					70 : 0 ;

	setColor(val, val, val) ; 

}
////////////////////////////////////// DEFAULT LED Settings
void init_adc()
{
	ADMUX = 0x00;
	ADCSRA = 0x8F;
}



void settings(){

	//DDRA = 0xFF;
	
	DDRB = _BV(5) | _BV(6) | _BV(7) ;

	initTimer() ;

	init_adc();

}


int main() {
	
	settings() ;
	
	sei();
	
	while(1) { 		
		ADCSRA |= (1<<ADSC);
	}

}


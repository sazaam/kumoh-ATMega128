#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

unsigned char digit[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};
unsigned char fnd[4];

unsigned char song[25] = {5,5,6,6,5,5,3,5,5,3,3,2,5,5,6,6,5,5,3,5,3,2,3,1};




int main(){
	
	
	/* LED */
	DDRA = 0xFF ;

	
	/* FND */
	DDRC = 0xFF;
	DDRG = 0x0F;
	
	
	// SETTINGS
	
	
	int val = 0;
	int pattern = 0xFF ;


	// REQUIRED

	// unsigned is necessary in order to get highest possible numbers
	unsigned int cnt = 0;
	
	
	while(++cnt){
		
		////////// CALCULATIONS

		cnt = cnt % 26 ;
		

		if(cnt >= 24) { // In order to have a pause 2 seconds before the song repeats
			PORTA = 0x0 ;
			PORTG = 0x0 ;
			_delay_ms(1000);
			continue ; 
		}
		
		val = cnt ;
		
		int note = song[val] ;
		///////// LED
		pattern = (0x01 << (8-note)) ;
		
		
		PORTA = pattern ;

		///////// END LED 
		




		///////// 7-SEGMENT

		// switching digit display
		int s = 0x01;

		// finding right digit in bytelist
		int d = digit[note] ;
		
		PORTC = d ;  
		PORTG = s ;

		///////// END 7-SEGMENT


		_delay_ms(1000); 	
		

		

	};

   



}

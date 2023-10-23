#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

unsigned char digit[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};
unsigned char fnd[4];



int main(){
	
	
	/* LED */
	DDRA = 0xFF ;

	
	/* FND */
	DDRC = 0xFF;
	DDRG = 0x0F;
	
	
	// SETTINGS
	
	
	int val = 0;
	int pattern = 0xFF ;
	int oldpattern = 0 ;
	int forward = 1;


	// REQUIRED

	// unsigned is necessary in order to get highest possible numbers
	unsigned int cnt = 0;
	
	
	while(++cnt){
		
		////////// CALCULATIONS

		cnt = cnt % 16 ;
		
		val = cnt ;
		forward = val <= 8;
		val = forward ? val : 16 - val ;
		
		
		///////// LED

		if(forward){
			pattern = (0x01 << (8-val)) ;
			pattern+= oldpattern ;
		}else{
			pattern -= (0x01 << (8-val-1)) ;
		}
		if(val == 0) pattern = 0;
			
		
		oldpattern = pattern ;
		
		PORTA = pattern ;

		///////// END LED 
		




		///////// 7-SEGMENT

		// switching digit display
		int s = 0x01;

		// finding right digit in bytelist
		int d = digit[val] ;
		
		PORTC = d ;  
		PORTG = s ;

		///////// END 7-SEGMENT


		_delay_ms(1000); 	
		

		

	};

   



}

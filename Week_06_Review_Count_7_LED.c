#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

unsigned char digit[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};
unsigned char fnd[4];


unsigned int toSeconds(int m, int s){ return m * 60 + s ; }

void num2time(unsigned char arr[], unsigned int time){
	
	arr[3] = (time / 1000)	 %	10;
	arr[2] = (time / 100)	 %	10;
	arr[1] = (time / 10)	 %	10;
	arr[0] = (time / 1)		 % 	10;

}

int ensureSafeInt(int n, int lim){
	return n % lim ;
}

int main(){
	
	
	/* LED */
	DDRA = 0xFF ;

	
	/* FND */
	DDRC = 0xFF;
	DDRG = 0x0F;
	
	
	// SETTINGS
	
	// start is meant for debug, gives a feined start, not at 0 all the time
	unsigned int start = toSeconds(0, 0);
	
	// Variable speed (243 closest to actual seconds here)	set to 5 in order to go real fast (chronometer-like)
	int speed = 100 ;

	// easy blinking cancelling if set to 0;
	int blink = 1;	

	// default to all off -> disable for blinking effect
	int def = 0x00 ;	


	// REQUIRED

	// unsigned is necessary in order to get highest possible numbers
	unsigned int t = start;
	unsigned int cnt = 0;
	
	// blinking effect related
	int on = 1;
	int limit = 3600;
	
	while(++cnt){
		

			

		// ensure count is not incrementing until reaches unsigned int overflow
		cnt = ensureSafeInt(cnt, speed) ;
		

		// set a dedicated timer
		if(cnt % speed == 0) { 
			t = ensureSafeInt(t+1, limit) ;
		}
		
		on = 1 ;


		
		
		// setting the array of time values
		num2time(fnd, t);
		
		
		/* LED */
		int pattern = ((t-1)/7) ;
		PORTA = t < 8 ? 0x0 : pattern;	
		

		for(int i = 0 ; i < 4 ; i++){
			
			// switching digit display
			int s = 0x01 << i;
			
			// finding right digit in bytelist
			int d = digit[fnd[i]] ;
			
			PORTC = d ;  
			PORTG = s ;
			
			_delay_ms(1); 				// delay 1 is what i found the least annoying for blinking time calculation with seconds
		}
		

	};

   



}

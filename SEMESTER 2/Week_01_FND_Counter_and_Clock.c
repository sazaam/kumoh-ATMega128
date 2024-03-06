#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <math.h>


unsigned char digit[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};
unsigned int fnd[4];
int ON = 1 ; // ON Variable to turn off infinit rendering loop


/*
//////////////////////////////////////// CLASS EXAMPLE 1~1000 Counter

#define FND_LEN 4


int init_FND(){
	DDRC = 0xFF ;
	DDRG = 0x0F ;
	
	
	return 0 ;
}

int FND_ERROR() {
	// Error Msg

	return 1 ;
}



void fnd_arr(unsigned int arr[], int n){
	double l = 3 ;
	while(l--){
		arr[(int) l] = (int) (n / pow(10, l)) % 10 ; // replace multiple lines with calculations through powers of 10 => 10^3 = 1000 , 10^0 = 1
		// But beware !! The arguments and the output of pow() are of type DOUBLE !!!
	}
	
	// arr[0] = (n / 1000) % 10 ;
	// arr[1] = (n / 100) % 10 ;
	// arr[2] = (n / 10) % 10 ;
	// arr[3] = (n / 1) % 10 ;
	
	return 0 ;
}


int main(){

	
	int n = 0 ;

	if (init_FND()) return FND_ERROR() ;
	
	

	while(ON){
		
		n++ ;
		n = n % 1000 ;
		int max = FND_LEN - 1 ;
		int l = max ;
		
		fnd_arr(fnd, n) ;
		
		for(; l >= 0 ; --l){
			
			PORTC = digit[fnd[l]] ;
			PORTG = 0x01 << l;

			if(n % 2){
				_delay_ms(2) ;
			}else{
				_delay_ms(3) ;
			}
			
		}
		
		_delay_ms(1) ;
	
	}
	return 0 ;
}

/*




*/



/////////////////////////////////// MISSION LED CLOCK COUNTER



unsigned int toSeconds(int m, int s){ return m * 60 + s ; }

void num2time(unsigned char arr[], unsigned int time){
	
	arr[3] = (time / 600)	 %	 6;
	arr[2] = (time / 60)	 %	10;
	arr[1] = (time / 10)	 %	 6;
	arr[0] = (time / 1)		 % 	10;

}

int ensureSafeInt(int n, int lim){
	return n % lim ;
}

int main(){
	
	
	DDRC = 0xFF;
	DDRG = 0x0F;
	
	
	// SETTINGS
	
	// start is meant for debug, gives a anticipated start, in order to test higher numbers
	unsigned int start = toSeconds(0, 0);
	
	// Variable speed (244 closest to actual seconds here)	set to 5 in order to go real fast (chronometer-like)
	int speed = 244 ;

	// easy blinking cancelling if set to 0;
	int blink = 1;	

	// default to all off -> disable for blinking effect
	int def = 0x00 ;	


	// REQUIRED

	// unsigned is necessary in order to get highest possible numbers
	unsigned int t = start;
	unsigned int cnt = 0;
	
	// blinking effect related
	int on = 1, remains = 0;
	int limit = 3600;
	

	while(++cnt){
		
		// ensure count is not incrementing until reaches unsigned int overflow
		
		cnt = ensureSafeInt(cnt, speed) ;

		// set a dedicated timer
		if(cnt % speed == 0) { 
			t = ensureSafeInt(t+1, limit) ;
		}
		
		if(blink){
			// remainer of fraction of second
			remains = cnt % speed;
		
			// decide if blinking should be ON or OFF (50% or more of a second ? -> set to black)
			on = remains < (speed >> 1);
		}else{
			on = 1;
		}
		
		
		// setting the array of time values
		num2time(fnd, t);
		

		for(int i = 0 ; i < 4 ; i++){
			
			// switching digit display
			int s = 0x01 << i;
			
			// finding right digit in bytelist
			int d = digit[fnd[i]] ;

			if(i == 2) d |= 0x80 ; 		// have to OR this with 128 in order to spawn the dot
										// = change MSB to high (128 = 0b10000000 = 0x80)
			PORTC = on ? d : def;  
			PORTG = on ? s : def ;
			
			_delay_ms(1); 				// delay 1 is what i found the least annoying for blinking time calculation with seconds
		}
		

	};


}

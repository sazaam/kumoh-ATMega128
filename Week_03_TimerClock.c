#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

unsigned char digit[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};
unsigned char fnd_sel[4] = {0x01, 0x02, 0x04, 0x08};
unsigned char fnd[4];


unsigned int toSeconds(int m, int s){ return m * 60 + s ; }

void num2time(unsigned char arr[], unsigned int time){
	
	arr[3] = (time / 600)	 %	 6;
	arr[2] = (time / 60)	 %	10;
	arr[1] = (time / 10)	 %	 6;
	arr[0] = (time / 1)		 % 	10;

}


int main(){
	
	
	DDRC = 0xFF;
	DDRG = 0x0F;
	
	int i = 0 ;
	int on = 1, remains = 0;

	unsigned int start = toSeconds(59, 57);
	unsigned int t = start;
	unsigned int cnt = 0;
	
	while(1){
		
		cnt = (cnt >= 3600) ? 0 : cnt + 1;		// ensure count is not incrementing until reaches unsigned int overflow

		if(cnt % 360 == 0) { t = (t >= 3600) ? 1 : t + 1 ;}			// set a different timer
		
		remains = cnt % 360 ; 					// remainer of fraction of second (here 1 second is 360 subseconds !!)
		// on = remains < 180; 
		on = 1;

		num2time(fnd, t); 					// setting the array of time values
		
		

		int def =0x00 ;
		int def2 = 0x00 ;

		for(i = 0 ; i < 4 ; i++){
			PORTC = on ? digit[fnd[i]] : def;
			PORTG = on ? fnd_sel[i] : def2 ;
			
			_delay_ms(1);
		}
		

	};
	
}


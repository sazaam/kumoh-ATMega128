#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

unsigned char digit[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};
unsigned char fnd_sel[4] = {0x01, 0x02, 0x04, 0x08};
unsigned char fnd[4];



int num2time(unsigned char arr[], int time){
	

	
	
	int fracTen = (time / 10) % 10;
	int fracUn = (time) % 10;
	int frac = fracTen * 10 + fracUn ;
	
	//arr[3] = (time / (120 * 60 * 10) % 6);
	arr[3] = 0;
	arr[2] = (time / (120 * 60) % 10);
	arr[1] = (time / (1200)) % 6;
	arr[0] = (time / 120) % 10;
	
	
	return frac; // fraction of 1 second (time in milliseconds)
}



int main(){
	
	
	DDRC = 0xFF;
	DDRG = 0x0F;

	int min = 4 ;
	int sec = 31 ;
	int start = (min * 120 * 60) + sec * 120;
	int i = 0 ;
	
	

	int cnt = start;
	

	do{
		cnt++;
		
		//if(cnt == max) cnt = 0;
		
		
		int on = 1 ;
		
		int rem = num2time(fnd, cnt);
		
		// on = rem < 50; // remainer of seconds
		on = 1;

		for(i = 0 ; i < 4 ; i++){
			PORTC = on ? digit[fnd[i]] : 0x00;
			PORTG = on ? fnd_sel[i] : 0x00 ;
			
			_delay_ms(2);
		}
		
		

	}while(1);	
	
}


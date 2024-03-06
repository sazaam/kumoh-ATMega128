#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL




///////////////////////////// MISSION 1

int main(void){

	DDRA = 0xFF ;
	

	unsigned int n = 0x01 << 7 ;
	

	int way = 1 ;

	while(1){
		
		PORTA = n ;
		
		if(n >= 128){
			way = 1 ;
		}else if(n == 1){
			way = 0 ;
		}
		
		n = way ? n >> 1 : n << 1 ;
		
		_delay_ms(1500);

	}
}

/**/

///////////////////////////// END MISSION 1







///////////////////////////// MISSION 2


/*
int main(void){

	DDRA = 0xFF ;
	

	unsigned int n = 0x01 ;
	int mark = 0 ;
	
	int way = 1 ; // SWITCH TO ZERO IN ORDER TO GO IN THE OTHER WAY	

	while(1){
		
		mark++ ;
		mark = mark % 8 ;
		
				

		PORTA = 0x01 << (way ? mark : 7 - mark) ;
		
		_delay_ms(500);
		
	}
}


*/

///////////////////////////// END MISSION 2












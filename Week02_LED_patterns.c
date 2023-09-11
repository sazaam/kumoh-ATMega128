#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

#include <stdlib.h>


/*
int main(void){
	unsigned char ct = 0 ;

	DDRA = 0xFF ;
	
	do{
		PORTA = ct ;
		ct = ct + 1 ;
		
		_delay_ms(500);

	}while(1);
	

}
*/

/*
int main(void){
	unsigned char ct = 0 ;

	DDRA = 0xFF ;
	
	srand(100);

	do{
	
		PORTA = rand() % 256;
		_delay_ms(500) ; 

	}while(1);
	

}
*/


int main(void){
	unsigned int ind = 256 ;
	int hi = 256;
	int lo = 1;
	int up = 0;
	DDRA = 0xFF ;
	
	srand(100);

	int pattern = 0 ;
	
	do{
		switch(pattern){
			case 0 :
				PORTA = ind;
		 
				ind = up ? ind * 2  : ind / 2 ;
		
				if(ind <= 0 || ind >= 255){
					up = !up ;
					ind = up ? lo : hi;
				}
			break;
	
		}
		
		
		_delay_ms(20) ; 
		
	}while(1);
	

}




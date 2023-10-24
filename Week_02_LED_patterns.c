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
	unsigned int ind = 128 ;
	int hi = ind;
	int lo = 1;
	int up = 0;
	
	int i = 0;	

	DDRA = 0xFF ;
	
	srand(100);

	int index = 0;
	
	
	
	do{
		int pattern = (index/30) % 3 ;

		switch(pattern){
			case 0 :
				PORTA = ind;
		 
				ind = up ? ind * 2  : ind / 2 ;
				
				if(ind <= lo || ind >= hi){
					up = !up ;
					ind = up ? lo : hi;
				}
			break;
			case 1 :
				ind = 0;
				
				int ascend = 0, descend = 0;

				for(int n = 0 ; n < i ; n++){
					
					ascend = ascend == 0 ? 1 : ascend;					
					descend = descend == 0 ? 128 : descend;
				
					ind |= ascend ;
					ascend = ascend << 1 ;
					
					ind |= descend ;
					descend = descend >> 1;
					
				}
				
				PORTA = ind;
				
				up ? i-- : i++ ;

				if(i >= 4 || i < 1) up = !up ;

			break;
			case 2 :
				ind = rand( ) % 256 ;
				
				PORTA = ind;
				
				up ? i-- : i++ ;

				if(i >= 4 || i < 1) up = !up ;

			break;
	
		}

		index ++;
		
		_delay_ms(50) ; 
		
	}while(1);
	

}

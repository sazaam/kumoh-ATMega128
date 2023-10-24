
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#include <util/delay.h>


#define MAX 8
volatile unsigned char sw = 0, off = 0;
volatile unsigned int ct = 0;

unsigned char digit[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};
unsigned char fnd[4];

#define MS 250

ISR(INT4_vect){
    cli();
    sw = 1;
    sei();	
}

ISR(INT5_vect){
    cli();
    sw = 2;
    sei();
}

int Up[2] = {0x3E, 0x73} , Dn[2] = {0x5E, 0x54};
int interval = 50 ;
int displayCount(int upwards){
	
	int time = 0 ;

	while(1){
		
		time ++;
		if(time % interval == 0){
			ct ++ ;
			ct = ct %10 ;
			time = 0 ;
		}
		int n = upwards ? ct : 9 - ct;
		///////// 7-SEGMENT
		for(int i = 0 ; i < 4 ; i++){
			
			int val = 
				(i >= 2) ? 
					(upwards ? Up[3-i] : Dn[3-i] )
					: (i == 1 ) ? (n+1) / 10 : (n+1) % 10 ;
		
			display(i, i < 2 ? digit[val] : val) ;
			
			_delay_ms(2) ;
		}
		///////// END 7-SEGMENT
		if(ct == 9 && time > interval - 2)
			break ;
        
    }
	return exiting();

}
void display(int n, int d){
	PORTC = d ;  
	PORTG = 0x01 << n ;
}


int exiting(){
	off = 1 ;
	for(int i = 0 ; i < 4 ; i++) {display(i, 0x0);_delay_ms(2);}
	DDRC = 0x0 ;
	DDRG = 0x0;
	
	return 0;
	
}


int settings(){
	

	/* LED */
	DDRA 				= 0xFF ;
    DDRC 				= 0xCF ;

	/* FND */
	DDRC = 0xFF;
	DDRG = 0x0F;


	/* INT */
	EICRB 				= 0x0A ;
	EIMSK 				= 0x30 ;
	
    return 0;
	
}

int order[2] = {0}, uporder[2] = {1, 2} ; 

int compareOrders(){
	
	int matches = 1;
	for(int i = 0 ; i < 2 ; i++){
		matches &= (order[i] == uporder[i]) ;
	}

	return matches ;	
}


int main(){
	
	settings() ;
	
	sei() ;
	
	
	int n = 0;
	int marked = 0;
	while(1){
		
		if(n == 2){
			
			int up = compareOrders() ;
			PORTA = up? 128 : 1 ;
			
			displayCount(up) ;

						
		} else if(marked != sw) marked = order[n++] = sw ; // can not be the same

		_delay_ms(1) ;
		if(off) break;
	}
	
	

	return 1;
    
}


#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#include <util/delay.h>


#define MAX 8
volatile unsigned char sw = 0, ct1 = 2, ct2 = 98, time = 0;


unsigned char digit[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};

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

void display(int n, int d){
	
	PORTG = 0x01 << n ;
	PORTC = d ;

}
volatile int arr[4] = {0,} ;
void build(int way, int ct){
	
	if(way){
		arr[3] = ct / 10 ;
		arr[2] = ct % 10 ;
		arr[1] = -1 ;
		arr[0] = -1 ;
	}else{
		arr[3] = -1 ;
		arr[2] = -1 ;
		arr[1] = ct / 10 ;
		arr[0] = ct % 10 ;
		
	}
	
}


void displayCount(int way, int ct){
	
	build(way, ct) ;
	
	while(1){
		time++ ;
		for(int i = 0 ; i < 4 ; i++){
			int val = arr[i] ;
		
			if(val != -1) display(i, digit[val]) ;

			_delay_ms(1);
		}

		if(time >= 24) {	
			time = 0 ;
			break;
		
		}
		
	}
}
int counter1(){
    
	displayCount(1, ct1) ;
	if(ct1 < 98){
		ct1+= 2 ;
	}
	return 1 ;
}

int counter2(){
	
	displayCount(0, ct2) ;
	if(ct2 > 2){
		ct2-= 2 ;
	}
    return 1;
}




int settings(){

	
	/* LED */
	DDRA 				= 0xFF ;
    DDRC 				= 0xCF ;

	/* FND */
	DDRC = 0xFF;
	DDRG = 0x0F;


	/* INT */
	EICRB 				= 0x0B ; // Rising 5 + Falling 4 => 1011 
	EIMSK 				= 0x30 ;
	
	sei() ;


    return 0;
	
}


int main(){
	
	settings() ;

	sei() ;
    while(1){
		
        switch(sw){
            case 1:
                counter1();
            break;
            case 2:
                counter2();
            break;

        }
		
		_delay_ms(1) ;
    }
}

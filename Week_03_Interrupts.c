#include <avr/io.h> //ATmega128 register 정의
#define F_CPU 16000000UL
#include <util/delay.h>


/////////////////////////////////// INTERRUPTS

#include <avr/interrupt.h> //interrupt 관련

#define OFF 0
#define ON 1

// volatile: 컴파일러가 임의로 코드를 최적화하지 않도록 사용하는 키워드
// 이 키워드가 붙여진 변수와 관련된 코드는 코딩된 내용 그대로 컴파일됨
// 인터럽트 루틴에서 사용하는 변수에는 챙겨주자.
volatile int int4_flag=OFF; //스위치 1 눌렀을 때의 LED 상태를 기억
volatile unsigned int count = 0;

// INT 4번에 연결된 스위치 1이 눌러졌을 때 자동 시작되는 ISR 
ISR(INT4_vect){
	cli();	// 다른 인터럽트가 안걸리게 전체 인터럽트 disable
	
	if(int4_flag==OFF){	// 스위치1 눌렀을 때 이전 상태가 꺼진 상태였다면
		int4_flag=ON; // 켜진 상태로 바꾸고
		PORTA=0xff;  // LED를 실제로 켬
	} else{	 // 스위치 1 눌렀을 때 이전 상태가 켜진 상태였다면
		int4_flag=OFF;	// 꺼진 상태로 바꾸고
		PORTA=0x00;	// LED를 실제로 끔
	}
	_delay_ms(100);
	sei();	// 서비스 끝났으므로 다시 인터럽트 가능하게 전체 인터럽트 enable
}

// INT 5번에 연결된 스위치 1이 눌러졌을 때 자동 시작되는 ISR 
ISR(INT5_vect){
	cli();
	count++;
	PORTA=count;
	_delay_ms(100);
	sei();
}

void main(){
	DDRA=0xff; //포트 A를 출력으로 사용
	PORTA=0x00; //LED를 모두 OFF

	DDRE=0xCF;
		// 스위치1(4번 INT)과 스위치2(5번 INT)을 둘 다 입력(해당 비트를 0으로)으로 설정 
		// 수정 전 1110 1111 (4번 INT) => 수정 후 1100 1111(5번/4번 INT) 

	EICRA=0x00; 
		// INT 0~3은 쓰지 않음. 그래서 이 코드는 생략 가능

	EICRB=0x0A; 
		// INT 4,5의 Trigger 방법을 둘다 falling edge (이진 수로 10)로 각각 설정 
		// 수정 전 2(0010) (4번 INT의 falling edge 설정)
		// 수정 후 A(1010) (5번 INT / 4번 INT 모두 falling edge 설정)

	EIMSK=0x30; 
		// INT 5와 INT 4를 사용하도록 enable 
		// 0011 0000

	sei(); 
		// 전체 인터럽트 enable 꼭 해주어야 함.

	while(1); //main에서는 세부적으로 수행하는 내용없이 무한 루프만 유지
}





/*

/// SAME EXAMPLE WITHOUT INTERRUPTS

int main(){
	
	
	// SWITCHES on PORT
		
	DDRA = 0xFF;
	DDRE = 0x00;
	
	
	// SETTINGS
	

	while(1){
		
		if((PINE & 0x10) == 0x00) // Port IN E (on Top)
			PORTA = 0x80;
		else if((PINE & 0x20) == 0x00) // Port IN E (on Bottom)
			PORTA = 0x01;
		else
			PORTA = 0x00;

	};

   	
	







}
*/

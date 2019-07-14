/*
 * Wave Generate.c
 *
 * Created: 2018-02-21 오전 7:21:35
 * Author : njg71
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile int count = 0;		//비교 일치가 발생한 횟수
int state = 0;				//LED점멸 상태

ISR(TIMER0_COMP_vect)
{
	count++;
	TCNT0 = 0;				//자동으로 0으로 변하지 않는다.
}

int main(void)
{
	
	DDRB = 0x10;			//파형 출력 핀인 OC0(PB4) 핀을 출력으로 설정
	PORTB = 0x00;			//LED는 끈 상태에서 시작
	
	
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);//분주비를 1.024로 설정
	
	OCR0 = 255;				//비교 일치 기준값
	
	
	TCCR0 |= (1 << COM00);	//비교 일치 인터럽트 발생 시 OC0 핀의 출력을 반전
	
	TIMSK |= (1 << OCIE0);	//비교 일치 인터럽트 허용
	
	sei();					//전역적 인터럽트 허용
	
    while (1) {}
	return 0;
}


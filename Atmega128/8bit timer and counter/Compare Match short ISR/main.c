/*
 * Compare Match short ISR.c
 *
 * Created: 2018-02-21 오전 6:11:16
 * Author : njg71
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile int count = 0;
int state = 0;

ISR(TIMER0_COMP_vect)
{
	count++;
	TCNT0 = 0;					//자동으로 변하지 않는다.
}

int main(void)
{
   DDRB = 0x01;					//PB0 핀을 출력으로 설정
   PORTB = 0x00;				//LED는 끈 상태에서 시작
   
   TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);
   
   OCR0 = 128;					//비교 일치 기준값
   
   TIMSK |= (1 << OCIE0);		//비교 일치 인터럽트 허용
   
   sei();						//전역적으로 인터럽트 허용
   
    while (1) 
    {
		if (count == 64)		//비교 일치 64회 발생 = 0.5초 경과
		{
			count = 0;			//카운터 초기화
			state = !state;		//LED상태 반전
			
			if (state)			//변수값에 따라 LED상태변환
			PORTB = 0x01; 
			else
			PORTB = 0x00;
		}
    }
	return 0;
}


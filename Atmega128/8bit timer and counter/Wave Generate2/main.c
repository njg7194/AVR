/*
 * Wave Generate2.c
 *
 * Created: 2018-02-21 오후 4:53:13
 * Author : njg71
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile int count =0;				//비교 일치가 발생한 횟수
int state = 0;						//LED점멸 상태

ISR(TIMER0_COMP_vect)
{
	count++;
}

int main(void)
{
    DDRB = 0x10;
	PORTB = 0x00;					//LED는 끈 상태에서 시작
	
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);//분주비 1,024 설정
	
	TCCR0 |= (1 << WGM01);			//CTC모드
	
	OCR0 =255;						//비교 일치 기준값
	
									//비교 일치 인터럽트 발생 시 OC0핀의 출력을 반전
	TCCR0 |= (1 << COM00);			
	
	TIMSK |= (1 << OCIE0);			//비교 일치 인터럽트 허용
	
	sei();							//전역적으로 인터럽트 허용
	
    while (1){}
	return 0;
}


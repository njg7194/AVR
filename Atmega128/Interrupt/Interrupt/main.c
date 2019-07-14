/*
 * Interrupt.c
 *
 * Created: 2018-02-01 오전 1:15:33
 * Author : njg71
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int state = 0;			//컴파일러가 명시되지 않은 변수를 상수로 이해하는것을 막기 위해서 volatile을 사용.

ISR(INT0_vect)					//외부 인터럽트0 에 대한 ISR함수이다.
{
	state = (state + 1) % 2;	//기본값은 0이다. ISR이 호출되면 1을 더한 후 2로 나누고, 나머지인 1을 다시 변수에 입력한다.
								//다시 한번 더 호출되면, 변수값에 1을 더하고 2로 나누어서 나머지인 0을 다시 변수에 입력한다.
}

void INIT_PORT(void)			//외부입출력포트 초기화 함수이다.
{
	DDRB = 0x01;				//PORTB0번 핀을 출력으로 설정.
	PORTB = 0x00;				//초기값을 LOW로 설정.(LED꺼짐)
	
	DDRD = 0x00;				//PORTD0번 핀을 입력으로 설정.
	PORTD = 0x01;				//PORTD0번 핀을 풀업 저항으로 사용.(기본 HIGH)
}

void INIT_INT0(void)
{
	EIMSK |= (1 << INT0);		//외부 인터럽트0 의 발생을 허가함.
	EICRA |= (1 << ISC01);		//발생 시점은 하강 에지에서.
	sei();						//전역적 인터럽트 허용.
}


int main(void)
{
	INIT_PORT();				//포트 초기화.
	INIT_INT0();				//인터럽트 초기화.
	
    while (1)					
    {
		if (state == 1)			//변수값이 1이면 LED ON, 0이면 LED OFF.
		{
			PORTB = 0x01;
		}
		else
		{
			PORTB = 0x00;
		}
    }
	
	return 0;
}


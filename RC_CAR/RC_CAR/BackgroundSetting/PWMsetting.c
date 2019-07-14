/*
 * UARTsetting.c
 *
 * Created: 2018-11-03 오후 1:38:14
 *  Author: 
 */ 
#include "PWMsetting.h"

void PWM_init(unsigned char port1, unsigned char port2)
{
	DDRB |= (1<< port1) | (1<<port2);				//출력핀 설정
	
	TCCR0 |= (1<<WGM01) | (1<<WGM00);				//파형생성모드 설정(FastPWM)
	TCCR0 |= (1<<COM01);							//비교 일치 출력 모드 설정(비반전 모드)
	TCCR0 |= /*(1<<CS02) | (1<<CS01) | */(1<<CS00);	//클럭 설정(분주비1,024)
	
	TCCR2 |= (1<<WGM21) | (1<<WGM20);				//파형생성모드 설정(FastPWM)
	TCCR2 |= (1<<COM21);							//비교 일치 출력 모드 설정(비반전 모드)
	TCCR2 |= /*(1<<CS22) | (1<<CS21) | */(1<<CS20);	//클럭 설정(분주비8)
	
	//TIMSK |= (1<<OCIE0) | (1<<TOIE0) | (1<<OCIE2) | (1<<TOIE2);	//인터럽트 마스크 개방(미사용)
}

void PWMwrite(unsigned char port, unsigned char value)
{
	if (port==PORTB4)
	OCR0=value;
	else
	OCR2=value;
}
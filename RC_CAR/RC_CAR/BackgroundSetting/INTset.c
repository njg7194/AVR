/*
 * INTset.c
 *
 * Created: 2018-11-05 오전 6:23:32
 *  Author: njg71
 */ 
#include "INTset.h"

void INT_disableSENSOR()
{
	EIMSK &= 0x00;
}

void INT_enableSENSOR()
{
	EIMSK |= (1<<INT5) | (1<<INT6) | (1<<INT7);
}

void INT_init()
{
	DDRE = 0x00;
	PORTE = 0xFF;
	
	EIMSK |= (1<<INT5) | (1<<INT6) | (1<<INT7);			//INT5,6,7인터럽트 발생 허용
	EICRB |= 0x00;										//LOW일때 인터럽트
	sei();
}



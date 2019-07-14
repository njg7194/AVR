/*
 * FirstProgram.c
 *
 * Created: 2018-01-07 오후 4:39:32
 * Author : njg71
 */ 

#define  F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRA |= 0x01;
	
    while (1) 
    {
		PORTA |=0x01;
		_delay_ms(500);
		PORTA &= ~0x01;
		_delay_ms(500);
    }
	
	return 0;
}


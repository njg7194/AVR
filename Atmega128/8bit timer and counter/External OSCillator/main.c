/*
 * External OSCillator.c
 *
 * Created: 2018-02-22 오전 10:07:16
 * Author : njg71
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER0_OVF_vect)
{
	state = !state;
	if(state) PORTB = 0x01;
	else PORTB = 0x00;
}

int main(void)
{
    DDRB = 0x01;
	PORTB = 0x00;
	
	ASSR |= (1 << AS0);
	
	TCCR0 |= (1 << CS02) | (1 << CS00);
	TIMSK |= (1 << TOIE0);
	
	sei();
	
    while (1){}
}


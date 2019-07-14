/*
 * 8bit timer and counter.c
 *
 * Created: 2018-02-04 오전 5:16:19
 * Author : njg71
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

int count = 0;
int state =0;
ISR (TIMER0_OVF_vect)
{	
	count++;
	if(count ==32)
	{
		count = 0;
		state = !state;
		if(state) PORTB = 0x01;
		else PORTB = 0x00;
	}
}

int main(void)
{
	DDRB = 0x01;
	PORTB = 0x00;
	
	TCCR0 |= (1<<CS02) | (1 << CS01) | (1 << CS00);
	
	TIMSK |= (1 << TOIE0);
	
	sei();
	
    while (1){}
	return 0;
}


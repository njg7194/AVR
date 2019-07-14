/*
 * Potenthometer Reading.c
 *
 * Created: 2018-01-08 오후 9:55:35
 * Author : njg71
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART1.h"

/*
FILE OUTPUT 
	= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT 
	= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);
*/
void ADC_init(unsigned char channel)
{
	
	ADMUX |= (1 << REFS0);
	
	ADCSRA |= 0x07;
	ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADFR);
	
	ADMUX = ((ADMUX & 0xE0) | channel);
	ADCSRA |= (1 << ADSC);
}

int read_ADC(void)
{
	while(!(ADCSRA & (1 << ADIF)));
	
	return ADC;
}

int main(void)
{
	int read;
	
	//stdout = &OUTPUT;
	//stdin = &INPUT;
	DDRE=0xFF;
	UART1_init();
	ADC_init(0);
	
    while (1) 
    {
		read = read_ADC();
		
		if (read>(1024/8)*1)
		PORTE=0b00000001;
		else if (read>(1024/8)*2)
		PORTE=0b00000011;
		else if (read>(1024/8)*3)
		PORTE=0b00000111;
		else if (read>(1024/8)*4)
		PORTE=0b00001111;
		else if (read>(1024/8)*5)
		PORTE=0b00011111;
		else if (read>(1024/8)*6)
		PORTE=0b00111111;
		else if (read>(1024/8)*7)
		PORTE=0b01111111;
    }
	
	return 0;
}


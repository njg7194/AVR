/*
 * UART_test.c
 *
 * Created: 2017-12-11 오후 6:58:32
 * Author : njg71
 */ 

#include <avr/io.h>
//#include <stdio.h>
#include "UART1.h"

int main(void)
{
	UART1_init();
	
	char str[] =  "Test using UART1 Liberary";
	uint8_t num = 128;
	
	UART1_print_string(str);
	UART1_print_string("\n\r");
	
	UART1_print_1_byte_number(num);
	UART1_print_string("\n\r");
	
    /* Replace with your application code */
    while (1)
    {
    }
	
	return 0;
}


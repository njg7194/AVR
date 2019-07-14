/*
 * UARTsetting.h
 *
 * Created: 2018-11-03 오후 1:38:30
 *  Author: njg71
 */ 


#ifndef UARTSETTING_H_
#define UARTSETTING_H_

#include <stdio.h>
#include <avr/interrupt.h>
//#include <avr/sfr_defs.h>

void UART1_init(void);
void UART1_transmit(unsigned char data);
unsigned char UART1_receive(void);
void UART1_print_string(char *str);
void UART1_print_1_byte_number(uint8_t n);

#endif
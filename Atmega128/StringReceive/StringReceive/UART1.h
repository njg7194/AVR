/*
 * UART1.h
 *
 * Created: 2017-12-11 오후 7:00:08
 *  Author: njg71
 */ 


#ifndef UART1_H_
#define UART1_H_

void UART1_init(void);
void UART1_transmit(char data);
unsigned char UART1_receive(void);
void UART1_print_string(char *str);
void UART1_print_1_byte_number(uint8_t n);
void receiveString(char* buffer, int* index);

#endif /* UART1_H_ */
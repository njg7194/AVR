/*
 * PWMsetting.h
 *
 * Created: 2018-11-03 오후 1:38:21
 *  Author: 
 */ 


#ifndef PWMSETTING_H_
#define PWMSETTING_H_

#include <avr/io.h>

void PWM_init(unsigned char port1, unsigned char port2);
void PWMwrite(unsigned char port, unsigned char value);

#endif /* INCFILE1_H_ */
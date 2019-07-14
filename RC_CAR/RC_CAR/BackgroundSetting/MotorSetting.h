/*
 * MotorSetting.h
 *
 * Created: 2018-11-03 오후 3:02:59
 *  Author: njg71
 */ 


#ifndef MOTORSETTING_H_
#define MOTORSETTING_H_

#define F_CPU 16000000L

#include "PWMsetting.h"
#include "INTset.h"

#include <util/delay.h>

#define LEFT_M_CONTROL PORTB4
#define RIGHT_M_CONTROL PORTB7

void Motor_init();
void LEFT_wheel(char direction, char speed);
void RIGHT_wheel(char direction, char speed);
void Go_forward(char speed);
void Go_backward(char speed);
void Go_STOP();
void Evasion(char direction);

#endif /* MOTORSETTING_H_ */
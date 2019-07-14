/*
 * MotorSetting.c
 *
 * Created: 2018-11-03 오후 3:02:44
 *  Author: njg71
 */ 
#include "MotorSetting.h"

void Motor_init()
{
	DDRB |= (1<<DDB0);			//Motor1-1	Output Set
	DDRB |= (1<<DDRB1);			//Motor1-2
	DDRB |= (1<<DDRB2);			//Motor2-1
	DDRB |= (1<<DDRB3);			//Motor2-2
	
	PORTB |= 0x05;				//모터 기본 방향-전진
}
/*
first is direction.(F or B or S)
*/
void LEFT_wheel(char direction, char speed)
{
	PWMwrite(LEFT_M_CONTROL, speed);
	if (direction == 'F')
	{
		PORTB &= 0xFC;
		PORTB |= 0x01;
	}
	else if (direction == 'B')
	{
		PORTB &= 0xFC;
		PORTB |= 0x02;
	}
	else if (direction == 'S')
	{
		PORTB &= 0xFC;
	}
}

void RIGHT_wheel(char direction, char speed)
{
	PWMwrite(RIGHT_M_CONTROL, speed);
	if (direction == 'F')
	{
		PORTB &= 0xF3;
		PORTB |= 0x04;
	}
	else if (direction == 'B')
	{
		PORTB &= 0xF3;
		PORTB |= 0x08;
	}
	else if (direction == 'S')
	{
		PORTB &= 0xF3;
	}
}

void Go_forward(char speed)
{
	if (!speed)
	speed = 255;
	
	LEFT_wheel('F', speed);
	RIGHT_wheel('F', (speed+9));	
}

void Go_backward(char speed)
{
	if (!speed)
	speed = 255;
	
	LEFT_wheel('B', speed);
	RIGHT_wheel('B', speed);
}

void Go_STOP()
{
	LEFT_wheel('S',0);
	RIGHT_wheel('S',0);
}
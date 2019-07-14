/*
 * RC_CAR.c
 *
 * Created: 2018-11-03 오후 1:36:10
 * Author : 
 */ 

#include "BackgroundSetting/Bluetooth.h"
#include "BackgroundSetting/MotorSetting.h"

#define FORWARD 'F'
#define BACKWARD 'B'
#define STOP 'S'

#define LEFTWHEEL DDRB4
#define RIGHTWHEEL DDRB7	

volatile int index = 0;									//명령어 저장 배열 index
char buffer[20] = "RIGHT";								//명령어 저장용 배열
char COM_IN = 0;										//명령어 도착 알림
char Front_toggle = 1;									//LEFT,FRONT,RIGHT SENSOR TOGGLE
char AUTO_STATE = 0;	
					
ISR(INT5_vect)
{
	INT_disableSENSOR();
	if (AUTO_STATE)
	{
		RIGHT_wheel('S',0);
		LEFT_wheel('F',130);
		UART1_print_string("!LEFT!");
		UART1_transmit('\n');
	}
	_delay_ms(100);
	INT_enableSENSOR();
}
ISR(INT6_vect)
{
	INT_disableSENSOR();
	if (AUTO_STATE)
	{
		if (Front_toggle)
		{
			RIGHT_wheel('S',0);
			LEFT_wheel('F',130);
		}
		else
		{
			LEFT_wheel('S',0);
			RIGHT_wheel('F',140);
		}
		

		UART1_print_string("!FRONT!");
		UART1_transmit('\n');
	}
	_delay_ms(100);
	INT_enableSENSOR();

}
ISR(INT7_vect)
{
	INT_disableSENSOR();
	if (AUTO_STATE)
	{
		LEFT_wheel('S',0);
		RIGHT_wheel('F',140);
		UART1_print_string("!RIGHT!");
		UART1_transmit('\n');
	}
	_delay_ms(100);
	INT_enableSENSOR();
}

ISR(USART1_RX_vect)
{
	cli();
	char data = UART1_receive();
	if (data =='@')
	{
		buffer[index] = '\0';
		index = 0;
		COM_IN = 1;
	}
	else
	{
		if (index==0)
		{
			for(int i = 0;i<index;i++)
			buffer[i]=NULL;
		}
		buffer[index] = data;
		index++;
	}
	sei();
}

int main(void)
{
	INT_init();
	UART1_init();
	PWM_init(LEFTWHEEL,RIGHTWHEEL);
	
	INT_disableSENSOR();

	char COMMAND;
	char AUTO_SPEED=0;
    while (1) 
    {

		COMMAND = BLTdiscrimination(buffer);			//버퍼 데이터를 문자 커멘드로 변환						

		if ((COMMAND == 'T') && (COM_IN && 1))			// 자동운전 상태 토글 명령어 수신했을 경우
		{
			COM_IN = 0;									//명령 도착 알림 제거
			AUTO_STATE = !AUTO_STATE;					//명령 : T(toggle)자동운전 제어 변수를 토글한다.
			
			if (AUTO_STATE==1)
			{
				UART1_print_string("NOW IS AUTO DRIVING");
				UART1_transmit('\n');
				INT_enableSENSOR();
			}
			else
			{
				Go_STOP();
				INT_disableSENSOR();
			}
			
		}
		
		
		if (!AUTO_STATE)									//자동운전이 아닐 경우
		{
			if (COM_IN)
			{
				COM_IN = 0;									//명령 도착 알림 제거
				switch(COMMAND)								//명령어 선택
				{
				case 'S':
				UART1_transmit('S');
				LEFT_wheel(STOP, 0);
				RIGHT_wheel(STOP, 0);
				break;
				
				case 'F':
				UART1_transmit('F');
				LEFT_wheel(FORWARD, 200);
				RIGHT_wheel(FORWARD, 217);
				break;
				
				case 'L':
				UART1_transmit('L');
				LEFT_wheel(BACKWARD, 160);
				RIGHT_wheel(FORWARD, 160);
				break;
				
				case 'R':
				UART1_transmit('R');
				LEFT_wheel(FORWARD, 160);
				RIGHT_wheel(BACKWARD, 160);
				break;
				
				case 'B':
				UART1_transmit('B');
				LEFT_wheel(BACKWARD, 200);
				RIGHT_wheel(BACKWARD, 210);
				break;
				}

			}
			
		}
		else if (AUTO_STATE)				//자동운전일 경우
		{
			if (AUTO_SPEED!=135)
				AUTO_SPEED++;
				
			Go_forward(AUTO_SPEED);
			if (Front_toggle)
			Front_toggle = 0;
			else
			Front_toggle = 1;
		}
		_delay_ms(1);
	}
    return 0;
}


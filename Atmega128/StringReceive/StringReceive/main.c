/*
 * StringReceive.c
 *
 * Created: 2017-12-11 오후 7:56:08
 * Author : njg71
 */ 
#define  F_CPU 16000000L
#define  TERMINATOR '\r'

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include "UART1.h"

char buffer[20] = "";
volatile int index = 0;

ISR(USART1_RX_vect)
{
	char data = UART1_receive();
	if (data =='@')
	{
		buffer[index] = '\0';
		index = 0;
	}
	else
	{
		if (index==0)
		{
			for(int i=0;i<index;i++)
			buffer[i]=NULL;
		}
		buffer[index] = data;
		index++;
	}
	
}

int main(void)
{
	sei();
	uint8_t counter = 100;					//카운터
	//int index = 0;							//수신 버퍼에 저장할 위치
	//int procss_data = 0;					//문자열 처리
						//수신 데이터 버퍼
	//char data;								//수신 데이터
	UART1_init();							//UART 통신 초기화
	
	UART1_print_string("Current Counter Value : ");
	UART1_print_1_byte_number(counter);
	UART1_print_string("\r\n");

    while (1) 
    {
		/*
		data = UART1_receive();				//데이터 수신 대기
		if(data == TERMINATOR)				//종료 문자를 수신한 경우
		{
			buffer[index] = '\0';			//
			procss_data = 1;				//수신 문자열 처리 지시
		}
		else
		{
			buffer[index] = data;			//수신 버퍼에 저장
			index++;
		}
		*/
		/*
		if(procss_data == 1)
		{
			if (strcasecmp(buffer,"DOWN") == 0)
			{
				counter--;
				UART1_print_string("Current Counter Value : ");
				UART1_print_1_byte_number(counter);
				UART1_print_string("\r\n");
			}
			
			else if (strcmp(buffer,"UP") == 0 )
			{
				counter++;
				UART1_print_string("Current Counter Value : ");
				UART1_print_1_byte_number(counter);
				UART1_print_string("\r\n");
			}
			
			else
			{
				UART1_print_string("** Unknown Command **");
				UART1_print_string("\r\n");
			}
			
			
			
			index = 0;
			procss_data = 0;
			
		}
		*/
	//	for (int i=0; i<10;i++)
	//	{
			UART1_print_string(buffer);
			UART1_print_string("\r\n");
			_delay_ms(100);
	//	}
    }
	return 0;
}


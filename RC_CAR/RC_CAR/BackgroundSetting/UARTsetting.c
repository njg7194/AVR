﻿/*
UARTsetting.c
 *
 * Created: 2018-11-03 오후 1:38:26
 *  Author: 
 */ 
#include "UARTsetting.h"

void UART1_init(void)
{
	UBRR1H = 0x00;						//9,600보율로 설정
	UBRR1L = 207;
	
	UCSR1A |= _BV(U2X1);				//2배속 모드
	UCSR1C |= 0x06;						//비동기, 8비트 데이터, 패리티 없음, 1비트 정지 비트 모드
	
	UCSR1B |= _BV(RXEN1);				//송신 허용
	UCSR1B |= _BV(TXEN1);				//수신 허용
	UCSR1B |= _BV(RXCIE0);				//수신 인터럽트 허용

}

void UART1_transmit(unsigned char data)
{
	cli();
	while( !(UCSR1A & (1 << UDRE1)) );	//송신 가능 대기
	UDR1 = data;						//: UDRE1의 값 만큼 1(비트)을 이동시킨다.  UCSR1A레지스터와 같지않으면 계속 실행(대기)한다.
	sei();
}										//  1<<5는 0b00100000이다.	UDRE1의 값이 5지만, 5를 그대로 2진수로 변환하면
										//  0b00000101 이기 때문에 UDRE인 5번째 비트를 나타내지 못한다. 때문에 비트이동으로 비트위치를 표현한다.

unsigned char UART1_receive(void)
{
	while( !(UCSR1A & (1<<RXC1)) );		//UCSR1A레지스터와 RXC1비트를 비교하며, 같지 않으면 계속 실행(대기)한다
	return UDR1;						//데이터가 수신되어 읽지 않은 문자가 있으면 RXC1이set되어 반복문을 빠져나오고 리턴값으로
}										//UDR1레지스터를 반환한다.

void UART1_print_string(char *str)		//문자열 송신
{
	for(int i = 0; str[i]; i++)			//'\0'문자(끝)을 만날 때까지 반복
	UART1_transmit(str[i]);				//바이트 단위로 출력한다.
}

void UART1_print_1_byte_number(uint8_t n)
{
	//char numString[4] = "0";			//파일 스트림을 이용해 printfs함수 사용하여 구현한 코드.
	//sprintf(numString, "%d", n);		//파일 스트림 호환성 문제가 발생할 수 있기에 비활성화 함.
	//UART1_print_string(numString);
	
	char numString[4] = "0";
	int i, index = 0;
	
	if (n > 0)							//문자열 변환
	{
		for (i = 0; n != 0; i++)
		{
			numString[i] = n % 10 + '0';
			n = n / 10;
		}
		numString[i] = '\0';
		index = i - 1;
	}
	
	for(i = index; i >= 0; i--)			//변환된 문자열을 역순으로 출력
	UART1_transmit(numString[i]);

}

void UART1_print_float(float f)
{
	char numString[20] = "0";
	
	sprintf(numString, "%f", f);
	UART1_print_string(numString);
}
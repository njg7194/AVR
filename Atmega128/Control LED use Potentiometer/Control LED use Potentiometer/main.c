/*
 * Control LED use Potentiometer.c
 *
 * Created: 2018-01-15 오후 12:59:10
 * Author : njg71
 */ 
#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "UART1.h"

FILE OUTPUT 
	= FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT 
	= FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

void ADC_init(unsigned char channel)
{
	ADMUX |= (1 << REFS0);				//AVCC를 기준 전압으로 선택
	
	ADCSRA |= 0x07;
	ADCSRA |= (1 << ADEN);				//7 ADC활성화
	ADCSRA |= (1 << ADFR);				//5 프리러닝 모드
	
	ADMUX = ((ADMUX & 0xE0) | channel);	//채널 선택
	ADCSRA |= (1 << ADSC);				//변환 시작
}

int read_ADC(void)
{
	while (!(ADCSRA & (1 << ADIF)));	//ADIF는 상수4이다. 1을 4자리만큼 비트쉬프트하면 0b00000001에서 0b00010000이 된다.
										//0~8번비트중 4번비트(5번째)인 ADIF를 지칭한다.
										//코드는 ADIF와 ADCSRA를 비교해 값이 참이 '아닐' 경우 무한루프. (ADIF는 AD변환이 종료되고 데이터 레지스터가 업데이트되면 1로 set)
	return ADC;							//결론 : 4번째 비트가 1인지 ADCSRA와 계속 비교하며, 조건문이 거짓일경우 탈출.
}

int main(void)
{
	int read;
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART1_init();						//UART통신 초기화
	ADC_init(1);						//ADC변환 초기화(프리러닝)
	
	DDRE = 0xFF;						//0b11111111 (255)포트 B를 출력으로 설정
	
    while (1) 
    {
		read = read_ADC();				//가변저항 값 읽기 (1~1024)
		
		uint8_t pattern = 0;			//uint8_t = unsigned char (0~255)
		int LED_count = read >> 7;//)+ 1;	//ADC로 읽어들인 값을 오른쪽으로 7만큼 비트쉬프트 한다. (Atmega128의 ADC는 10bit의 분해능을 가진다.)
											//예) 598 = 10 0101 0110 =bit shift=> 00 0000 0100 으로 상수값 4이다.
											//필요 저장공간이 8비트를 초과하기 때문에 int형으로 선언한다.
											//+1은 8개의LED가 모두 켜지기 위한, 그리고 최소 1개는 켜지기 위한 덧샘이다.
		
		for (int i = 0; i< LED_count; i++)	//LED_count 보다 i값이 커질때까지 반복한다.
		{
			pattern |= (0x01 << i);			//LED_count의 값 만큼 pattern을 1로만든다.
											//ex) LED_count=4 일 경우
											//    1 과 0을 or 연산, 10과 1을 or연산, 100과 11을 or연산, 1000과 111을 or연산. 총 4번 반복
											//    결과로 변수 pattern은 0x0f(0b1111)의 값을 가진다.
		}
		
		PORTE = pattern;					//PORTB 레지스터를 0000 1111 로 set. 4개의 LED점등.
		
		printf("Read : %d,\t%d LEDs are ON!\r\n", read, LED_count);
		_delay_ms(5);
    }
	
	return 0;
}


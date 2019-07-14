///*
 //* ATmega128 Example.c
 //*
 //* Created: 2017-12-09 오후 1:50:15
 //* Author : njg71
 //*/ 
//#define F_CPU 16000000L
//#include <avr/io.h>
//#include <util/delay.h>
//
////#define _BV(bit) (1<<bit)
//
//void UART1_init(void)
//{
	//UBRR1H=0x00;				// 9,600 보율로 설정
	//UBRR1L=207;
	//UCSR1A |= _BV(U2X1);		// 2배속 모드
	//UCSR1C |= 0x06;
	//
	//UCSR1B |= _BV(RXEN1);		// 송수신 가능
	//UCSR1B |= _BV(TXEN1);
//}
//
//void UART1_transmit(char data)
//{
	//
	//while( !(UCSR1A & (1<<UDRE1)) );	// 데이터 수신 대기
	//UDR1 = data;
//}
//
//unsigned char UART1_receive(void)
//{
	//
	//while( !(UCSR1A & (1<<RXC1)) );	// 데이터 수신 대기(RXC1이 1이 될 때까지 대기함. mean:RXC1이 1이
	//return UDR1;
//}
//
//int main(void)
//{
	//UART1_init();
    //
    //while (1) 
    //{
		//UART1_transmit(UART1_receive());
    //}
	//
	//return 0;
//}
//

// 전자공작 : http://cafe.naver.com/circuitsmanual
// DateTime : 2018-03-20 오후 3:57:05
// by Ok-Hyun Park
#define F_CPU 16000000UL // 16 MHz
#include <avr/io.h>
#include <util/delay.h>
//
#define nop2 {asm volatile(" NOP "); asm volatile(" NOP ");}
#define nop8 {nop2; nop2; nop2; nop2;}
//
#define ws2812b_1  PORTB|=4  //PORTB.2
#define ws2812b_0  PORTB&=~4 //PORTB.2
//
void byte_out(unsigned char d){
	unsigned char i;
	for(i=0;i<8;i++){
		if(d&0x80){ ws2812b_1; nop8; ws2812b_0; }
		else      { ws2812b_1; nop2; ws2812b_0; }
		d<<=1;
	}
}
//
int main(void){
	int x,y,z;
	unsigned char r,g,b;
	DDRB=4;
	//
	while(1){
		for(x=0;x<256;x++){
			for(y=0;y<256;y++){
				for(z=0;z<256;z++){
					g=(unsigned char)y; r=(unsigned char)x; b=(unsigned char)z;
					byte_out(g);        byte_out(r);        byte_out(b);
					_delay_ms(10);
				}
			}
		}
	}
}
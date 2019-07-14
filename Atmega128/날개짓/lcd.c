#asm
.equ __lcd_port=0x15                // PORTC의 경우 
#endasm

#include <mega128.h>
#include <delay.h>
#include <stdio.h>
#include <lcd.h>

int KeyMatrix(unsigned char in); 		// 키 값에서 FND 점등을 위한 값으로 변환함수
// '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'dot'
unsigned char fnd[17]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e, 0x7f}; 

void main(void)
{ 
               int keyout;                                              // 키 메트릭스 행스캔 값 변수 
	int i;	
	unsigned char matrix;
               int matrix_fast;
               int wkflt=0; 
	PORTB = 0xFF; DDRB = 0xFF;
	PORTA = 0xFF; DDRA = 0x0F;	// PORTA0,1,2,3출력, 4,5,6,7 입력으로
	PORTB = ~fnd[0];               // 초기 "0" 표시
               lcd_init(16);
              while(1) 
	{      
	        keyout = 0xfe; 			// 키 메트릭스 초기 행스캔 값 : 1110 
	        for(i = 0; i<=3; i++)
	        {
		PORTA = keyout; 		// 행 스캔 출력
		matrix = PINA;
                              matrix_fast=KeyMatrix(matrix);
		//PORTB = ~fnd[ KeyMatrix(matrix) ]; // 키 값 -> FND 표시 
		keyout = (keyout<<1) + 0x01;   // 스캔데이터 갱신 1110 ->1101 ->1011 ->0111 
			 	
	        }
                       if (matrix_fast==0x0F)
                          {
                           lcd_clear(); 
                          }
                       else if(matrix_fast)
                       {  
                        lcd_gotoxy(wkflt,0);
                        lcd_putchar(matrix_fast);
                          
                        //int key=KeyMatrix();
                        //int value=0;
                        //key=KeyMatrix();
                        //if(key!=INVALID_NUM)
                        //{
                        // value=key;
                        // continue;
                        //}
                        //if(value==0x0f)
                        //  clear
                        //else if(value<=9)
                        //{
                        // value=value*10;
                        //}
                       }   
                          
                          
                //delay_ms(1);       
	} 

}


int KeyMatrix(unsigned char in) 	// 키 값에서 FND 점등을 위한 값으로 변환 함수
{
	// static: case에 들어가지 않는 잘못된 입력이 오면 기존 값 유지
	static unsigned char key = 0;
	switch(in){
	 	case 0x7E : key = 0;  break;
		case 0x7D : key = 1;  break;
	 	case 0x7B : key = 2;  break;
	 	case 0x77 : key = 3;  break;
	 	case 0xBE : key = 4;  break;
	 	case 0xBD : key = 5;  break;
	 	case 0xBB : key = 6;  break;
	 	case 0xB7 : key = 7;  break;
	 	case 0xDE : key = 8;  break;
	 	case 0xDD : key = 9;  break;
	 	case 0xDB : key = 0x0A;  break;
	 	case 0xD7 : key = 0x0B;  break;
	 	case 0xEE : key = 0x0C;  break; 			
	 	case 0xED : key = 0x0D;  break;
	 	case 0xEB : key = 0x0E;  break;			
	 	case 0xE7 : key = 0x0F;  break;
	}
	return key;
}

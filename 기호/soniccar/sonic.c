#include <mega128a.h>
#include <delay.h>
#include <stdio.h>
#include <stdlib.h>
#asm
.equ __lcd_port=0x1B     // PORTC의 경우 
#endasm
#include <lcd.h>        // LCD 제어함수를 사용하기 위한 헤더파일
#define USTrgger        PORTC.0             // 트리거 펄스 출력 핀
#define USEcho          PINC.1                // 에코 신호 입력 핀
#define SOUND_SPEED_mm  (340000.f)  // 340m/s
#define OVF_PERIOD_s    (0.000016f)     // = 1*256/16MHz = 16us
/*
int MeasureDistanceByUSS();
long ovf_count;
long rise_time, fall_time;

interrupt [TIM0_OVF] void timer0_ovf_isr(void) // interr_countr 0 overflow interrupt 
{
    ovf_count++;    // 매 16μs 마다 인터럽트 발생
} 
void main(void)
{
    char sbuf[16];
    int distance;
    PORTA = 0xff;  DDRA = 0xff;      // LCD
    PORTC = 0x02;  DDRC = 0x01;    // PC0: 트리거 출력, PC1: 에코 입력
    PORTD = 0xff;  DDRD = 0xff;
    TCCR0 = 0x01;             // normal mode, prescaler: 1
    lcd_init(16);
    #asm("sei")
        
    while (1)
    {
        distance = MeasureDistanceByUSS();    // 초음파 발생 거리 구함
        lcd_clear();
        delay_ms(1);
        lcd_gotoxy(0,0);
        sprintf(sbuf, "Dist:%d mm", distance);  // 거리 값 디스플레이
        lcd_puts(sbuf); 
        delay_ms(50);
        
        if(distance<100 && distance >=40)
        {
         //Beep Beep
         PORTD.0=0x01;
         delay_ms(distance*5);
         PORTD.0=0x00;
        }
        else
        {
         PORTD.0=0x01;
        }
    }
}
int MeasureDistanceByUSS(void)    // 초음파센서 작동하여 거리 구하기
{
    static float old_distance = 0.f;
    float distance;
    ovf_count = 0;
    TIMSK = 0x01;     // timer0 overflow interrupt start
    USTrgger = 1;     // 트리거 펄스(10μs) 발생 
    delay_us(10);
    USTrgger = 0;
    
    while( (USEcho == 0) && (ovf_count <= 2200) ); // 약 12미터가 넘어가면, 루프 탈출
    rise_time = ovf_count;
    while( (USEcho == 1) && (ovf_count <= 2200) );
    fall_time = ovf_count;
    
    TIMSK=0x00;        // timer interrupt stop
    distance = SOUND_SPEED_mm*OVF_PERIOD_s*(fall_time-rise_time)/2.f; 
    if(distance < 40)
        distance = old_distance;     // 거리 값이 50mm이하이면 이전데이터 리턴
    else
        old_distance = distance;     // 지금의 데이터를 예전 데이터변수로 전달
    return (int)distance;
}
*/
/*
//DC PWM

          //  "0",    "1",   "2",   "3",    "4",   "5",    "6",   "7",    "8",   "9"

unsigned char FND[10]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};

 

void MPUinit(void)  // ============ 초기화 함수 =============

{

    PORTG=0x00;  DDRG=0xFF;    // Port A 모터 

    PORTD=0xFF;  DDRD=0xFF;     // Port B FND

    PORTC=0xFF;  DDRC=0x00;    // Port C 스위치 입력

    PORTB=0xFF;  DDRB=0xFF;

    

    TCCR0 = 0x6c;   // 01101100  고속PWM, 256분주 
    TCNT0 = 0x00;
    
    TCCR2 = 0x6c;   // 01101100  고속PWM, 256분주 
    TCNT2 = 0x00;
}

void main(void)     //============= 메인 함수 ============

{

    unsigned char buff0;    // 스위치 입력값 저장

    MPUinit();

    OCR0 = 0;             // 모터 정지 : duty ratio = 0% 
    OCR2 = 0;             // 모터 정지 : duty ratio = 0%
    PORTG  = 0xF5;        // 정회전 모드 : 1(1)11 0101

     

    while (1)

    {

        buff0 = PINC & 0x1F;    // 스위치 값 읽음 

        delay_ms(5);

        switch(buff0)

        {

        case 0x1E:            // SW0이 눌리면(0001 1110)

            OCR0 = 76;    // duty 비 =  30%
            OCR2 = 76;    // duty 비 =  30%
            PORTD = ~FND[1]; // FND “1“

            break;

        case 0x1D:            // SW1이 눌리면(0001 1101)

           OCR0 = 152;   // duty 비 = 60%
           OCR2 = 152;   // duty 비 = 60%
            PORTD = ~FND[2]; // FND “2“ 

            break;

        case 0x1B:            // SW2이 눌리면(0001 1011)

            OCR0 = 255;  // duty 비 = 100%
            OCR2 = 255;   // duty 비 = 100%
            PORTD = ~FND[3]; // FND “3“

            break;

        case 0x17:            // SW3이 눌리면 (0001 0111)

            OCR0 = 0;   // duty 비 = 0%
            OCR2 = 0;   // duty 비 = 60%
            PORTD = ~FND[4]; // FND “4“

            break;
        }

    };

}
*/
//new 19.05.29
 //  "0",    "1",   "2",   "3",    "4",   "5",    "6",   "7",    "8",   "9"
#define Q0        PORTB.0	//PortB.0 bit : FND Q0
#define Q1        PORTB.1 	//PortB.1 bit : FND Q1
#define Q2        PORTB.2 	//PortB.2 bit : FND Q2
#define Q3        PORTB.3 	//PortB.3 bit : FND Q3

unsigned char fnd[10]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};

int MeasureDistanceByUSS();
long ovf_count;
long rise_time, fall_time;
//int i;

interrupt [TIM0_OVF] void timer0_ovf_isr(void) // interr_countr 0 overflow interrupt 
{
    ovf_count++;    // 매 16μs 마다 인터럽트 발생
}

void MPUinit(void)  // ============ 초기화 함수 =============

{
    PORTA = 0xff;  DDRA = 0xff;      // LCD
    PORTB=0xF0;  DDRB=0xFF; //PWM
    PORTC = 0x02;  DDRC = 0x01;    // PC0: 트리거 출력, PC1: 에코 입력
    PORTD = 0xff;  DDRD = 0xff;
    PORTE = 0xff;  DDRE = 0xff;
    PORTG=0x00;  DDRG=0xFF;    // Port A 모터 

    //TCCR0 = 0x6c;   // 01101100  고속PWM, 256분주 
    //TCNT0 = 0x00;
    
    //TCCR2 = 0x6c;   // 01101100  고속PWM, 256분주 
    //TCNT2 = 0x00;
}
void FFND(int j)
{
 int i=j;
        PORTE = ~fnd[i/1000];		// i 값 천 단위 계산 FND3 만 점등
		Q0=1,Q1=1,Q2=1,Q3=0; 
		delay_ms(1);
		
		PORTE = ~fnd[i%1000/100]; 	// i 값 백 단위 계산 FND2 만 점등 
		Q0=1,Q1=1,Q2=0,Q3=1; 		
		delay_ms(1); 
		 		
		PORTE = ~fnd[i%100/10]; 	// i 값 십 단위 계산 FND1 만 점등
		Q0=1,Q1=0,Q2=1,Q3=1; 		
		delay_ms(1); 
		 		
		PORTE = ~fnd[i%10/1]; 	// i 값 일 단위 계산 FND0 만 점등
		Q0=0,Q1=1,Q2=1,Q3=1;
		delay_ms(1);
} 
void main(void)
{
    char sbuf[16];
    int distance;
   
    MPUinit();
    
    TCCR0 = 0x01;             // normal mode, prescaler: 1 
    
    TCCR2 = 0x6c;   // 01101100  고속PWM, 256분주 
    TCNT2 = 0x00;
    OCR2 = 187;             // 모터 정지 : duty ratio = 0%
    
    lcd_init(16);
    #asm("sei")
        
    while (1)
    {
        distance = MeasureDistanceByUSS();    // 초음파 발생 거리 구함
        
        lcd_clear();
        delay_ms(1);
        lcd_gotoxy(0,0);
        sprintf(sbuf, "Dist:%d mm", distance);  // 거리 값 디스플레이
        lcd_puts(sbuf); 
        delay_ms(50); 
        
         //Beep Beep
         if(distance<200 && distance >=30)
         {
          PORTD.0=0x01;
          delay_ms(distance);
          PORTD.0=0x00;
         }
         else
         {
          PORTD.0=0x01;
         }
         
        
        if(distance>30&&distance<70)
        {
          OCR2 = 254;
          PORTG  = 0xFA;       // 역회전 모드 : 1(1)11 1010
          delay_ms(1000);
          PORTD.0=0x01;
    
          PORTG = 0xF6 ; //1111 0110 
          delay_ms(1000);
           
        } 
        else
        {
         OCR2 = 187;
         PORTG  = 0xF5;        // 정회전 모드 : 1(1)11 0101
        }
        FFND(distance);
    }
}

// 초음파센서 작동하여 거리 구하기
int MeasureDistanceByUSS(void)    
{
    static float old_distance = 0.f;
    float distance;
    ovf_count = 0;
    TIMSK = 0x01;     // timer0 overflow interrupt start
    USTrgger = 1;     // 트리거 펄스(10μs) 발생 
    delay_us(10);
    USTrgger = 0;
    
    while( (USEcho == 0) && (ovf_count <= 2200) ); // 약 12미터가 넘어가면, 루프 탈출
    rise_time = ovf_count;
    while( (USEcho == 1) && (ovf_count <= 2200) );
    fall_time = ovf_count;
    
    TIMSK=0x00;        // timer interrupt stop
    distance = SOUND_SPEED_mm*OVF_PERIOD_s*(fall_time-rise_time)/2.f; 
    if(distance < 40)
        distance = old_distance;     // 거리 값이 50mm이하이면 이전데이터 리턴
    else
        old_distance = distance;     // 지금의 데이터를 예전 데이터변수로 전달
    return (int)distance;
}
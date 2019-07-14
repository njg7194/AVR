
/* 이 AVR코드는 Pololu RGB LED스트립을 구동시키기 위한 코드입니다.
   임의 수의 LED색상을 완전히 제어할 수 있습니다.
   이 구현은 인라인 어셈블리와 비트 전송을 수행하는 동안 인터럽트를 비활성화한다.
 */

/* 이 줄은 AVR실행 주파수를 지정합니다.
   This code supports 20 MHz, 16 MHz and 8MHz */
#define F_CPU 16000000

// 이 라인은 LED스트립의 핀 위치를 지정합니다.
// LED스트립의 데이터 라인을 PC0에 부착하거나
// 코드를 수정하여 알맞은 데이터 라인을 지정해야 합니다.
#define LED_STRIP_PORT PORTC
#define LED_STRIP_DDR  DDRC
#define LED_STRIP_PIN  0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/** 아래의 rgb_color구조체는 8비트 RGBLED의 색상을 나타냅니다.
    Examples:
      Black:      (rgb_color){ 0, 0, 0 }
      Pure red:   (rgb_color){ 255, 0, 0 }
      Pure green: (rgb_color){ 0, 255, 0 }
      Pure blue:  (rgb_color){ 0, 0, 255 }
      White:      (rgb_color){ 255, 255, 255} */
typedef struct rgb_color
{
  unsigned char red, green, blue;
} rgb_color;

/** led_strip_write는 LED스트립에 일련의 색상을 전송하여 LED를 업데이트합니다.
 색상 매개 변수는 전송할 색상을 고정하는 RGD_color구조 배열을 가리켜야 합니다.
 카운트 매개 변수는 전송할 색상 수입니다.

 이 함수는 30개의 LED를 업데이트하는 데 약 1.1ms가 걸립니다.
 이 시간동안 인터럽트는 비활성화되어야 하므로 모든 인터럽트 기반 라이브러리는
 이 함수에 의해 부정적인 영향을 받을 수 있습니다.

 20 MHz일 때 Timing디테일  (16Mhz와 8Mhz에서는 약간씩 다를수 있음):
  0 pulse  = 400 ns
  1 pulse  = 850 ns
  "period" = 1300 ns
 */
void __attribute__((noinline)) led_strip_write(rgb_color * colors, unsigned int count) 
{
  // 핀을 LOW로 설정합니다.
  LED_STRIP_PORT &= ~(1<<LED_STRIP_PIN);
  LED_STRIP_DDR |= (1<<LED_STRIP_PIN);

  cli();   // 펄스 타이밍이 엉망이 되는 것을 원치 않기 때문에 인터럽트를 일시적으로 비활성화합니다.
  while(count--)
  {
    // LED strip으로 색상을 보냅니다.
    // 또한 아래의 어셈블리는 'color'포인터를 증가시키며,
    // 이 루프의 끝에 있는 다음 색상을 가리킵니다.
    asm volatile(
        "ld __tmp_reg__, %a0+\n"
        "ld __tmp_reg__, %a0\n"
        "rcall send_led_strip_byte%=\n"  // 빨간색 구성 요소를 보냅니다.
        "ld __tmp_reg__, -%a0\n"
        "rcall send_led_strip_byte%=\n"  // 초록색 구성 요소를 보냅니다.
        "ld __tmp_reg__, %a0+\n"
        "ld __tmp_reg__, %a0+\n"
        "ld __tmp_reg__, %a0+\n"
        "rcall send_led_strip_byte%=\n"  // 파란색 구성 요소를 보냅니다.
        "rjmp led_strip_asm_end%=\n"     // 어셈블리 서브 루틴을 통과합니다.

        // send_led_strip_byte subroutine:  바이트를 LED스트립으로 보냅니다.
        "send_led_strip_byte%=:\n"
        "rcall send_led_strip_bit%=\n"  // 가장 중요한 비트(비트 7)를 전송합니다.
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"  // 최하위 비트(비트 0)를 전송합니다.
        "ret\n"

        // send_led_strip_bit subroutine:  데이터 라인을 구동하여 LED스트립에 단일 비트를 전송합니다.
        // high for some time.  The amount of time the line is high depends on whether the bit is 0 or 1,
        // but this function always takes the same time (2 us).
        "send_led_strip_bit%=:\n"
#if F_CPU == 8000000
        "rol __tmp_reg__\n"                      // Rotate left through carry.
#endif
        "sbi %2, %3\n"                           // Drive the line high.

#if F_CPU != 8000000
        "rol __tmp_reg__\n"                      // Rotate left through carry.
#endif

#if F_CPU == 16000000
        "nop\n" "nop\n"
#elif F_CPU == 20000000
        "nop\n" "nop\n" "nop\n" "nop\n"
#elif F_CPU != 8000000
#error "Unsupported F_CPU"
#endif

        "brcs .+2\n" "cbi %2, %3\n"              // If the bit to send is 0, drive the line low now.

#if F_CPU == 8000000
        "nop\n" "nop\n"
#elif F_CPU == 16000000
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
#elif F_CPU == 20000000
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n"
#endif

        "brcc .+2\n" "cbi %2, %3\n"              // If the bit to send is 1, drive the line low now.

        "ret\n"
        "led_strip_asm_end%=: "
        : "=b" (colors)
        : "0" (colors),         // %a0 : 표시할 다음 색을 가리킵니다.
          "I" (_SFR_IO_ADDR(LED_STRIP_PORT)),   // %2 : 포트 레지스터(예:PORTC)
          "I" (LED_STRIP_PIN)     // %3 : 핀 번호(0-8)
    );

    // 각 색상 간의 언터럽트를 일시적으로 활성화하려면 아래의 주석을 해제하십시오.
    //sei(); asm volatile("nop\n"); cli();
  }
  sei();          // 끝났으니 인터럽트 허용함.
  _delay_us(80);  // 리셋 신호 보냄.
}

#define LED_COUNT 3
rgb_color colors[LED_COUNT];

int main()
{
	

	while(1)
	{
		unsigned int i;
		for(i = 0; i < LED_COUNT; i++)
		{
			unsigned char x = 8*i;
			colors[i] = (rgb_color){ x, 255 - x, x };
		}

		led_strip_write(colors, LED_COUNT);

		_delay_ms(20);
	}
}
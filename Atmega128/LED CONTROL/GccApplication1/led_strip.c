
/* �� AVR�ڵ�� Pololu RGB LED��Ʈ���� ������Ű�� ���� �ڵ��Դϴ�.
   ���� ���� LED������ ������ ������ �� �ֽ��ϴ�.
   �� ������ �ζ��� ������� ��Ʈ ������ �����ϴ� ���� ���ͷ�Ʈ�� ��Ȱ��ȭ�Ѵ�.
 */

/* �� ���� AVR���� ���ļ��� �����մϴ�.
   This code supports 20 MHz, 16 MHz and 8MHz */
#define F_CPU 16000000

// �� ������ LED��Ʈ���� �� ��ġ�� �����մϴ�.
// LED��Ʈ���� ������ ������ PC0�� �����ϰų�
// �ڵ带 �����Ͽ� �˸��� ������ ������ �����ؾ� �մϴ�.
#define LED_STRIP_PORT PORTC
#define LED_STRIP_DDR  DDRC
#define LED_STRIP_PIN  0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/** �Ʒ��� rgb_color����ü�� 8��Ʈ RGBLED�� ������ ��Ÿ���ϴ�.
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

/** led_strip_write�� LED��Ʈ���� �Ϸ��� ������ �����Ͽ� LED�� ������Ʈ�մϴ�.
 ���� �Ű� ������ ������ ������ �����ϴ� RGD_color���� �迭�� �����Ѿ� �մϴ�.
 ī��Ʈ �Ű� ������ ������ ���� ���Դϴ�.

 �� �Լ��� 30���� LED�� ������Ʈ�ϴ� �� �� 1.1ms�� �ɸ��ϴ�.
 �� �ð����� ���ͷ�Ʈ�� ��Ȱ��ȭ�Ǿ�� �ϹǷ� ��� ���ͷ�Ʈ ��� ���̺귯����
 �� �Լ��� ���� �������� ������ ���� �� �ֽ��ϴ�.

 20 MHz�� �� Timing������  (16Mhz�� 8Mhz������ �ణ�� �ٸ��� ����):
  0 pulse  = 400 ns
  1 pulse  = 850 ns
  "period" = 1300 ns
 */
void __attribute__((noinline)) led_strip_write(rgb_color * colors, unsigned int count) 
{
  // ���� LOW�� �����մϴ�.
  LED_STRIP_PORT &= ~(1<<LED_STRIP_PIN);
  LED_STRIP_DDR |= (1<<LED_STRIP_PIN);

  cli();   // �޽� Ÿ�̹��� ������ �Ǵ� ���� ��ġ �ʱ� ������ ���ͷ�Ʈ�� �Ͻ������� ��Ȱ��ȭ�մϴ�.
  while(count--)
  {
    // LED strip���� ������ �����ϴ�.
    // ���� �Ʒ��� ������� 'color'�����͸� ������Ű��,
    // �� ������ ���� �ִ� ���� ������ ����ŵ�ϴ�.
    asm volatile(
        "ld __tmp_reg__, %a0+\n"
        "ld __tmp_reg__, %a0\n"
        "rcall send_led_strip_byte%=\n"  // ������ ���� ��Ҹ� �����ϴ�.
        "ld __tmp_reg__, -%a0\n"
        "rcall send_led_strip_byte%=\n"  // �ʷϻ� ���� ��Ҹ� �����ϴ�.
        "ld __tmp_reg__, %a0+\n"
        "ld __tmp_reg__, %a0+\n"
        "ld __tmp_reg__, %a0+\n"
        "rcall send_led_strip_byte%=\n"  // �Ķ��� ���� ��Ҹ� �����ϴ�.
        "rjmp led_strip_asm_end%=\n"     // ����� ���� ��ƾ�� ����մϴ�.

        // send_led_strip_byte subroutine:  ����Ʈ�� LED��Ʈ������ �����ϴ�.
        "send_led_strip_byte%=:\n"
        "rcall send_led_strip_bit%=\n"  // ���� �߿��� ��Ʈ(��Ʈ 7)�� �����մϴ�.
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"  // ������ ��Ʈ(��Ʈ 0)�� �����մϴ�.
        "ret\n"

        // send_led_strip_bit subroutine:  ������ ������ �����Ͽ� LED��Ʈ���� ���� ��Ʈ�� �����մϴ�.
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
        : "0" (colors),         // %a0 : ǥ���� ���� ���� ����ŵ�ϴ�.
          "I" (_SFR_IO_ADDR(LED_STRIP_PORT)),   // %2 : ��Ʈ ��������(��:PORTC)
          "I" (LED_STRIP_PIN)     // %3 : �� ��ȣ(0-8)
    );

    // �� ���� ���� ���ͷ�Ʈ�� �Ͻ������� Ȱ��ȭ�Ϸ��� �Ʒ��� �ּ��� �����Ͻʽÿ�.
    //sei(); asm volatile("nop\n"); cli();
  }
  sei();          // �������� ���ͷ�Ʈ �����.
  _delay_us(80);  // ���� ��ȣ ����.
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
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include <stdio.h>

#include "../avr-uart/uart.h"
#include "../lcd/lcd.h"

#include "util.h"
#include "millis.h"
#include "pwm.h"

char buf[16];
char buf2[16];
pwm_out_t pwm_out;

int main(){
  uart0_init(UART_BAUD_SELECT(9600, F_CPU));
  lcd_init(LCD_DISP_ON);
  millis_init();

  pwm_init(&pwm_out, &PORTD, PD5, PWM_TIMER1, PWM_CHAN_A, PWM_TRIANGLE, 0,
           128, 10, PWM_DIR_NONE);

  wdt_enable(WDTO_2S);
  sei();

  while (1){
    pwm_update(&pwm_out);

    uint16_t d = uart0_getc();
    if ((d >> 8) == 0){
      switch (d){
      case '1':
        pwm_enable(&pwm_out);
        break;
      case '0':
        pwm_disable(&pwm_out);
        break;
      }
    }

    sprintf(buf, "%3u", OCR1A);
    lcd_home();
    lcd_puts(buf);

    lcd_gotoxy(0, 1);
    millis_pretty_print(buf2);
    lcd_puts(buf2);
    
    wdt_reset();
  }

  return 0;
}


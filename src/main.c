#include <avr/io.h>
#include <avr/interrupt.h>

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

  // Initialize PWM stuff
  pwm_init(&pwm_out, &PORTD, PD5, PWM_TIMER1, PWM_CHAN_A, PWM_TRIANGLE, 0,
           128, 10, PWM_DIR_NONE);

  sei();

  while (1){

    // Must be called at each main loop
    pwm_update(&pwm_out);

    // Write '0' to serial to disable, '1' to enable
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

    // Show PWM val on LCD
    sprintf(buf, "%3u", *pwm_out.pwm_val);
    lcd_home();
    lcd_puts(buf);

    // Show runtime
    lcd_gotoxy(0, 1);
    millis_pretty_print(buf2);
    lcd_puts(buf2);
  }

  return 0;
}


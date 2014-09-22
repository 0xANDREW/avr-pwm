#include <avr/io.h>

#include "pwm.h"
#include "util.h"
#include "millis.h"

uint32_t _now;

// Currently written for ATmega164A
void pwm_timer_init(pwm_out_t* pwm_out){
  volatile uint8_t* timer_a = 0;
  volatile uint8_t* timer_b = 0;
  uint8_t com;

  switch (pwm_out->timer){
  case PWM_TIMER0:
    timer_a = &TCCR0A;
    timer_b = &TCCR0B;
    pwm_out->pwm_val = (pwm_out->channel == PWM_CHAN_A) ? &OCR0A: &OCR0B;
    break;

  case PWM_TIMER1:
    timer_a = &TCCR1A;
    timer_b = &TCCR1B;

    // Timer1 is 16-bit, so use low-order registers
    pwm_out->pwm_val = (pwm_out->channel == PWM_CHAN_A) ? &OCR1AL: &OCR1BL;
    break;

  case PWM_TIMER2:
    timer_a = &TCCR2A;
    timer_b = &TCCR2B;
    pwm_out->pwm_val = (pwm_out->channel == PWM_CHAN_A) ? &OCR2A: &OCR2B;
    break;
  }

  switch (pwm_out->channel){
  case PWM_CHAN_A:
    com = COM0A1;
    break;

  case PWM_CHAN_B:
    com = COM0B1;
    break;
  }

  // com: Clear OCnX on compare match
  // CS00: no prescaler
  // WGM00 | WGM02: fast PWM, 8-bit
  *timer_a |= _BV(com) | _BV(WGM00);
  *timer_b |= _BV(CS00) | _BV(WGM02);
}

void pwm_init(pwm_out_t* pwm_out, volatile uint8_t* port, uint8_t pin, 
              uint8_t timer, uint8_t channel, pwm_type_t type, 
              uint8_t start_val, uint8_t end_val, uint16_t delay, 
              pwm_dir_t dir){

  pwm_out->port = port;
  pwm_out->pin = pin;
  pwm_out->type = type;
  pwm_out->start_val = start_val;
  pwm_out->end_val = end_val;
  pwm_out->delay = delay;
  pwm_out->dir = dir;
  pwm_out->timer = timer;
  pwm_out->channel = channel;
  pwm_out->active = 1;

  util_set_ddr(port, pin, UTIL_DDR_OUTPUT);
  pwm_timer_init(pwm_out);

  switch (type){
  case PWM_SET:
  case PWM_TRIANGLE:
    *pwm_out->pwm_val = start_val;
    break;

  case PWM_LINEAR:
    *pwm_out->pwm_val = (dir == PWM_DIR_UP) ? start_val: end_val;
    break;
  }
}

void pwm_update(pwm_out_t *pwm_out){
  if (!pwm_out->active){
    return;
  }

  _now = millis();

  if (pwm_out->type == PWM_SET){
    pwm_set(pwm_out);
    return;
  }

  if (_now - pwm_out->last > pwm_out->delay){
    pwm_out->last = _now;

    switch (pwm_out->type){
    case PWM_TRIANGLE:
      pwm_update_triangle(pwm_out);
      break;

    case PWM_LINEAR:
      pwm_update_linear(pwm_out);
      break;

    case PWM_SET:
      break;
    }
  }
}

void pwm_set(pwm_out_t* pwm_out){
  *pwm_out->pwm_val = pwm_out->start_val;
}

void pwm_update_linear(pwm_out_t* pwm_out){
  switch (pwm_out->dir){
  case PWM_DIR_UP:
    *pwm_out->pwm_val = (*pwm_out->pwm_val == pwm_out->end_val) ? 0: *pwm_out->pwm_val + 1;
    break;
  case PWM_DIR_DOWN:
    *pwm_out->pwm_val = (*pwm_out->pwm_val == 0) ? pwm_out->end_val: *pwm_out->pwm_val - 1;
    break;
  case PWM_DIR_NONE:
    break;
  }
}

void pwm_update_triangle(pwm_out_t* pwm_out){
  if (*pwm_out->pwm_val == pwm_out->end_val){
    pwm_out->dir = PWM_DIR_DOWN;
  }
  else if (*pwm_out->pwm_val == pwm_out->start_val){
    pwm_out->dir = PWM_DIR_UP;
  }

  pwm_update_linear(pwm_out);
}

void pwm_enable(pwm_out_t* pwm_out){
  pwm_out->active = 1;
  *pwm_out->pwm_val = pwm_out->start_val;
}

void pwm_disable(pwm_out_t* pwm_out){
  pwm_out->active = 0;
  *pwm_out->pwm_val = 0;
}

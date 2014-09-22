#ifndef PWM_H
#define PWM_H

#include <inttypes.h>

typedef enum {
  PWM_TIMER0,
  PWM_TIMER1,
  PWM_TIMER2
} pwm_timer_t;

typedef enum {
  PWM_CHAN_A,
  PWM_CHAN_B
} pwm_chan_t;

typedef enum { 
  PWM_TRIANGLE, 
  PWM_LINEAR,
  PWM_SET
} pwm_type_t;

typedef enum {
  PWM_DIR_UP,
  PWM_DIR_DOWN,
  PWM_DIR_NONE
} pwm_dir_t;

typedef struct {
  pwm_type_t type;
  uint16_t delay;
  uint32_t last;
  volatile uint8_t* port;
  uint8_t pin;
  uint8_t start_val;
  uint8_t end_val;
  pwm_dir_t dir;
  uint8_t timer;
  uint8_t channel;
  uint8_t active;
  volatile uint8_t* pwm_val;
} pwm_out_t;

void pwm_init(pwm_out_t* pwm_out, volatile uint8_t* port, uint8_t pin, 
              uint8_t timer, uint8_t channel, pwm_type_t type, 
              uint8_t start_val, uint8_t end_val, uint16_t delay, 
              pwm_dir_t dir);

void pwm_timer_init(pwm_out_t *pwm_out);
void pwm_update(pwm_out_t* pwm_out);
void pwm_update_linear(pwm_out_t* pwm_out);
void pwm_update_triangle(pwm_out_t* pwm_out);
void pwm_set(pwm_out_t* pwm_out);
void pwm_enable(pwm_out_t* pwm_out);
void pwm_disable(pwm_out_t* pwm_out);

#endif

#include <avr/interrupt.h>
#include <util/atomic.h>

#include <stdio.h>

#include "millis.h"

void millis_init(){
  millis_val = 0;

#ifdef __AVR_ATmega162__
  
  // Configure timer 1 for CTC mode 
  TCCR0 |= _BV(WGM01);

  // Enable CTC interrupt 
  TIMSK |= (1 << OCIE0); 

  // Prescaler 64
  TCCR0 |= _BV(CS00) | _BV(CS01);

  // 1000Hz
  OCR0 = 124;

  // ATmega2560 (at 16MHz)
#else
  TCCR0A |= _BV(WGM01);
  TIMSK0 |= (1 << OCIE0A); 
  TCCR0B |= _BV(CS00) | _BV(CS01);
  OCR0A = 124;
#endif  
}

uint32_t millis(){
  uint32_t val;

  // Ensure this is read atomically to avoid timing errors
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    val = millis_val;
  }
  
  return val;
}

void millis_pretty_print(char* buf){
  uint32_t ms = millis();
  uint16_t min = ms / 1000 / 60;
  uint16_t sec = ms / 1000 % 60;

  sprintf(buf, "%03u:%02u", min, sec);
}

#ifdef __AVR_ATmega162__
ISR (TIMER0_COMP_vect){
  millis_val++;
}
#else
ISR (TIMER0_COMPA_vect){
  millis_val++;
}
#endif

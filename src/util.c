#include <avr/io.h>

#include "util.h"
/* #include "millis.h" */

void util_set_ddr(volatile uint8_t* port, uint8_t pin, uint8_t dir){
  volatile uint8_t* ddr = port - 1;

  if (dir == UTIL_DDR_INPUT){
    *ddr &= ~_BV(pin);
  }
  else {
    *ddr |= _BV(pin);
  }
}

void util_set_pin(volatile uint8_t* port, uint8_t pin, uint8_t val){
  if (val == 0){
    *port &= ~_BV(pin);
  }
  else {
    *port |= _BV(pin);
  }
}

uint8_t util_read_pin(volatile uint8_t* port, uint8_t pin){
  volatile uint8_t* port_input = port - 2;

  return *port_input & _BV(pin);
}

void util_toggle_pin(volatile uint8_t* port, uint8_t pin){
  *port ^= _BV(pin);
}

void util_enable_pullup(volatile uint8_t* port, uint8_t pin){
  util_set_ddr(port, pin, UTIL_DDR_INPUT);
  util_set_pin(port, pin, 1);
}

/* void util_get_time_array(char* buf){ */
/*   uint32_t now = millis(); */

/*   buf[0] = (now >> 24) & 0xFF; */
/*   buf[1] = (now >> 16) & 0xFF; */
/*   buf[2] = (now >> 8) & 0xFF; */
/*   buf[3] = now & 0xFF; */
/* } */

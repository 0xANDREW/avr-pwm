/** @file */

#ifndef UTIL_H
#define UTIL_H

#include <inttypes.h>

enum { UTIL_DDR_INPUT, UTIL_DDR_OUTPUT } util_ddr;

void    util_set_ddr(volatile uint8_t* port, uint8_t pin, uint8_t dir);
void    util_set_pin(volatile uint8_t* port, uint8_t pin, uint8_t val);
void    util_toggle_pin(volatile uint8_t* port, uint8_t pin);
void    util_enable_pullup(volatile uint8_t* port, uint8_t pin);
void    util_get_time_array(char* buf);
uint8_t util_read_pin(volatile uint8_t* port, uint8_t pin);

#endif

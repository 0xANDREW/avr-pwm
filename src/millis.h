/** @file */

#ifndef MILLIS_H
#define MILLIS_H

#include <inttypes.h>

volatile uint32_t millis_val;

/**
   Initialize millisecond counter timer.
*/
void millis_init();

/**
   Return current number of milliseconds since system start.
   @return  Number of milliseconds since system start.
*/
uint32_t millis();

void millis_pretty_print(char* buf);

#endif

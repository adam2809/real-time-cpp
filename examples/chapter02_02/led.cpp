///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2025.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// The LED program.

#include "mcal_reg.h"
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>

#include <cstdint>

enum port_direction
{
  eInput = 0U,
  eOutput = 1U
};

static inline void gpio_set_dir(volatile uint8_t* port, uint8_t bit_index, port_direction dir)
{
  *(port - 1U) |= (dir << bit_index);
}

static inline void gpio_toggle(volatile uint8_t* port, uint8_t bit_index)
{
  *(port) ^= (1U << bit_index);
}

static inline void gpio_high(volatile uint8_t* port, uint8_t bit_index)
{
  *(port) |= (1U << bit_index);
}

static inline void gpio_low(volatile uint8_t* port, uint8_t bit_index)
{
  *(port) &= ~(1U << bit_index);
}

static inline bool gpio_read(volatile uint8_t* port, uint8_t bit_index)
{
  return (*(port) >> bit_index & 1U);
}

auto main() -> int;

auto main() -> int
{

  gpio_set_dir(&PORTD, 5U, eOutput);
  gpio_set_dir(&PORTD, 3U, eOutput);
  gpio_set_dir(&PORTB, 1U, eInput);
  for (;;)
  {
    gpio_high(&PORTD, 5U);
    gpio_low(&PORTD, 3U);
    _delay_ms(500U);
    gpio_low(&PORTD, 5U);
    gpio_high(&PORTD, 3U);
    _delay_ms(500U);
  }
  
}

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

enum address_direction
{
  eInput = 0U,
  eOutput = 1U
};



static inline void set_bit_at_address(volatile uint8_t* address, uint8_t bit_index)
{
  *(address) |= (1U << bit_index);
}

static inline void toggle_bit_at_address(volatile uint8_t* address, uint8_t bit_index)
{
  *(address) ^= (1U << bit_index);
}

static inline void clear_bit_at_address(volatile uint8_t* address, uint8_t bit_index)
{
  *(address) &= ~(1U << bit_index);
}

static inline bool check_bit_at_address(volatile uint8_t* address, uint8_t bit_index)
{
  return (*(address) >> bit_index & 1U);
}

auto main() -> int;

auto main() -> int
{

  set_bit_at_address(&DDRD, 5U);
  set_bit_at_address(&DDRD, 3U);

  clear_bit_at_address(&DDRB, 1U);
  set_bit_at_address(&PORTB, 1U);

  _delay_ms(5U);
  for (;;)
  {
    if(!check_bit_at_address(&PINB, 1U))
    {
      set_bit_at_address(&PORTD, 5U);
      clear_bit_at_address(&PORTD, 3U);
      _delay_ms(500U);
      clear_bit_at_address(&PORTD, 5U);
      set_bit_at_address(&PORTD, 3U);
      _delay_ms(500U);
    }
    else
    {
      clear_bit_at_address(&PORTD, 5U);
      clear_bit_at_address(&PORTD, 3U);
    }
  }
}

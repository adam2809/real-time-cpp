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

#include <cstdint>

enum port_direction
{
  eInput = 0U,
  eOutput = 1U
};

enum port_address
{
  ePortB = 0x25U,
  ePortD = 0x2BU
};

static inline void gpio_set_dir(port_address port, uint8_t bit_index, port_direction dir)
{
  *((volatile uint8_t*) port - 1U) |= (dir << bit_index);
}

static inline void gpio_toggle(port_address port, uint8_t bit_index)
{
  *((volatile uint8_t*) port) ^= (1U << bit_index);
}

static inline void gpio_high(port_address port, uint8_t bit_index)
{
  *((volatile uint8_t*) port) |= (1U << bit_index);
}

static inline void gpio_low(port_address port, uint8_t bit_index)
{
  *((volatile uint8_t*) port) &= ~(1U << bit_index);
}

auto main() -> int;

auto main() -> int
{
  gpio_set_dir(ePortD, 5U, eOutput);
  gpio_set_dir(ePortD, 3U, eOutput);
  gpio_set_dir(ePortB, 5U, eOutput);

  gpio_high(ePortD, 5U);
  _delay_ms(1000);

  gpio_high(ePortD, 3U);
  _delay_ms(1000);

  gpio_low(ePortD, 3U);
  _delay_ms(1000);

  gpio_low(ePortD, 5U);
  _delay_ms(1000);

  gpio_toggle(ePortD, 5U);
  gpio_toggle(ePortD, 3U);
  gpio_toggle(ePortB, 5U);
  _delay_ms(1000);

  gpio_toggle(ePortD, 5U);
  gpio_toggle(ePortD, 3U);
  gpio_toggle(ePortB, 5U);
}

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

class led
{
public:
  // Use convenient class-specific typedefs.
  using port_type = std::uint8_t;
  using bval_type = std::uint8_t;

  // The led class constructor.
  explicit led(const port_type p, const bval_type b)
    : port(p),
      bval(b)
  {
    // Set the port pin value to low.
    *reinterpret_cast<volatile bval_type*>(port) &= static_cast<bval_type>(~bval);

    // Set the port pin direction to output.

    // Note that the address of the port direction
    // register is one less than the address
    // of the port value register.
    const auto pdir = static_cast<port_type>(port - 1U);

    *reinterpret_cast<volatile bval_type*>(pdir) |= bval;
  }

  auto toggle() const -> void
  {
    // Toggle the LED via direct memory access.
    *reinterpret_cast<volatile bval_type*>(port) ^= bval;
  }

private:
  // Private member variables of the class.
  const port_type port;
  const bval_type bval;
};

namespace
{
  // Create led_b5 on portb.5.
  led led_b5
  {
    mcal::reg::portb,
    mcal::reg::bval5
  };
  led led_red
  {
    mcal::reg::portd,
    mcal::reg::bval3
  };
  led led_blue
  {
    mcal::reg::portd,
    mcal::reg::bval5
  };
}

static inline void gpio_toggle(uint8_t port, uint8_t bit_index)
{
  *((volatile uint8_t*) port) ^= (1U << bit_index);
}

static inline void gpio_high(uint8_t port, uint8_t bit_index)
{
  *((volatile uint8_t*) port) |= (1U << bit_index);
}

static inline void gpio_low(uint8_t port, uint8_t bit_index)
{
  *((volatile uint8_t*) port) &= ~(1U << bit_index);
}

auto main() -> int;

auto main() -> int
{
  gpio_high(mcal::reg::portd, 5U);
  _delay_ms(1000);

  gpio_high(mcal::reg::portd, 3U);
  _delay_ms(1000);

  gpio_low(mcal::reg::portd, 3U);
  _delay_ms(1000);

  gpio_low(mcal::reg::portd, 5U);
  _delay_ms(1000);
  for (;;)
  {
    gpio_toggle(mcal::reg::portd, 5U);
    gpio_toggle(mcal::reg::portd, 3U);
    _delay_ms(1000);
  }
}

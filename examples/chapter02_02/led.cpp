///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2025.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// The LED program.

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>

#define BAUD 9600                     // Baud rate in bits per second (bps).
#define MY_UBRR F_CPU / 8 / BAUD - 1  // UART Baud Rate Register.

void usart_init(unsigned int ubrr) {
  // Set baud rate.
  UBRRH = (uint8_t)(ubrr >> 8);
  UBRRL = (uint8_t)(ubrr);
  // UBRRH = USART Baud Rate Register High.
  // UBRRL = USART Baud Rate Register Low.

  // Enable receiver and transmitter.
  UCSRB = (1 << RXEN) | (1 << TXEN);
  // UCSRB = USART Control and Status Register B.
  // RXEN = Receiver Enabled.
  // TXEN = Transmitter Enabled.

  // Double the USART transmission speed.
  UCSRA = (1 << U2X);

  // Set frame format: 1 stop bit, 8 data bits.
  UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
  // UCSRC = (1 << URSEL) | (1 << USBS) | (1 << UCSZ0) | (1 << UCSZ1);
  // UCSRC = USART Control and Status Register C.
  // URSEL = USART Register Select.
  // USBS = USART Stop Bit Select (0 -> 1b, 1 -> 2b).
  // USZ0 = USART Character Size.
}

void usart_transmit(uint8_t data) {
  // Wait for empty transmit buffer.
  while (!(UCSRA & (1 << UDRE))) {
  };
  // UCSRA = USART Control and Status Register A.
  // UDRE = USART Data Register Empty.

  // TODO: What about buffer overflow etc?
  UDR = data;
  // UDR = USART Data Register.
}

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

int main(void)
{
  usart_init(MY_UBRR);
  uint8_t data[] = {0x41, 0x42, 0x43, 0x44};

  set_bit_at_address(&DDRD, 5U);
  set_bit_at_address(&DDRD, 3U);

  clear_bit_at_address(&DDRB, 1U);
  set_bit_at_address(&PORTB, 1U);

  _delay_ms(5U);
  for (;;)
  {
    _delay_ms(3000U);
    set_bit_at_address(&PORTD, 5U);
    set_bit_at_address(&PORTD, 3U);
    for (int i = 0; i < 4; i++) {
      usart_transmit(data[i]);
      _delay_ms(500U);
    }
    clear_bit_at_address(&PORTD, 5U);
    clear_bit_at_address(&PORTD, 3U);

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

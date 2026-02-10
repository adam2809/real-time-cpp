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
  UBRR0H = (uint8_t)(ubrr >> 8);
  UBRR0L = (uint8_t)(ubrr);
  // UBRR0H = USART Baud Rate Register High.
  // UBRR0L = USART Baud Rate Register Low.

  // Enable receiver and transmitter.
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  // UCSR0B = USART Control and Status Register B.
  // RXEN0 = Receiver Enabled.
  // TXEN0 = Transmitter Enabled.

  // Double the USART transmission speed.
  UCSR0A = (1 << U2X0);

  // Set frame format: 1 stop bit, 8 data bits.
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
  // UCSR0C = USART Control and Status Register C.
  // UCSZ01, UCSZ00 = USART Character Size (8 bits).
}

void usart_transmit(uint8_t data) {
  while (!(UCSR0A & (1 << UDRE0))) {
  };
  UDR0 = data;
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
  // uint8_t data[] = {
  //   0x01, // device ID 1
  //   0x05, // function code 5 (write single coil)
  //   0x00, // address 0
  //   0x01, // value 1
  //   0x19,0xD0 // CRC16 checksum
  // };
  uint8_t data_pressed[] = {
    0x01U, 0x05U, 0x00U, 0x00U, 0x01U, 0xD8U ,0xCCU
  };
  uint8_t data_released[] = {
    0x01U, 0x05U, 0x00U, 0x00U, 0x00U, 0x19U, 0x0CU
  };

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
    for (int i = 0; i < 8; i++) {
      usart_transmit(data_released[i]);
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

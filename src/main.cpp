#include <Arduino.h>

void init()
{
  UCSR0C = 0b00000110;
  // 9600 BPS
  UBRR0 = 103;

  // Enable the Receiver
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);
}

void transmit(uint8_t data)
{
  while (!(UCSR0A & (1 << UDRE0)))
    ;

  UDR0 = data;
}

uint8_t receive()
{
  // Polling
  while (!(UCSR0A & (1 << RXC0)))
    ;

  return UDR0;
}

void setup()
{
  init();
}

char data;

void loop()
{
  // First poll/wait for a character input
  data = receive();
  // Then echo the character to terminal client
  transmit(data);
}
